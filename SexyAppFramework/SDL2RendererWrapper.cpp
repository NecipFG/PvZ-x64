#include "SDL2RendererWrapper.h"
#include "Image.h"
#include <chrono>
#include <thread>
#include "MemoryImage.h"
#include "DDImage.h"
#include "DDInterface.h"
#include <cmath>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

extern void FrameworkLog(const char* theFormat, ...);

namespace Sexy
{

SDL2RendererWrapper* gSDL2Renderer = nullptr;

SDL2RendererWrapper::SDL2RendererWrapper()
    : mWindow(nullptr), mRenderer(nullptr), mWidth(800), mHeight(600), mIsWindowed(true), mTargetFPS(120)
{
}

SDL2RendererWrapper::~SDL2RendererWrapper()
{
    Cleanup();
}

bool SDL2RendererWrapper::Init(HWND hWnd, int width, int height, bool windowed)
{
    mWidth = width;
    mHeight = height;
    mIsWindowed = windowed;

    FrameworkLog("SDL2RendererWrapper::Init: hWnd=%p, width=%d, height=%d, windowed=%d\n", hWnd, width, height, windowed);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        FrameworkLog("SDL2RendererWrapper::Init: SDL_Init failed: %s\n", SDL_GetError());
        return false;
    }

    // Attempt to hook onto legacy Win32 Window, or create native SDL window if NULL
#ifdef _WIN32
    if (hWnd != NULL)
    {
        FrameworkLog("SDL2RendererWrapper::Init: Attempting SDL_CreateWindowFrom with hWnd=%p\n", hWnd);
        mWindow = SDL_CreateWindowFrom((void*)hWnd);
        if (mWindow == nullptr)
        {
            FrameworkLog("SDL2RendererWrapper::Init: SDL_CreateWindowFrom failed: %s. Will fallback to creating native window.\n", SDL_GetError());
        }
        else
        {
            FrameworkLog("SDL2RendererWrapper::Init: SDL_CreateWindowFrom succeeded!\n");
        }
    }
#endif
    
    if (mWindow == nullptr)
    {
        FrameworkLog("SDL2RendererWrapper::Init: Creating native SDL window.\n");
        mWindow = SDL_CreateWindow(
            "SexyAppFramework SDL2 Modding Base",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            mWidth, mHeight,
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | (windowed ? 0 : SDL_WINDOW_FULLSCREEN)
        );
    }

    if (mWindow == nullptr)
    {
        FrameworkLog("SDL2RendererWrapper::Init: Failed to create/hook window: %s\n", SDL_GetError());
        Cleanup();
        return false;
    }

    // Parse environment variables for target FPS and VSync
    bool useVSync = false;
    const char* env_vsync = getenv("PVZ_VSYNC");
    if (env_vsync && atoi(env_vsync) != 0)
    {
        useVSync = true;
    }

    int targetFps = 120;
    const char* env_fps = getenv("PVZ_FPS");
    if (env_fps)
    {
        targetFps = atoi(env_fps);
        if (targetFps <= 0) targetFps = 120;
    }
    else if (useVSync)
    {
        SDL_DisplayMode mode;
        if (SDL_GetWindowDisplayMode(mWindow, &mode) == 0 && mode.refresh_rate > 0)
        {
            targetFps = mode.refresh_rate;
        }
    }
    mTargetFPS = targetFps;
    FrameworkLog("SDL2RendererWrapper::Init: Target FPS is %d, VSync is %s\n", mTargetFPS, useVSync ? "ON" : "OFF");

    Uint32 flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
    if (useVSync)
    {
        flags |= SDL_RENDERER_PRESENTVSYNC;
    }

    // Hardware accelerated renderer
    FrameworkLog("SDL2RendererWrapper::Init: Creating SDL renderer.\n");
    mRenderer = SDL_CreateRenderer(
        mWindow, -1,
        flags
    );

    if (mRenderer == nullptr)
    {
        FrameworkLog("SDL2RendererWrapper::Init: Failed to create renderer: %s\n", SDL_GetError());
        Cleanup();
        return false;
    }

