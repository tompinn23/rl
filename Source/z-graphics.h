#pragma once
#include "stdbool.h"
#include "SDL.h"

typedef struct screen screen;
screen* graphics_init(int scr_width, int scr_height, int tile_w, int tile_h, const char* filename);
void destroy_graphics(screen* scr);
void draw_clip(screen* scr, int x, int y, SDL_Rect* clip, SDL_Rect* renderQuad);
void draw_char(screen* scr, int x, int y, char c);
void redraw(screen* scr);
void set_bg(screen* scr, int r, int g, int b);
void set_fg(screen* scr, int r, int g, int b);
void print(screen* scr, int x, int y, const char* str); 
SDL_Rect get_clip_for_letter(screen* scr, char c);