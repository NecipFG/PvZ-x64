// VHSFilter.cpp
// Full-screen VHS tape + fisheye post-processing effect for the PvZ boss fight.
//
// Pipeline (executed each frame, in order):
//   1. Fisheye warp, organic wave wobble, and multi-pass chromatic aberration
//   2. Soft analog scrolling scanlines
//   3. Low-res film grain, single-frame tape dropouts, and tracking snow
//   4. VHS tracking bar hum & scroll logic
//   5. Elliptical vignette shading
//
// All processing runs in the SDL2 rendering pipeline using hardware geometry and blits.

#include "VHSFilter.h"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ---------------------------------------------------------------------------
// Tuning constants
// ---------------------------------------------------------------------------

// Fisheye (Barrel distortion grid)
static constexpr int   FISHEYE_GRID_COLS   = 48;    // mesh columns
static constexpr int   FISHEYE_GRID_ROWS   = 36;    // mesh rows
static constexpr float FISHEYE_STRENGTH    = 0.06f; // barrel coefficient k

// Noise & Grain
static constexpr Uint8 NOISE_ALPHA_MAX     = 18;    // max noise pixel alpha
static constexpr int   NOISE_TEX_W         = 160;   // low-resolution for coarser grain
static constexpr int   NOISE_TEX_H         = 120;

// Tracking line
static constexpr float TRACKING_SPEED      = 0.35f; // fraction of screen height per second
static constexpr int   TRACKING_HEIGHT     = 12;    // band height in pixels
static constexpr Uint8 TRACKING_ALPHA      = 26;    // band opacity (kept low: real tracking errors darken, not glow)
static constexpr float TRACKING_PROB       = 0.002f;// chance of spawning a tracking band per frame

// Vignette
static constexpr Uint8 VIGNETTE_ALPHA      = 65;    // outer border darkness

// ---------------------------------------------------------------------------
// Static member definitions
// ---------------------------------------------------------------------------

SDL_Texture* VHSFilter::sNoiseTexture = nullptr;
int          VHSFilter::sNoiseW       = 0;
int          VHSFilter::sNoiseH       = 0;
SDL_Texture* VHSFilter::sVignetteTexture = nullptr;
SDL_Texture* VHSFilter::sScanlineTexture = nullptr;
int          VHSFilter::sScanlineH       = 0;
int          VHSFilter::sGlitchDuration  = 0;
float        VHSFilter::sGlitchScrollY   = 0.0f;
float        VHSFilter::sGlitchShake     = 0.0f;
unsigned int VHSFilter::sRand            = 0x1234ABCD;

// Tracking line state
static bool  sTrackActive  = false;
static float sTrackY       = 0.0f;
static Uint8 sTrackAlpha   = 0;

// ---------------------------------------------------------------------------
// Tiny LCG
// ---------------------------------------------------------------------------

unsigned int VHSFilter::FastRand()
{
    sRand = sRand * 1664525u + 1013904223u;
    return sRand;
}

void VHSFilter::SeedRand(unsigned int seed)
{
    sRand = seed ^ 0xDEADBEEF;
}

// ---------------------------------------------------------------------------
// Cleanup
// ---------------------------------------------------------------------------

void VHSFilter::Cleanup()
{
    if (sNoiseTexture)
    {
        SDL_DestroyTexture(sNoiseTexture);
        sNoiseTexture = nullptr;
    }
    sNoiseW = 0;
    sNoiseH = 0;

    if (sVignetteTexture)
    {
        SDL_DestroyTexture(sVignetteTexture);
        sVignetteTexture = nullptr;
    }

    if (sScanlineTexture)
    {
        SDL_DestroyTexture(sScanlineTexture);
        sScanlineTexture = nullptr;
    }
    sScanlineH = 0;
}

// ---------------------------------------------------------------------------
// Apply – master entry point
// ---------------------------------------------------------------------------