    SDL_RendererInfo rendererInfo;
    if (SDL_GetRendererInfo(mRenderer, &rendererInfo) == 0)
    {
        FrameworkLog("SDL2RendererWrapper::Init: SDL2RendererWrapper successfully initialized with driver: %s\n", rendererInfo.name);
    }
    else
    {
        FrameworkLog("SDL2RendererWrapper::Init: SDL2RendererWrapper successfully initialized! (Failed to get renderer info: %s)\n", SDL_GetError());
    }
    SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
    SDL_StartTextInput();
    return true;
}

void SDL2RendererWrapper::Cleanup()
{
    for (auto& pair : mTextureCache)
    {
        if (pair.second.mTexture)
        {
            SDL_DestroyTexture(pair.second.mTexture);
        }
    }
    mTextureCache.clear();

    if (mRenderer)
    {
        SDL_DestroyRenderer(mRenderer);
        mRenderer = nullptr;
    }

    if (mWindow)
    {
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }

    SDL_Quit();
}

SDL_Texture* SDL2RendererWrapper::GetTextureForImage(Image* image, bool isRenderTarget)
{
    if (!image) return nullptr;

    MemoryImage* memImage = dynamic_cast<MemoryImage*>(image);
    if (!memImage) return nullptr;

    DDImage* ddImage = dynamic_cast<DDImage*>(image);
    bool isTarget = isRenderTarget || (ddImage != nullptr && ddImage->mDDInterface != nullptr && ddImage->mDDInterface->mScreenImage == ddImage);

    auto it = mTextureCache.find(image);
    if (it != mTextureCache.end())
    {
        if (isTarget)
        {
            int access = 0;
            SDL_QueryTexture(it->second.mTexture, NULL, &access, NULL, NULL);
            if (access != SDL_TEXTUREACCESS_TARGET)
            {
                FrameworkLog("SDL2RendererWrapper::GetTextureForImage: Upgrading texture for image=%p to TARGET\n", image);
                SDL_DestroyTexture(it->second.mTexture);
                mTextureCache.erase(it);
            }
            else
            {
                // Check if software bits changed
                if (memImage->mBitsChanged || memImage->mBitsChangedCount != it->second.mBitsChangedCount)
                {
                    ulong* bits = memImage->GetBits();
                    if (bits)
                    {
                        SDL_UpdateTexture(it->second.mTexture, NULL, bits, memImage->mWidth * sizeof(uint32_t));
                    }
                    memImage->mBitsChanged = false;
                    it->second.mBitsChangedCount = memImage->mBitsChangedCount;
                }
                return it->second.mTexture;
            }
        }
        else
        {
            // Check if software bits changed
            if (memImage->mBitsChanged || memImage->mBitsChangedCount != it->second.mBitsChangedCount)
            {
                ulong* bits = memImage->GetBits();
                if (bits)
                {
                    SDL_UpdateTexture(it->second.mTexture, NULL, bits, memImage->mWidth * sizeof(uint32_t));
                }
                memImage->mBitsChanged = false;
                it->second.mBitsChangedCount = memImage->mBitsChangedCount;
            }
            return it->second.mTexture;
        }
    }

    int access = isTarget ? SDL_TEXTUREACCESS_TARGET : SDL_TEXTUREACCESS_STREAMING;
    FrameworkLog("SDL2RendererWrapper::GetTextureForImage: Creating texture for image=%p (width=%d, height=%d, target=%d, access=%d)\n",
                 image, memImage->mWidth, memImage->mHeight, isTarget, access);
    SDL_Texture* texture = SDL_CreateTexture(
        mRenderer,
        SDL_PIXELFORMAT_ARGB8888,
        access,
        memImage->mWidth,
        memImage->mHeight
    );

    if (texture)
    {
        mTextureCache[image] = { texture, memImage->mBitsChangedCount };
        // Upload initial pixel bits if they exist
        ulong* bits = memImage->GetBits();
        if (bits)
        {
            SDL_UpdateTexture(texture, NULL, bits, memImage->mWidth * sizeof(uint32_t));
        }
    }

    return texture;
}

