#include "z-graphics.h"

#include "stdbool.h"
#include "stdio.h"

#include "mem.h"

#include "SDL.h"
#include "lodepng.h"
SDL_Surface* load_to_surface(const char* filename);
SDL_Texture* load_to_texture(screen* scr, const char* filename);
SDL_Rect* generate_font_clips(int tile_w, int tile_h);

static bool init = false;
typedef struct screen {
	unsigned long width;
	unsigned long height;
	unsigned int tile_width;
	unsigned int tile_height;
	SDL_Renderer* render;
	SDL_Surface* screen;
	SDL_Window* window;
	SDL_Texture* font;
	SDL_Rect* font_clips;
} screen;



screen* graphics_init(int scr_width, int scr_height, int tile_w, int tile_h, const char* filename)
{
	init = true;
	screen* scr = mem_alloc(sizeof(screen));
	scr->width = scr_width * tile_w;
	scr->height = scr_height * tile_h;
	scr->tile_width = tile_w;
	scr->tile_height = tile_h;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return NULL;
	}
	else
	{
		//Create window
		scr->window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, scr->width, scr->height, SDL_WINDOW_SHOWN);
		if (scr->window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return NULL;
		}
		else
		{
			//Get window surface
			scr->screen = SDL_GetWindowSurface(scr->window);
		}
	}
	scr->render = SDL_CreateRenderer(scr->window, -1, SDL_RENDERER_ACCELERATED);
	if (scr->render == NULL)
	{
		printf("Failed to create a renderer: %s\n", SDL_GetError());
		return NULL;
	}
	else
	{
		SDL_SetRenderDrawColor(scr->render, 0xFF, 0xFF, 0xFF, 0xFF);
	}
	SDL_SetRenderDrawBlendMode(scr->render, SDL_BLENDMODE_BLEND);
	scr->font = load_to_texture(scr, filename);
	if (scr->font == NULL)
	{
		printf("Failed to load texture: %s with error: %s\n", filename, SDL_GetError());
		return NULL;
	}
	scr->font_clips = generate_font_clips(tile_w, tile_h);
	return scr;
}

void destroy_graphics(screen* scr)
{
	SDL_DestroyTexture(scr->font);
	SDL_DestroyRenderer(scr->render);
	SDL_DestroyWindow(scr->window);
	mem_free(scr->font_clips);
	mem_free(scr);
	SDL_Quit();
}



/*
 * This only suports .png but why should it support more huh.
 */
SDL_Texture* load_to_texture(screen* scr, const char* filename)
{
	SDL_Surface* surf = load_to_surface(filename);
	if (surf == NULL)
		return NULL;
	SDL_Texture* tex = SDL_CreateTextureFromSurface(scr->render, surf);
	SDL_FreeSurface(surf);
	SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
	return tex;
}

/* Same as above tbh */
SDL_Surface* load_to_surface(const char* filename)
{
	SDL_Surface* image;
	unsigned error;
	unsigned char* imgBuf;
	unsigned w, h, x, y;
	error = lodepng_decode32_file(&imgBuf, &w, &h, filename);
	if (error)
	{
		printf("decoder error %u: %s\n", error, lodepng_error_text(error));
		return NULL;
	}
	//Transparent Colour
	uint8_t r_t, g_t, b_t, a_t;
	r_t = imgBuf[0];
	g_t = imgBuf[1];
	b_t = imgBuf[2];
	a_t = imgBuf[3];
	Uint32 rmask, gmask, bmask, amask;
	image = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA32);
	if (image == NULL)
		return NULL;
	for (y = 0; y < h; y++)
		for (x = 0; x < w; x++)
		{
			int checkerColor;
			uint32_t* bufp;
			uint32_t r, g, b, a;
			/* get RGBA components */
			r = imgBuf[4 * y * w + 4 * x + 0]; // Red
			g = imgBuf[4 * y * w + 4 * x + 1]; // Green
			b = imgBuf[4 * y * w + 4 * x + 2]; // Blue
			a = imgBuf[4 * y * w + 4 * x + 3]; // Alpha
			if (r == r_t && g == g_t && b == b_t && a == a_t)
				a = 0;
											   /*make translucency visible by placing checkerboard pattern behind image*/
			//checkerColor = 191 + 64 * (((x / 16) % 2) == ((y / 16) % 2));
			checkerColor = 1;
			//r = (a * r + (255 - a) * checkerColor) / 255;
			//g = (a * g + (255 - a) * checkerColor) / 255;
			//b = (a * b + (255 - a) * checkerColor) / 255;
			/* Assign the color to the surface */
			bufp = (Uint32 *)image->pixels + (y * image->pitch / 4) + x;
			*bufp = SDL_MapRGBA(image->format, r, g, b, a);
			
		}
	return image;
}


SDL_Rect* generate_font_clips(int tile_w,int tile_h)
{
	SDL_Rect* clips = mem_zalloc(256 * sizeof(SDL_Rect));
	// x = i, y = j
	// Loop over row then col
	int idx = 0;
	for (int j = 0; j < 16; j++)
	for (int i = 0; i < 16; i++)
	{
		int x = i * tile_w;
		int y = j * tile_h;
		SDL_Rect renderQuad = { x, y, tile_w, tile_h };
		clips[idx] = renderQuad;
		idx++;
	}
	return clips;
}

void draw_clip(screen* scr, int x, int y, SDL_Rect* clip, SDL_Rect* renderQuad)
{
	SDL_RenderCopy(scr->render, scr->font, clip, renderQuad);
}

void redraw(screen* scr)
{
	SDL_RenderPresent(scr->render);
}


SDL_Rect get_clip_for_letter(screen* scr, char c)
{
	return scr->font_clips[c];
}

void draw_char(screen* scr, int x, int y, char c)
{
	int i = x * scr->tile_width;
	int j = y * scr->tile_height;
	SDL_Rect renderQuad = { i, j, scr->tile_width, scr->tile_height };
	SDL_RenderFillRect(scr->render, &renderQuad);
	draw_clip(scr, i, j,&scr->font_clips[c], &renderQuad);
}

void set_bg(screen* scr, int r, int g, int b)
{
	SDL_SetRenderDrawColor(scr->render, r, g, b, 255);
}

void set_fg(screen* scr, int r, int g, int b)
{
	SDL_SetTextureColorMod(scr->font, r, g, b);
}

void print(screen* scr, int x, int y, const char* str)
{
	for (int i = 0; i < strlen(str); i++)
	{
		draw_char(scr, x + i, y, str[i]);
	}
}