void VHSFilter::Apply(SDL_Renderer* renderer,
                      SDL_Texture*  sourceTexture,
                      int           screenW,
                      int           screenH,
                      int           frameIndex)
{
    if (!renderer || !sourceTexture) return;

    // Seed per-frame noise with frame index
    SeedRand((unsigned int)frameIndex * 6364136223846793005u + 1442695040888963407u);

    // Handle lost sync glitch state
    if (sGlitchDuration > 0)
    {
        sGlitchDuration--;
        sGlitchScrollY += 0.04f; // vertical roll speed
        if (sGlitchScrollY > 1.0f) sGlitchScrollY -= 1.0f;
        sGlitchShake = ((float)(FastRand() & 0xFF) / 255.0f - 0.5f) * 14.0f; // horizontal shake
    }
    else
    {
        sGlitchScrollY = 0.0f;
        sGlitchShake = 0.0f;

        // Rare chance to trigger a lost sync glitch
        float roll = (float)(FastRand() & 0xFFFF) / 65535.0f;
        if (roll < 0.0006f) // ~once every 30 seconds
        {
            sGlitchDuration = 15 + (FastRand() % 25); // 15 to 35 frames
        }
    }

    // 1. Fisheye warp + Horizontal wave wobble + Multi-pass chromatic aberration + Vertical sync roll
    ApplyFisheye(renderer, sourceTexture, screenW, screenH, frameIndex);

    // 2. Scanlines (smooth analog scrolling scanlines)
    ApplyScanlines(renderer, screenW, screenH, frameIndex);

    // 3. Film grain / noise + single-frame tape dropouts + tracking snow
    ApplyNoise(renderer, screenW, screenH, frameIndex);

    // 4. VHS tracking line
    ApplyTracking(renderer, screenW, screenH, frameIndex);

    // 5. Vignette border (elliptical gradient)
    ApplyVignette(renderer, screenW, screenH);
}

// ---------------------------------------------------------------------------
// 1. Fisheye & Chromatic Aberration & Wobble
// ---------------------------------------------------------------------------