void SDL2RendererWrapper::DestroyTextureForImage(Image* image)
{
    auto it = mTextureCache.find(image);
    if (it != mTextureCache.end())
    {
        if (it->second.mTexture)
        {
            SDL_DestroyTexture(it->second.mTexture);
        }
        mTextureCache.erase(it);
    }
}

void SDL2RendererWrapper::UpdateTextureFromBits(Image* image)
{
    MemoryImage* memImage = dynamic_cast<MemoryImage*>(image);
    if (!memImage) return;

    ulong* bits = memImage->GetBits();
    if (!bits) return;

    SDL_Texture* texture = GetTextureForImage(image);
    if (!texture) return;

    SDL_UpdateTexture(texture, NULL, bits, memImage->mWidth * sizeof(uint32_t));
    mTextureCache[image].mBitsChangedCount = memImage->mBitsChangedCount;
}

void SDL2RendererWrapper::ApplyTextureStates(SDL_Texture* texture, const Color& color, int drawMode)
{
    // Color modulation
    SDL_SetTextureColorMod(texture, color.mRed, color.mGreen, color.mBlue);
    SDL_SetTextureAlphaMod(texture, color.mAlpha);

    // Blending Mode
    if (drawMode == 1) // DRAWMODE_ADDITIVE
    {
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
    }
    else
    {
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    }
}

void SDL2RendererWrapper::ResetTextureStates(SDL_Texture* texture)
{
    SDL_SetTextureColorMod(texture, 255, 255, 255);
    SDL_SetTextureAlphaMod(texture, 255);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
}

void SDL2RendererWrapper::BeginFrame()
{
    static int sBeginFrameCount = 0;
    if (sBeginFrameCount < 50)
    {
        sBeginFrameCount++;
        FrameworkLog("SDL2RendererWrapper::BeginFrame: Call %d\n", sBeginFrameCount);
    }
    
    if (SDL_SetRenderTarget(mRenderer, NULL) < 0)
    {
        FrameworkLog("SDL2RendererWrapper::BeginFrame: SDL_SetRenderTarget failed: %s\n", SDL_GetError());
    }
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mRenderer);
}

void SDL2RendererWrapper::Present()
{
    static int sPresentCount = 0;
    if (sPresentCount < 50)
    {
        sPresentCount++;
        FrameworkLog("SDL2RendererWrapper::Present: Call %d\n", sPresentCount);
    }

    SDL_RenderPresent(mRenderer);
}

void SDL2RendererWrapper::SetClipRect(const Rect& clipRect)
{
    SDL_Rect sdlRect = { clipRect.mX, clipRect.mY, clipRect.mWidth, clipRect.mHeight };
    SDL_RenderSetClipRect(mRenderer, &sdlRect);
}

void SDL2RendererWrapper::ClearClipRect()
{
    SDL_RenderSetClipRect(mRenderer, NULL);
}

void SDL2RendererWrapper::SetRenderTarget(Image* destImage)
{
    static int sSetRenderTargetCount = 0;
    if (sSetRenderTargetCount < 50)
    {
        sSetRenderTargetCount++;
        FrameworkLog("SDL2RendererWrapper::SetRenderTarget: Call %d destImage=%p\n", sSetRenderTargetCount, destImage);
    }

    if (destImage == nullptr)
    {
        if (SDL_SetRenderTarget(mRenderer, nullptr) < 0)
        {
            FrameworkLog("SDL2RendererWrapper::SetRenderTarget: Failed to set default render target: %s\n", SDL_GetError());
        }
    }
    else
    {
        SDL_Texture* targetTex = GetTextureForImage(destImage, true);
        if (targetTex == nullptr)
        {
            FrameworkLog("SDL2RendererWrapper::SetRenderTarget: GetTextureForImage returned NULL for destImage=%p\n", destImage);
        }
        else if (SDL_SetRenderTarget(mRenderer, targetTex) < 0)
        {
            FrameworkLog("SDL2RendererWrapper::SetRenderTarget: SDL_SetRenderTarget failed: %s\n", SDL_GetError());
        }
    }
}

