/* Explicit panel mounting, independent of the game's drawing coordinates.
 * Zero rotation keeps the original path. Nonzero rotation uses one GPU target.
 * Angles use SDL_RenderCopyEx's clockwise convention. */
#ifndef GBADS_PANEL_ROTATION_H
#define GBADS_PANEL_ROTATION_H
#include <SDL.h>

typedef struct {
    SDL_Texture* target;
    int angle, w, h, pw, ph;
} PanelRotation;

static inline int panel_rotation_sync(PanelRotation* p, SDL_Renderer* ren) {
    if (p->angle == 0) return 0;
    if (p->angle != 90 && p->angle != 180 && p->angle != 270)
        return SDL_SetError("panel angle must be 0, 90, 180 or 270");
    SDL_Texture* previous = SDL_GetRenderTarget(ren);
    if (SDL_SetRenderTarget(ren, NULL) < 0) return -1;
    int pw, ph;
    if (SDL_GetRendererOutputSize(ren, &pw, &ph) < 0) return -1;
    if (p->target && pw == p->pw && ph == p->ph)
        return SDL_SetRenderTarget(ren, previous);
    SDL_Texture* target = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888,
                                            SDL_TEXTUREACCESS_TARGET,
                                            p->angle == 180 ? pw : ph,
                                            p->angle == 180 ? ph : pw);
    if (!target) { SDL_SetRenderTarget(ren, previous); return -1; }
    SDL_SetTextureBlendMode(target, SDL_BLENDMODE_NONE);
    if (p->target) SDL_DestroyTexture(p->target);
    p->target = target;
    p->pw = pw; p->ph = ph;
    p->w = p->angle == 180 ? pw : ph;
    p->h = p->angle == 180 ? ph : pw;
    return SDL_SetRenderTarget(ren, p->target);
}

static inline int panel_rotation_present(PanelRotation* p, SDL_Renderer* ren) {
    if (p->target) {
        if (SDL_SetRenderTarget(ren, NULL) < 0) return -1;
        SDL_Rect dst = { (p->pw - p->w) / 2, (p->ph - p->h) / 2, p->w, p->h };
        if (SDL_RenderCopyEx(ren, p->target, NULL, &dst, p->angle, NULL, SDL_FLIP_NONE) < 0) return -1;
    }
    SDL_RenderPresent(ren);
    return p->target ? SDL_SetRenderTarget(ren, p->target) : 0;
}

static inline void panel_rotation_pointer(const PanelRotation* p, int* x, int* y) {
    int px = *x, py = *y;
    if (p->angle == 90) { *x = py; *y = p->pw - 1 - px; }
    else if (p->angle == 180) { *x = p->pw - 1 - px; *y = p->ph - 1 - py; }
    else if (p->angle == 270) { *x = p->ph - 1 - py; *y = px; }
}
#endif
