#ifndef __VHSFILTER_H__
#define __VHSFILTER_H__

#include <SDL2/SDL.h>

// ---------------------------------------------------------------------------
// VHSFilter
//
// A full-screen post-processing pass that applies a combined VHS tape +
// fisheye (barrel distortion) effect.  It is designed to be called once per
// frame, just before SDL_RenderPresent(), and is completely self-contained
// inside the SexyAppFramework SDL2 layer.
//
// Usage:
//   VHSFilter::Apply(renderer, sourceTexture, screenW, screenH, frameIndex);
//
// The filter is built as a pure-static utility class so it needs no lifetime
// management; all SDL resources it allocates are lazily created and cached.
// Call VHSFilter::Cleanup() when the renderer is destroyed.
// ---------------------------------------------------------------------------

class VHSFilter
{
public:
    // Apply the full VHS + fisheye effect.
    //   renderer      – the SDL_Renderer that owns the window backbuffer
    //   sourceTexture – TEXTUREACCESS_TARGET texture containing the finished
    //                   game frame (the one that was used as render-target)
    //   screenW/H     – logical screen dimensions
    //   frameIndex    – monotonically increasing frame counter (drives time-
    //                   dependent effects such as jitter and tracking lines)
    static void Apply(SDL_Renderer* renderer,
                      SDL_Texture*  sourceTexture,
                      int           screenW,
                      int           screenH,
                      int           frameIndex);

    // Release all internally cached SDL textures / surfaces.
    // Must be called before SDL_DestroyRenderer().
    static void Cleanup();

private:
    // --- Fisheye pass -------------------------------------------------------
    // Renders sourceTexture onto the current render target using a barrel-
    // distortion UV mesh so the image looks like it was shot through a
    // wide-angle lens. Also handles horizontal wobble, chromatic aberration,
    // and vertical roll.
    static void ApplyFisheye(SDL_Renderer* renderer,
                             SDL_Texture*  sourceTexture,
                             int           screenW,
                             int           screenH,
                             int           frameIndex);

    // --- VHS overlay passes -------------------------------------------------
    static void ApplyScanlines(SDL_Renderer* renderer, int screenW, int screenH, int frameIndex);
    static void ApplyNoise    (SDL_Renderer* renderer, int screenW, int screenH, int frameIndex);
    static void ApplyTracking (SDL_Renderer* renderer, int screenW, int screenH, int frameIndex);
    static void ApplyVignette (SDL_Renderer* renderer, int screenW, int screenH);

    // --- Cached GPU resources -----------------------------------------------
    static SDL_Texture* sNoiseTexture;    // ~screenW x screenH RGBA noise map
    static int          sNoiseW;
    static int          sNoiseH;

    static SDL_Texture* sVignetteTexture; // cached 32x32 radial vignette map
    static SDL_Texture* sScanlineTexture; // cached 1xscreenH scrolling scanline map
    static int          sScanlineH;

    // --- Glitch / lost sync state -------------------------------------------
    static int          sGlitchDuration;  // remaining frames for lost sync glitch
    static float        sGlitchScrollY;   // vertical shift offset
    static float        sGlitchShake;     // horizontal shake amplitude

    // Simple LCG used for per-frame noise (fast, deterministic, no stdlib rand())
    static unsigned int sRand;
    static unsigned int FastRand();
    static void         SeedRand(unsigned int seed);
};

#endif // __VHSFILTER_H__