void SDL2RendererWrapper::FillRect(const Rect& rect, const Color& color, int drawMode)
{
    SDL_Rect sdlRect = { rect.mX, rect.mY, rect.mWidth, rect.mHeight };
    
    SDL_BlendMode blendMode = (drawMode == 1) ? SDL_BLENDMODE_ADD : SDL_BLENDMODE_BLEND;
    SDL_SetRenderDrawBlendMode(mRenderer, blendMode);
    SDL_SetRenderDrawColor(mRenderer, color.mRed, color.mGreen, color.mBlue, color.mAlpha);
    SDL_RenderFillRect(mRenderer, &sdlRect);
}

void SDL2RendererWrapper::DrawLine(double startX, double startY, double endX, double endY, const Color& color, int drawMode)
{
    SDL_BlendMode blendMode = (drawMode == 1) ? SDL_BLENDMODE_ADD : SDL_BLENDMODE_BLEND;
    SDL_SetRenderDrawBlendMode(mRenderer, blendMode);
    SDL_SetRenderDrawColor(mRenderer, color.mRed, color.mGreen, color.mBlue, color.mAlpha);
    SDL_RenderDrawLineF(mRenderer, (float)startX, (float)startY, (float)endX, (float)endY);
}

void SDL2RendererWrapper::Blt(Image* srcImage, int destX, int destY, const Rect& srcRect, const Color& color, int drawMode)
{
    static int sBltCount = 0;
    if (sBltCount < 50)
    {
        sBltCount++;
        FrameworkLog("SDL2RendererWrapper::Blt: Call %d srcImage=%p (width=%d, height=%d) destX=%d, destY=%d\n",
                     sBltCount, srcImage, srcImage->mWidth, srcImage->mHeight, destX, destY);
    }

    SDL_Texture* texture = GetTextureForImage(srcImage);
    if (!texture) return;

    // Sync software bits if modified
    MemoryImage* memImage = dynamic_cast<MemoryImage*>(srcImage);
    if (memImage && memImage->mBitsChanged)
    {
        UpdateTextureFromBits(srcImage);
        memImage->mBitsChanged = false;
    }

    ApplyTextureStates(texture, color, drawMode);

    SDL_Rect sdlSrc = { srcRect.mX, srcRect.mY, srcRect.mWidth, srcRect.mHeight };
    SDL_Rect sdlDest = { destX, destY, srcRect.mWidth, srcRect.mHeight };

    if (SDL_RenderCopy(mRenderer, texture, &sdlSrc, &sdlDest) < 0)
    {
        FrameworkLog("SDL2RendererWrapper::Blt: SDL_RenderCopy failed: %s\n", SDL_GetError());
    }
    ResetTextureStates(texture);
}

void SDL2RendererWrapper::StretchBlt(Image* srcImage, const Rect& destRect, const Rect& srcRect, const Rect& clipRect, const Color& color, int drawMode, bool fastStretch)
{
    static int sStretchBltCount = 0;
    if (sStretchBltCount < 50)
    {
        sStretchBltCount++;
        FrameworkLog("SDL2RendererWrapper::StretchBlt: Call %d srcImage=%p (width=%d, height=%d) destRect={%d,%d,%d,%d}\n",
                     sStretchBltCount, srcImage, srcImage->mWidth, srcImage->mHeight, destRect.mX, destRect.mY, destRect.mWidth, destRect.mHeight);
    }

    SDL_Texture* texture = GetTextureForImage(srcImage);
    if (!texture) return;

    MemoryImage* memImage = dynamic_cast<MemoryImage*>(srcImage);
    if (memImage && memImage->mBitsChanged)
    {
        UpdateTextureFromBits(srcImage);
        memImage->mBitsChanged = false;
    }

    ApplyTextureStates(texture, color, drawMode);
    SetClipRect(clipRect);

    SDL_Rect sdlSrc = { srcRect.mX, srcRect.mY, srcRect.mWidth, srcRect.mHeight };
    SDL_Rect sdlDest = { destRect.mX, destRect.mY, destRect.mWidth, destRect.mHeight };

    if (SDL_RenderCopy(mRenderer, texture, &sdlSrc, &sdlDest) < 0)
    {
        FrameworkLog("SDL2RendererWrapper::StretchBlt: SDL_RenderCopy failed: %s\n", SDL_GetError());
    }
    
    ClearClipRect();
    ResetTextureStates(texture);
}

