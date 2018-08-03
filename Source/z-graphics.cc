#include "z-graphics.h"

//#include "stdbool.h"
#include "stdio.h"

#include <iostream>
#include <vector>
#include <cinttypes>

#include "mem.h"

#include "SDL.h"
#include "lodepng.h"

SDL_Surface* load_to_surface(std::string filename);
SDL_Texture* load_to_texture(SDL_Renderer* render, std::string filename);
std::vector<SDL_Rect> generate_font_clips(int tile_width, int tile_height);

#define MAX_LETTER_CODE 255

//void render_font(screen* scr);

//SDL_Window* __give_window(screen* scr)
//{
//	return scr->window;
//}
using std::cout;
//Set data 
Screen::Screen(int scr_width, int scr_height, int tile_w, int tile_h, std::string filename)
{
	mLog = spdlog::get("main");
	mWidth = scr_width * tile_w;
	mHeight = scr_height * tile_h;
	mTile_width = tile_w;
	mTile_height = tile_h;
	mFont_clips = generate_font_clips(tile_w, tile_h);
	mFontfile = filename;
	//render_font(scr);
	mInit = false;
}
//Initialize SDL Stuff
bool Screen::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		mLog->error("SDL could not initialize! SDL_Error: {}", SDL_GetError());
		return false;
	}
	else
	{
		//Create window
		mWindow = SDL_CreateWindow("rl", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWidth, mHeight, SDL_WINDOW_SHOWN);
		if (mWindow == NULL)
		{
			mLog->error("Window could not be created! SDL_Error: {}", SDL_GetError());
			return false;
		}
		else
		{
			//Get window surface
			mScreen = SDL_GetWindowSurface(mWindow);
		}
	}
	mRender = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	if (mRender == NULL)
	{
		mLog->error("Failed to create a renderer! SDL_Error: {}", SDL_GetError());
		return false;
	}
	else
	{
		SDL_SetRenderDrawColor(mRender, 0xFF, 0xFF, 0xFF, 0xFF);
	}
	SDL_SetRenderDrawBlendMode(mRender, SDL_BLENDMODE_BLEND);
	mFont = load_to_texture(mRender, mFontfile);
	if (mFont == NULL)
	{
		mLog->error("Failed to load texture: {} with error: {}", mFontfile, SDL_GetError());
		return false;
	}
	mInit = true;
	return true;
}

// Release the resources.
Screen::~Screen()
{
	SDL_DestroyTexture(mFont);
	SDL_DestroyRenderer(mRender);
	SDL_DestroyWindow(mWindow);
}


// Render a section of the font / texture.
void Screen::draw_clip(int x, int y, SDL_Rect* clip, SDL_Rect* renderQuad)
{
	if (!mInit)
	{
		mLog->error("Screen not initialized.");
		return;
	}
	SDL_RenderCopy(mRender, mFont, clip, renderQuad);
}

// Redraw the screen.
void Screen::redraw()
{
	if (!mInit)
	{
		mLog->error("Screen not initialized.");
		return;
	}
	SDL_RenderPresent(mRender);
}

// Draw a character to the screen.
void Screen::draw_char(int x, int y, int c)
{
	if (!mInit)
	{
		mLog->error("Screen not initialized.");
		return;
	}
	int i = x * mTile_width;
	int j = y * mTile_height;
	SDL_Rect renderQuad = { i, j, mTile_width, mTile_height };
	SDL_RenderFillRect(mRender, &renderQuad);
	//printf("Index / Char: %d / %c", c);
	//printf("Source: X: %d Y: %d X2: %d Y2: %d", scr->font_clips[c].x, scr->font_clips[c].y, scr->font_clips[c].x + scr->tile_width, scr->font_clips[c].y + scr->tile_height);
	//printf("Destination: X: %d Y: %d X2: %d Y2: %d", i, j, i + scr->tile_width, j + scr->tile_height);
	if (c > MAX_LETTER_CODE)
	{
		c = MAX_LETTER_CODE;
	}
	draw_clip(i, j, &mFont_clips[c], &renderQuad);
}

// Set the background colour.
void Screen::set_bg(int r, int g, int b)
{
	if (!mInit)
	{
		mLog->error("Screen not initialized.");
		return;
	}
	SDL_SetRenderDrawColor(mRender, r, g, b, 255);
}

// Set the foreground colour.
void Screen::set_fg(int r, int g, int b)
{
	if (!mInit)
	{
		mLog->error("Screen not initialized.");
		return;
	}
	SDL_SetTextureColorMod(mFont, r, g, b);
}

// Print a string to the screen.
void Screen::print(int x, int y, std::string str)
{
	if (!mInit)
	{
		mLog->error("Screen not initialized.");
		return;
	}
	for (int i = 0; i < str.length(); i++)
	{
		draw_char(x + i, y, str[i]);
	}
}

SDL_Window* Screen::give_window()
{
	return mWindow;
}

/*
* This only suports .png but why should it support more huh.
*/
SDL_Texture* load_to_texture(SDL_Renderer* render, std::string filename)
{
	SDL_Surface* surf = load_to_surface(filename);
	if (surf == NULL)
		return NULL;
	SDL_Texture* tex = SDL_CreateTextureFromSurface(render, surf);
	SDL_FreeSurface(surf);
	SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
	return tex;
}


/* Same as above tbh */
SDL_Surface* load_to_surface(std::string filename)
{
	auto log = spdlog::get("main");
	SDL_Surface* image;
	unsigned error;
	std::vector<unsigned char> imgBuf;
	unsigned w, h, x, y;
	error = lodepng::decode(imgBuf, w, h, filename);
	if (error)
	{
		log->error("Decoder error {}: {}", error, lodepng_error_text(error));
		return NULL;
	}
	//Transparent Colour
	uint8_t r_t, g_t, b_t, a_t;
	r_t = imgBuf[0];
	g_t = imgBuf[1];
	b_t = imgBuf[2];
	a_t = imgBuf[3];
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

std::vector<SDL_Rect> generate_font_clips(int tile_w,int tile_h)
{
	// Zero allocate an array of max letter code + 1 cause arrays start at 0
	std::vector<SDL_Rect> clips;
	clips.resize(MAX_LETTER_CODE + 1);
	// x = i, y = j
	// Loop over row then col
	// Font must be rows then cols
	int idx = 0;
	for (int j = 0; j < 16; j++)
	for (int i = 0; i < 16; i++)
	{
		int x = i * tile_w;
		int y = j * tile_h;
		//Create clips for individual letters. 
		SDL_Rect renderQuad = { x, y, tile_w, tile_h };
		clips[idx] = renderQuad;
		idx++;
	}
	return clips;
}

