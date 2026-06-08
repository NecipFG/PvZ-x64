#ifndef __SDL2RENDERERWRAPPER_H__
#define __SDL2RENDERERWRAPPER_H__

#include "Common.h"
#include "Color.h"
#include "Rect.h"
#include "SexyMatrix.h"
#include "TriVertex.h"
#include <SDL2/SDL.h>
#include <unordered_map>

namespace Sexy
{
class Image;
class MemoryImage;

class SDL2RendererWrapper
{
private:
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    struct TextureCacheEntry
    {
        SDL_Texture* mTexture;
        int mBitsChangedCount;
    };
    std::unordered_map<Image*, TextureCacheEntry> mTextureCache;
    int mWidth;
    int mHeight;
    bool mIsWindowed;
    int mTargetFPS;

    // Helper to apply drawing modes (normal, additive) and color tint
    void ApplyTextureStates(SDL_Texture* texture, const Color& color, int drawMode);
    void ResetTextureStates(SDL_Texture* texture);

public:
    SDL2RendererWrapper();
    ~SDL2RendererWrapper();

    bool Init(HWND hWnd, int width, int height, bool windowed);
    void Cleanup();

    SDL_Window* GetWindow() const { return mWindow; }
    SDL_Renderer* GetRenderer() const { return mRenderer; }
    int GetTargetFPS() const { return mTargetFPS; }

    // Texture Cache Management
    SDL_Texture* GetTextureForImage(Image* image, bool isRenderTarget = false);
    void DestroyTextureForImage(Image* image);
    void UpdateTextureFromBits(Image* image);

    // Frame Lifecycle
    void BeginFrame();
    void Present();
    void SetClipRect(const Rect& clipRect);
    void ClearClipRect();

    // Render Target Control
    void SetRenderTarget(Image* destImage);

    // Drawing Callback Interceptors
    void FillRect(const Rect& rect, const Color& color, int drawMode);
    void DrawLine(double startX, double startY, double endX, double endY, const Color& color, int drawMode);
    
    void Blt(Image* srcImage, int destX, int destY, const Rect& srcRect, const Color& color, int drawMode);
    void StretchBlt(Image* srcImage, const Rect& destRect, const Rect& srcRect, const Rect& clipRect, const Color& color, int drawMode, bool fastStretch);
    void BltRotated(Image* srcImage, float destX, float destY, const Rect& srcRect, const Rect& clipRect, const Color& color, int drawMode, double rot, float rotCenterX, float rotCenterY);
    void BltMatrix(Image* srcImage, float x, float y, const SexyMatrix3& matrix, const Rect& clipRect, const Color& color, int drawMode, const Rect& srcRect, bool blend);
    void BltTrianglesTex(Image* texture, const TriVertex vertices[][3], int numTriangles, const Rect& clipRect, const Color& color, int drawMode, float tx, float ty, bool blend);
    void BltMirror(Image* srcImage, int destX, int destY, const Rect& srcRect, const Color& color, int drawMode);
    void StretchBltMirror(Image* srcImage, const Rect& destRect, const Rect& srcRect, const Rect& clipRect, const Color& color, int drawMode, bool fastStretch);
};

extern SDL2RendererWrapper* gSDL2Renderer;

}

#endif // __SDL2RENDERERWRAPPER_H__