void VHSFilter::ApplyFisheye(SDL_Renderer* renderer,
                             SDL_Texture*  sourceTexture,
                             int           screenW,
                             int           screenH,
                             int           frameIndex)
{
    const int cols = FISHEYE_GRID_COLS;
    const int rows = FISHEYE_GRID_ROWS;
    const float k  = FISHEYE_STRENGTH;

    struct ChannelPass {
        SDL_Color colorMod;
        SDL_BlendMode blendMode;
        float radialScale;
        float shiftX;
        float shiftY;
    };

    // Extra chromatic aberration during a sync glitch
    float glitchCA = 0.0f;
    if (sGlitchDuration > 0)
    {
        glitchCA = 0.008f * sin(frameIndex * 0.4f);
    }

    ChannelPass passes[3] = {
        // Base pass: Full color image warped normally
        { {255, 255, 255, 255}, SDL_BLENDMODE_NONE, 1.0f, 0.0f, 0.0f },
        // Red pass: faint additive glow, tight radius (power = alpha, radius = radialScale)
        { {255, 0, 0, 7}, SDL_BLENDMODE_ADD, 1.0012f + glitchCA,  0.0005f * sin(frameIndex * 0.12f), 0.0f },
        // Blue pass: faint additive glow, opposite shift
        { {0, 0, 255, 7}, SDL_BLENDMODE_ADD, 0.9988f - glitchCA, -0.0005f * sin(frameIndex * 0.12f), 0.0f }
    };

    for (int pIndex = 0; pIndex < 3; ++pIndex)
    {
        const auto& pass = passes[pIndex];
        std::vector<SDL_Vertex> verts;
        verts.reserve(cols * rows * 6);

        // Helper: apply barrel distortion to a normalised UV in [-1, 1]
        auto distort = [&](float nx, float ny, float& outU, float& outV)
        {
            float r2 = nx * nx + ny * ny;
            float factor = 1.0f + k * r2;
            float dx = nx * factor;
            float dy = ny * factor;

            // Apply zoom (to crop clamped edges) and channel-specific radial scale
            float zoom = 0.96f / (1.0f + k * 0.3f); 
            dx *= zoom * pass.radialScale;
            dy *= zoom * pass.radialScale;

            // Apply channel-specific horizontal/vertical translation
            dx += pass.shiftX;
            dy += pass.shiftY;

            // Map back to [0, 1] UV
            outU = dx * 0.5f + 0.5f;
            outV = dy * 0.5f + 0.5f;

            // Vertical roll sync roll
            if (sGlitchScrollY > 0.0f)
            {
                outV = fmod(outV + sGlitchScrollY, 1.0f);
                if (outV < 0.0f) outV += 1.0f;
            }

            // Clamp to avoid sampling out of bounds
            if (outU < 0.0f) outU = 0.0f;
            if (outU > 1.0f) outU = 1.0f;
            if (outV < 0.0f) outV = 0.0f;
            if (outV > 1.0f) outV = 1.0f;
        };

        // Helper to get screen position with horizontal wobble
        auto getVertexX = [&](int col, int row, float defaultX, float sy) -> float
        {
            float ny = (float)row / rows * 2.0f - 1.0f; // [-1, 1]

            // 1. Organic tape waving: sum of three low-frequency sines with
            //    irrational frequency ratios and a per-row phase offset so the
            //    distortion never repeats vertically. Looks like real tape
            //    slack rather than a perfect uniform ripple.
            float rowPhase = (float)row * 0.37f;
            float t        = (float)frameIndex * 0.18f;
            float wave1    = sin(ny * M_PI *  2.5f + t          + rowPhase)        * 0.55f;
            float wave2    = sin(ny * M_PI *  6.3f + t * 1.7f  + rowPhase * 1.3f) * 0.22f;
            float wave3    = sin(ny * M_PI * 13.7f + t * 0.6f  + rowPhase * 0.7f) * 0.08f;
            float wobble   = wave1 + wave2 + wave3;

            // 2. High-frequency micro-jitter
            float microJitter = 0.0f;
            // Use deterministic seeding per row/frame to keep color channels aligned
            unsigned int rowSeed = frameIndex * 1337 + row * 997;
            unsigned int rVal = rowSeed * 1664525u + 1013904223u;
            if ((rVal % 100) < 12)
            {
                microJitter = ((float)(rVal % 100) / 100.0f - 0.5f) * 2.2f;
            }

            // 3. Top of screen head sync flag-waving
            float topFactor = pow(1.0f - (ny + 1.0f) * 0.5f, 4.0f);
            float topWobble = topFactor * sin((float)frameIndex * 0.2f) * 5.0f;

            // 4. Tracking line tearing
            float trackingTear = 0.0f;
            if (sTrackActive)
            {
                float dist = std::abs(sy - sTrackY);
                if (dist < 40.0f)
                {
                    float t = 1.0f - (dist / 40.0f);
                    // Use deterministic hash for tearing to keep channels aligned
                    unsigned int tearSeed = frameIndex * 555 + row * 333;
                    unsigned int tVal = tearSeed * 1664525u + 1013904223u;
                    trackingTear = t * t * (((float)(tVal % 200) / 200.0f - 0.5f) * 20.0f);
                }
            }

            // 5. Global glitch shake
            float glitchShake = 0.0f;
            if (sGlitchDuration > 0)
            {
                glitchShake = sGlitchShake;
            }

            return defaultX + wobble + microJitter + topWobble + trackingTear + glitchShake;
        };

        for (int row = 0; row < rows; ++row)
        {
            for (int col = 0; col < cols; ++col)
            {
                // Default coordinates
                float sx0 = (float)(col    ) / cols * screenW;
                float sx1 = (float)(col + 1) / cols * screenW;
                float sy0 = (float)(row    ) / rows * screenH;
                float sy1 = (float)(row + 1) / rows * screenH;

                float nx0 = (float)(col    ) / cols * 2.0f - 1.0f;
                float nx1 = (float)(col + 1) / cols * 2.0f - 1.0f;
                float ny0 = (float)(row    ) / rows * 2.0f - 1.0f;
                float ny1 = (float)(row + 1) / rows * 2.0f - 1.0f;

                // Vertices with horizontal wobble
                float vx00 = getVertexX(col,     row,     sx0, sy0);
                float vx10 = getVertexX(col + 1, row,     sx1, sy0);
                float vx01 = getVertexX(col,     row + 1, sx0, sy1);
                float vx11 = getVertexX(col + 1, row + 1, sx1, sy1);

                float u00, v00, u10, v10, u01, v01, u11, v11;
                distort(nx0, ny0, u00, v00);
                distort(nx1, ny0, u10, v10);
                distort(nx0, ny1, u01, v01);
                distort(nx1, ny1, u11, v11);

                // Calculate vertical blanking bar colors for vertices
                auto getVertColor = [&](float v) -> SDL_Color
                {
                    SDL_Color c = pass.colorMod;
                    if (sGlitchScrollY > 0.0f)
                    {
                        float dist = v;
                        if (dist > 0.5f) dist = 1.0f - dist;
                        if (dist < 0.035f) // 3.5% height blanking bar
                        {
                            c = {0, 0, 0, pass.colorMod.a};
                        }
                    }
                    return c;
                };

                SDL_Color c00 = getVertColor(v00);
                SDL_Color c10 = getVertColor(v10);
                SDL_Color c01 = getVertColor(v01);
                SDL_Color c11 = getVertColor(v11);

                // Triangle 1: top-left, top-right, bottom-left
                verts.push_back({{vx00, sy0}, c00, {u00, v00}});
                verts.push_back({{vx10, sy0}, c10, {u10, v10}});
                verts.push_back({{vx01, sy1}, c01, {u01, v01}});

                // Triangle 2: top-right, bottom-right, bottom-left
                verts.push_back({{vx10, sy0}, c10, {u10, v10}});
                verts.push_back({{vx11, sy1}, c11, {u11, v11}});
                verts.push_back({{vx01, sy1}, c01, {u01, v01}});
            }
        }

        SDL_SetTextureColorMod(sourceTexture, 255, 255, 255);
        SDL_SetTextureBlendMode(sourceTexture, pass.blendMode);
        SDL_SetTextureAlphaMod(sourceTexture, 255);

        SDL_RenderGeometry(renderer, sourceTexture,
                           verts.data(), (int)verts.size(),
                           nullptr, 0);
    }

    // Restore source texture state
    SDL_SetTextureColorMod(sourceTexture, 255, 255, 255);
    SDL_SetTextureBlendMode(sourceTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(sourceTexture, 255);
}

// ---------------------------------------------------------------------------
// 2. Scanlines – vertical 1D scrolling texture
// ---------------------------------------------------------------------------

void VHSFilter::ApplyScanlines(SDL_Renderer* renderer, int screenW, int screenH, int frameIndex)
{
    if (!sScanlineTexture || sScanlineH != screenH)
    {
        if (sScanlineTexture) SDL_DestroyTexture(sScanlineTexture);
        sScanlineTexture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            1,
            screenH
        );
        sScanlineH = screenH;
    }

    if (!sScanlineTexture) return;

    void* pixels = nullptr;
    int pitch = 0;
    if (SDL_LockTexture(sScanlineTexture, nullptr, &pixels, &pitch) == 0)
    {
        Uint8* p = (Uint8*)pixels;
        for (int y = 0; y < screenH; ++y)
        {
            // Scrolling vertical scanline phase
            // Period is ~2.4 pixels for a soft analog look
            float angle = (float)y * (2.0f * M_PI / 2.4f) + (float)frameIndex * 0.04f;
            float sineVal = sin(angle);
            
            // Map sine value to alpha range [6, 24]
            Uint8 alpha = (Uint8)(6 + 18 * (0.5f + 0.5f * sineVal));

            // ARGB8888 in memory (little-endian BGRA): [B][G][R][A]
            p[y * 4 + 0] = 0;     // B
            p[y * 4 + 1] = 0;     // G
            p[y * 4 + 2] = 0;     // R
            p[y * 4 + 3] = alpha; // A
        }
        SDL_UnlockTexture(sScanlineTexture);
    }

    SDL_SetTextureBlendMode(sScanlineTexture, SDL_BLENDMODE_BLEND);
    SDL_Rect dst = {0, 0, screenW, screenH};
    SDL_RenderCopy(renderer, sScanlineTexture, nullptr, &dst);
}