void SDL2RendererWrapper::BltRotated(Image* srcImage, float destX, float destY, const Rect& srcRect, const Rect& clipRect, const Color& color, int drawMode, double rot, float rotCenterX, float rotCenterY)
{
    SDL_Texture* texture = GetTextureForImage(srcImage);
    if (!texture) return;

    MemoryImage* memImage = dynamic_cast<MemoryImage*>(srcImage);
    if (memImage && memImage->mBitsChanged)
    {
        UpdateTextureFromBits(srcImage);
        memImage->mBitsChanged = false;
    }

    ApplyTextureStates(texture, color, drawMode);
    SetClipRect(clipRect);

    SDL_Rect sdlSrc = { srcRect.mX, srcRect.mY, srcRect.mWidth, srcRect.mHeight };
    SDL_FRect sdlDest = { destX, destY, (float)srcRect.mWidth, (float)srcRect.mHeight };
    
    SDL_FPoint center = { rotCenterX, rotCenterY };
    double degrees = rot * (180.0 / M_PI); // Convert radians to degrees

    SDL_RenderCopyExF(mRenderer, texture, &sdlSrc, &sdlDest, degrees, &center, SDL_FLIP_NONE);

    ClearClipRect();
    ResetTextureStates(texture);
}

void SDL2RendererWrapper::BltMatrix(Image* srcImage, float x, float y, const SexyMatrix3& matrix, const Rect& clipRect, const Color& color, int drawMode, const Rect& srcRect, bool blend)
{
    TriVertex vertices[2][3];
    float w = (float)srcRect.mWidth;
    float h = (float)srcRect.mHeight;

    float w2 = w / 2.0f;
    float h2 = h / 2.0f;

    // Local coordinates of the 4 corners, centered around (0, 0)
    float x0 = -w2, y0 = -h2;
    float x1 = w2,  y1 = -h2;
    float x2 = w2,  y2 = h2;
    float x3 = -w2, y3 = h2;

    // Helper to transform a coordinate through matrix
    auto transform = [&](float lx, float ly, float& tx, float& ty) {
        tx = matrix.m00 * lx + matrix.m01 * ly + matrix.m02 + x - 0.5f;
        ty = matrix.m10 * lx + matrix.m11 * ly + matrix.m12 + y - 0.5f;
    };

    float tx0, ty0, tx1, ty1, tx2, ty2, tx3, ty3;
    transform(x0, y0, tx0, ty0);
    transform(x1, y1, tx1, ty1);
    transform(x2, y2, tx2, ty2);
    transform(x3, y3, tx3, ty3);

    // Texture coords (normalized 0.0 - 1.0)
    float u0 = (float)srcRect.mX / srcImage->mWidth;
    float v0 = (float)srcRect.mY / srcImage->mHeight;
    float u2 = (float)(srcRect.mX + srcRect.mWidth) / srcImage->mWidth;
    float v2 = (float)(srcRect.mY + srcRect.mHeight) / srcImage->mHeight;

    // Triangle 1: (tx0, ty0), (tx1, ty1), (tx3, ty3)
    vertices[0][0] = { tx0, ty0, u0, v0, color.ToInt() };
    vertices[0][1] = { tx1, ty1, u2, v0, color.ToInt() };
    vertices[0][2] = { tx3, ty3, u0, v2, color.ToInt() };

    // Triangle 2: (tx1, ty1), (tx2, ty2), (tx3, ty3)
    vertices[1][0] = { tx1, ty1, u2, v0, color.ToInt() };
    vertices[1][1] = { tx2, ty2, u2, v2, color.ToInt() };
    vertices[1][2] = { tx3, ty3, u0, v2, color.ToInt() };

    BltTrianglesTex(srcImage, vertices, 2, clipRect, color, drawMode, 0, 0, blend);
}

