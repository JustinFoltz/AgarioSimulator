/// @file gfx.c
/// @author Florent Gluck
/// @date November 6, 2016
/// Helper routines to render pixels in fullscreen graphic mode.
/// Uses the SDL2 library.

#include "gfx.h"
#include "../safe_thread.h"

/// Create a fullscreen graphic window.
/// @param title Title of the window.
/// @param width Width of the window in pixels.
/// @param height Height of the window in pixels.
/// @return a pointer to the graphic context or NULL if it failed.
struct gfx_context_t* gfx_create(char *title, uint width, uint height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return NULL;//goto error;

    // You can replace SDL_WINDOW_FULLSCREEN_DESKTOP by SDL_WINDOW_OPENGL if you 
    // don't want a full screen render.
    SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING, width, height);
    uint32_t *pixels = safe_malloc(width*height*sizeof(uint32_t));
    struct gfx_context_t *ctxt = safe_malloc(sizeof(struct gfx_context_t));

    if (!window || !renderer || !texture || !pixels || !ctxt) goto error;

    ctxt->renderer = renderer;
    ctxt->texture = texture;
    ctxt->window = window;
    ctxt->width = width;
    ctxt->height = height;
    ctxt->pixels = pixels;

    //SDL_ShowCursor(SDL_DISABLE);
    gfx_clear(ctxt, COLOR_BLACK);
    return ctxt;

error:
    return NULL;
}


/// Destroy a graphic window.
/// @param ctxt Graphic context of the window to close.
void gfx_destroy(struct gfx_context_t *ctxt) {
    SDL_ShowCursor(SDL_ENABLE);
    SDL_DestroyTexture(ctxt->texture);
    SDL_DestroyRenderer(ctxt->renderer);
    SDL_DestroyWindow(ctxt->window);
    free(ctxt->pixels);
    ctxt->texture = NULL;
    ctxt->renderer = NULL;
    ctxt->window = NULL;
    ctxt->pixels = NULL;
    free(ctxt);
    SDL_Quit();
    
}


/// Draw a pixel in the specified graphic context.
/// @param ctxt Graphic context where the pixel is to be drawn.
/// @param x X coordinate of the pixel.
/// @param y Y coordinate of the pixel.
/// @param color Color of the pixel.
void gfx_putpixel(struct gfx_context_t *ctxt, int x, int y, uint32_t color) {
    if (x < ctxt->width && y < ctxt->height && x >= 0 && y >= 0)
        ctxt->pixels[ctxt->width*y+x] = color;
}

/// Clear the specified graphic context.
/// @param ctxt Graphic context to clear.
/// @param color Color to use.
void gfx_clear(struct gfx_context_t *ctxt, uint32_t color) {
    int n = ctxt->width*ctxt->height;
    while (n)
        ctxt->pixels[--n] = color;
}

/// Display the graphic context.
/// @param ctxt Graphic context to clear.
void gfx_present(struct gfx_context_t *ctxt) {
    SDL_UpdateTexture(ctxt->texture, NULL, ctxt->pixels, ctxt->width*sizeof(uint32_t));
    SDL_RenderCopy(ctxt->renderer, ctxt->texture, NULL, NULL);
    SDL_RenderPresent(ctxt->renderer);
}


/// If a key was pressed, returns its key code (non blocking call).
/// List of key codes: https://wiki.libsdl.org/SDL_Keycode
/// @return the key that was pressed or 0 if none was pressed.
SDL_Keycode gfx_keypressed() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN)
            return event.key.keysym.sym;
    }
    return 0;
}

/**
 * @brief 
 * 
 * @param context 
 * @param cells 
 * @param nb_cells 
 * @param foods 
 * @param nb_y 
 */
void render(struct gfx_context_t *context, cell_t **cells, int nb_cells, food_t **foods, int nb_foods) {
    gfx_clear(context, COLOR_BLACK);

    for(int i=0; i<nb_foods;i++) { 
        if(foods[i]->state == 0) {
            gfx_drawcircle_food(context, foods[i]->x_position, foods[i]->y_position, 1, COLOR_GREEN);
        }
    } 

    for(int i=0; i<nb_cells; i++) {
        if(cells[i]->alive) {
            uint32_t color = MAKE_COLOR(cells[i]->color.red, cells[i]->color.green, cells[i]->color.blue);
            gfx_drawcircle(context, cells[i]->x, cells[i]->y, calculate_radius(cells[i]), color);
        }
    }

}

/// Draw filled circle in the specified context, center x, center y, radius and color.
/// @param ctxt Graphic context where the circle is to be drawn.
/// @param xc Center x coordinate of the circle.
/// @param yc Center y coordinate of the circle.
/// @param r Radius of the circle.
/// @param color Color of the circle.
void gfx_drawcircle(struct gfx_context_t *ctxt, int xc, int yc, int r, uint32_t color) {
    int x_min = xc-r;
    int x_max = xc+r+1; 
    int y_min = yc-r;
    int y_max = yc+r+1;
    for(int i=x_min; i<x_max; i++) {
        for(int j=y_min; j<y_max; j++) {
            double distance = calculate_distance_beetwen_points(xc, yc, i, j);
            if((int)(distance)==(int)r) {
                 gfx_putpixel(ctxt, i, j, COLOR_WHITE);
            }
            if(distance<r) {
                gfx_putpixel(ctxt, i, j, color);
            }
        }
    }
}

/// Draw filled circle in the specified context, center x, center y, radius and color.
/// @param ctxt Graphic context where the circle is to be drawn.
/// @param xc Center x coordinate of the circle.
/// @param yc Center y coordinate of the circle.
/// @param r Radius of the circle.
/// @param color Color of the circle.
void gfx_drawcircle_food(struct gfx_context_t *ctxt, int xc, int yc, int r, uint32_t color) {
    int x_min = xc-r;
    int x_max = xc+r+1; 
    int y_min = yc-r;
    int y_max = yc+r+1;
    for(int i=x_min; i<x_max; i++) {
        for(int j=y_min; j<y_max; j++) {
            double distance = calculate_distance_beetwen_points(xc, yc, i, j);
            if(distance<r) {
                gfx_putpixel(ctxt, i, j, color);
            }
        }
    }
}