// ---------------------------------------------------------------------------
// 3. Noise & Grain & Dropouts
// ---------------------------------------------------------------------------

void VHSFilter::ApplyNoise(SDL_Renderer* renderer, int screenW, int screenH, int frameIndex)
{
    if (!sNoiseTexture || sNoiseW != NOISE_TEX_W || sNoiseH != NOISE_TEX_H)
    {
        if (sNoiseTexture) SDL_DestroyTexture(sNoiseTexture);
        sNoiseTexture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            NOISE_TEX_W,
            NOISE_TEX_H
        );
        sNoiseW = NOISE_TEX_W;
        sNoiseH = NOISE_TEX_H;
    }

    if (!sNoiseTexture) return;

    void* pixels = nullptr;
    int   pitch  = 0;
    if (SDL_LockTexture(sNoiseTexture, nullptr, &pixels, &pitch) == 0)
    {
        Uint8* p = (Uint8*)pixels;

        // Calculate tracking band in noise coordinate space
        float noiseTrackY = 0.0f;
        float noiseTrackH = 0.0f;
        if (sTrackActive)
        {
            noiseTrackY = (sTrackY / (float)screenH) * (float)NOISE_TEX_H;
            noiseTrackH = ((float)TRACKING_HEIGHT / (float)screenH) * (float)NOISE_TEX_H;
        }

        for (int y = 0; y < NOISE_TEX_H; ++y)
        {
            bool isTrackingRow = sTrackActive && (std::abs((float)y - noiseTrackY) < noiseTrackH * 1.5f);
            
            // Check for tape dropout streak on this row
            bool isDropout = false;
            int dropoutStart = 0;
            int dropoutLen = 0;
            Uint8 dropoutVal = 0;
            Uint8 dropoutAlpha = 0;

            if (!isTrackingRow && (FastRand() % 1000) < 12) // 1.2% chance per row
            {
                isDropout = true;
                dropoutStart = FastRand() % NOISE_TEX_W;
                dropoutLen = 6 + (FastRand() % 20); // 6 to 25 pixels wide
                dropoutVal = (FastRand() & 1) ? 255 : 0; // white or black scratch
                dropoutAlpha = 150 + (FastRand() % 80);
            }

            for (int x = 0; x < NOISE_TEX_W; ++x)
            {
                int idx = (y * NOISE_TEX_W + x) * 4;
                unsigned int r = FastRand();

                if (isTrackingRow)
                {
                    // Heavy tracking snow (high contrast, high opacity)
                    Uint8 val = (r & 1) ? 255 : 0;
                    Uint8 alpha = 130 + (r % 70); // alpha [130, 200]
                    p[idx + 0] = val; // B
                    p[idx + 1] = val; // G
                    p[idx + 2] = val; // R
                    p[idx + 3] = alpha; // A
                }
                else if (isDropout && x >= dropoutStart && x < dropoutStart + dropoutLen)
                {
                    // Dropout streak pixel
                    p[idx + 0] = dropoutVal; // B
                    p[idx + 1] = dropoutVal; // G
                    p[idx + 2] = dropoutVal; // R
                    p[idx + 3] = dropoutAlpha; // A
                }
                else
                {
                    // Normal fine grain (88% dark grain, 12% light grain — bias
                    // toward dark so the noise texture doesn't brighten the frame)
                    bool isDark = (r % 100) < 88;
                    Uint8 grey = isDark ? 0 : 255;
                    Uint8 alpha = (r & 0xFF) % 11; // max alpha 10 for subtler noise
                    p[idx + 0] = grey;
                    p[idx + 1] = grey;
                    p[idx + 2] = grey;
                    p[idx + 3] = alpha;
                }
            }
        }
        SDL_UnlockTexture(sNoiseTexture);
    }

    SDL_SetTextureBlendMode(sNoiseTexture, SDL_BLENDMODE_BLEND);
    SDL_Rect dst = {0, 0, screenW, screenH};
    SDL_RenderCopy(renderer, sNoiseTexture, nullptr, &dst);
}

