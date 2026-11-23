/* Render every logical pixel through each rotation and check the physical
 * pixels plus inverse pointer mapping. No emulator core or display needed. */
#include "panel_rotation.h"
#include <assert.h>
#include <stdio.h>
int main(void) {
    assert(SDL_Init(0) == 0);
    for (int angle = 0; angle < 360; angle += 90) {
        SDL_Surface* out = SDL_CreateRGBSurfaceWithFormat(0, 8, 6, 32, SDL_PIXELFORMAT_ARGB8888);
        SDL_Renderer* ren = SDL_CreateSoftwareRenderer(out);
        assert(ren);
        PanelRotation p = { .angle = angle };
        assert(panel_rotation_sync(&p, ren) == 0);
        int w = p.target ? p.w : out->w, h = p.target ? p.h : out->h;
        for (int y = 0; y < h; ++y) for (int x = 0; x < w; ++x) {
            SDL_SetRenderDrawColor(ren, x * 20, y * 20, 31, 255);
            assert(SDL_RenderDrawPoint(ren, x, y) == 0);
        }
        assert(panel_rotation_present(&p, ren) == 0);
        assert(SDL_GetRenderTarget(ren) == p.target);
        for (int y = 0; y < out->h; ++y) for (int x = 0; x < out->w; ++x) {
            int lx = x, ly = y;
            panel_rotation_pointer(&p, &lx, &ly);
            Uint8 r, g, b, a;
            SDL_GetRGBA(((Uint32*)((char*)out->pixels + y * out->pitch))[x], out->format, &r, &g, &b, &a);
            assert(r == lx * 20 && g == ly * 20 && b == 31 && a == 255);
        }
        SDL_Texture* old = p.target;
        assert(panel_rotation_sync(&p, ren) == 0 && old == p.target);
        if (p.target) SDL_DestroyTexture(p.target);
        SDL_DestroyRenderer(ren);
        SDL_FreeSurface(out);
    }
    SDL_Surface* out = SDL_CreateRGBSurfaceWithFormat(0, 8, 6, 32, SDL_PIXELFORMAT_ARGB8888);
    SDL_Renderer* ren = SDL_CreateSoftwareRenderer(out);
    PanelRotation invalid = { .angle = 45 };
    assert(panel_rotation_sync(&invalid, ren) < 0);
    SDL_DestroyRenderer(ren); SDL_FreeSurface(out);
    SDL_Quit();
    puts("panel rotation pixels and pointer mapping: passed (0/90/180/270)");
    return 0;
}