void SDL2RendererWrapper::BltTrianglesTex(Image* texture, const TriVertex vertices[][3], int numTriangles, const Rect& clipRect, const Color& color, int drawMode, float tx, float ty, bool blend)
{
    SDL_Texture* sdlTex = GetTextureForImage(texture);
    if (!sdlTex) return;

    MemoryImage* memImage = dynamic_cast<MemoryImage*>(texture);
    if (memImage && memImage->mBitsChanged)
    {
        UpdateTextureFromBits(texture);
        memImage->mBitsChanged = false;
    }

    // Apply blending modes and reset color mod to white on the texture itself 
    // to avoid double modulating with vertex colors.
    ApplyTextureStates(sdlTex, Color::White, drawMode);
    SetClipRect(clipRect);

    // Flatten vertices into SDL_Vertex structure
    std::vector<SDL_Vertex> sdlVertices;
    sdlVertices.reserve(numTriangles * 3);

    for (int i = 0; i < numTriangles; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            const auto& v = vertices[i][j];
            
            // If v.color is 0 (use default), fallback to the global tint color.
            DWORD packedColor = (v.color != 0) ? v.color : color.ToInt();

            SDL_Color sdlColor = {
                (Uint8)((packedColor >> 16) & 0xFF),
                (Uint8)((packedColor >> 8) & 0xFF),
                (Uint8)(packedColor & 0xFF),
                (Uint8)((packedColor >> 24) & 0xFF)
            };

            sdlVertices.push_back({
                { (float)v.x + tx, (float)v.y + ty },
                sdlColor,
                { v.u, v.v }
            });
        }
    }

    SDL_RenderGeometry(mRenderer, sdlTex, sdlVertices.data(), (int)sdlVertices.size(), nullptr, 0);

    ClearClipRect();
    ResetTextureStates(sdlTex);
}

void SDL2RendererWrapper::BltMirror(Image* srcImage, int destX, int destY, const Rect& srcRect, const Color& color, int drawMode)
{
    SDL_Texture* texture = GetTextureForImage(srcImage);
    if (!texture) return;

    MemoryImage* memImage = dynamic_cast<MemoryImage*>(srcImage);
    if (memImage && memImage->mBitsChanged)
    {
        UpdateTextureFromBits(srcImage);
        memImage->mBitsChanged = false;
    }

    ApplyTextureStates(texture, color, drawMode);

    SDL_Rect sdlSrc = { srcRect.mX, srcRect.mY, srcRect.mWidth, srcRect.mHeight };
    SDL_Rect sdlDest = { destX, destY, srcRect.mWidth, srcRect.mHeight };

    SDL_RenderCopyEx(mRenderer, texture, &sdlSrc, &sdlDest, 0, NULL, SDL_FLIP_HORIZONTAL);
    ResetTextureStates(texture);
}

void SDL2RendererWrapper::StretchBltMirror(Image* srcImage, const Rect& destRect, const Rect& srcRect, const Rect& clipRect, const Color& color, int drawMode, bool fastStretch)
{
    SDL_Texture* texture = GetTextureForImage(srcImage);
    if (!texture) return;

    MemoryImage* memImage = dynamic_cast<MemoryImage*>(srcImage);
    if (memImage && memImage->mBitsChanged)
    {
        UpdateTextureFromBits(srcImage);
        memImage->mBitsChanged = false;
    }

    ApplyTextureStates(texture, color, drawMode);
    SetClipRect(clipRect);

    SDL_Rect sdlSrc = { srcRect.mX, srcRect.mY, srcRect.mWidth, srcRect.mHeight };
    SDL_Rect sdlDest = { destRect.mX, destRect.mY, destRect.mWidth, destRect.mHeight };

    SDL_RenderCopyEx(mRenderer, texture, &sdlSrc, &sdlDest, 0, NULL, SDL_FLIP_HORIZONTAL);
    
    ClearClipRect();
    ResetTextureStates(texture);
}

}