// ---------------------------------------------------------------------------
// 4. VHS tracking line
// ---------------------------------------------------------------------------

void VHSFilter::ApplyTracking(SDL_Renderer* renderer, int screenW, int screenH, int frameIndex)
{
    float spawnRoll = (float)(VHSFilter::FastRand() & 0xFFFF) / 65535.0f;
    if (!sTrackActive && spawnRoll < TRACKING_PROB)
    {
        sTrackActive = true;
        sTrackY      = -(float)TRACKING_HEIGHT;
        sTrackAlpha  = (Uint8)(TRACKING_ALPHA / 3 +
                               (VHSFilter::FastRand() & 0xFF) % (TRACKING_ALPHA / 3));
    }

    if (!sTrackActive) return;

    // Scroll downward
    sTrackY += TRACKING_SPEED * screenH / 60.0f;

    if (sTrackY > (float)screenH)
    {
        sTrackActive = false;
        return;
    }

    int y0 = (int)sTrackY;
    int y1 = y0 + TRACKING_HEIGHT;
    if (y1 > screenH) y1 = screenH;

    // Draw a subtle darkening band (not additive) — real tracking errors
    // smear a noisy darker stripe, they don't make the screen glow.
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, sTrackAlpha);
    SDL_Rect band = {0, y0, screenW, y1 - y0};
    SDL_RenderFillRect(renderer, &band);
}

// ---------------------------------------------------------------------------
// 5. Vignette – cached 32x32 radial map
// ---------------------------------------------------------------------------

void VHSFilter::ApplyVignette(SDL_Renderer* renderer, int screenW, int screenH)
{
    if (!sVignetteTexture)
    {
        sVignetteTexture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STATIC,
            32,
            32
        );

        if (sVignetteTexture)
        {
            Uint32 pixels[32 * 32];
            for (int y = 0; y < 32; ++y)
            {
                for (int x = 0; x < 32; ++x)
                {
                    float nx = (x - 15.5f) / 15.5f;
                    float ny = (y - 15.5f) / 15.5f;
                    float dist = sqrt(nx*nx + ny*ny);

                    // Starts fading around 60% radius
                    float t = dist - 0.6f;
                    if (t < 0.0f) t = 0.0f;
                    t = t / 0.4f;
                    if (t > 1.0f) t = 1.0f;

                    float factor = t * t; // quadratic profile
                    Uint8 alpha = (Uint8)(VIGNETTE_ALPHA * factor);

                    // ARGB8888 BGRA little-endian
                    pixels[y * 32 + x] = ((Uint32)alpha << 24);
                }
            }
            SDL_UpdateTexture(sVignetteTexture, nullptr, pixels, 32 * 4);
        }
    }

    if (sVignetteTexture)
    {
        SDL_SetTextureBlendMode(sVignetteTexture, SDL_BLENDMODE_BLEND);
        SDL_Rect dst = {0, 0, screenW, screenH};
        SDL_RenderCopy(renderer, sVignetteTexture, nullptr, &dst);
    }
}
