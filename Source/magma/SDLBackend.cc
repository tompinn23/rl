#include "SDLBackend.h"

#include "fmt/format.h"
#include "lodepng.h"


namespace magma {
	SDLRootBackend::SDLRootBackend(int width, int height, std::string font, int tile_size)
	{
		mLog = spdlog::get("main");
		mWidth = width;
		mHeight = height;
		mFontFile = font;
		mTileSize = tile_size;
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			throw std::runtime_error(fmt::format("SDL Couldnt initialize: {}", SDL_GetError()));
		}
		mWindow = SDL_CreateWindow("rl", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWidth * tile_size, mHeight * tile_size, SDL_WINDOW_SHOWN);
		if(mWindow == NULL)
		{
			throw std::runtime_error(fmt::format("SDL Window creation failed with error: {} ", SDL_GetError()));
		}
		mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
		if(mRenderer == NULL)
		{
			throw std::runtime_error(fmt::format("SDL Renderer creation failed with error: {}", SDL_GetError()));
		}
		SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
		SDL_Surface* image;
		unsigned error;
		std::vector<unsigned char> imgBuf;
		unsigned w, h, x, y;
		error = lodepng::decode(imgBuf, w, h, mFontFile);
		if (error)
		{
			throw std::runtime_error(fmt::format("Decoding errror: {} ", lodepng_error_text(error)));
		}
		//Transparent Colour
		uint8_t r_t, g_t, b_t, a_t;
		r_t = imgBuf[0];
		g_t = imgBuf[1];
		b_t = imgBuf[2];
		a_t = imgBuf[3];
		image = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA32);
		if (image == NULL)
		{
			throw std::runtime_error(fmt::format("Failed to create SDL surface: {}", SDL_GetError()));
		}
		for (y = 0; y < h; y++)
			for (x = 0; x < w; x++)
			{
				uint32_t* bufp;
				uint32_t r, g, b, a;
				/* get RGBA components */
				r = imgBuf[4 * y * w + 4 * x + 0]; // Red
				g = imgBuf[4 * y * w + 4 * x + 1]; // Green
				b = imgBuf[4 * y * w + 4 * x + 2]; // Blue
				a = imgBuf[4 * y * w + 4 * x + 3]; // Alpha
				/* If color == background then set as transparent. */
				if (r == r_t && g == g_t && b == b_t && a == a_t)
					a = 0;
				bufp = (Uint32 *)image->pixels + (y * image->pitch / 4) + x;
				*bufp = SDL_MapRGBA(image->format, r, g, b, a);
			}
		mFont = SDL_CreateTextureFromSurface(mRenderer, image);
		SDL_SetTextureBlendMode(mFont, SDL_BLENDMODE_BLEND);
		mFontClips->resize(MAX_LETTER_CODE + 1);
		int idx = 0;
		for(int j = 0; j < (w / tile_size); j++)
		for (int i = 0; i < (h / tile_size); i++)
		{
			int x = i * tile_size;
			int y = j * tile_size;
			SDL_Rect renderQuad = { x, y, tile_size, tile_size };
			(*mFontClips)[idx] = renderQuad;
			idx++;
		}
	}

	SDLRootBackend::~SDLRootBackend()
	{
		SDL_DestroyTexture(mFont);
		SDL_DestroyRenderer(mRenderer);
		SDL_DestroyWindow(mWindow);
	}

	SDLBackend::SDLBackend(int width, int height, SDLRootBackend* rootWindow)
	{
		mWidth = width;
		mHeight = height;
		mRenderer = rootWindow->mRenderer;
		mTileSize = rootWindow->mTileSize;
		mConsole = SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, mWidth * mTileSize, mHeight * mTileSize);
		if (mConsole == NULL)
		{
			throw std::runtime_error(fmt::format("Console texture could not be created: {}", SDL_GetError()));
		}
	}

	void SDLRootBackend::putc(int x, int y, int c)
	{
		int i = x * mTileSize;
		int j = y * mTileSize;
		SDL_Rect renderQuad = { i, j, mTileSize, mTileSize };
		SDL_RenderFillRect(mRenderer, &renderQuad);
		//printf("Index / Char: %d / %c", c);
		//printf("Source: X: %d Y: %d X2: %d Y2: %d", scr->font_clips[c].x, scr->font_clips[c].y, scr->font_clips[c].x + scr->tile_width, scr->font_clips[c].y + scr->tile_height);
		//printf("Destination: X: %d Y: %d X2: %d Y2: %d", i, j, i + scr->tile_width, j + scr->tile_height);
		if (c > MAX_LETTER_CODE)
		{
			c = MAX_LETTER_CODE;
		}
		SDL_RenderCopy(mRenderer, mFont, &(*mFontClips)[c], &renderQuad);
	}

	void sub_putc(int pixelX, int pixelY, int c)
	{
	}

	void SDLRootBackend::print(int x, int y, std::string str)
	{
		for (int i = 0; i < str.length(); i++)
		{
			putc(x + i, y, str[i]);
		}
	}

	void sub_print(int pixelX, int pixelY, int c)
	{
	}

	void SDLRootBackend::set_bg(int r, int g, int b)
	{
		SDL_SetRenderDrawColor(mRenderer, r, g, b, 255);
	}

	void SDLRootBackend::set_fg(int r, int g, int b)
	{
		SDL_SetTextureColorMod(mFont, r, g, b);
	}

	void SDLRootBackend::refresh()
	{
		SDL_RenderPresent(mRenderer);
	}
}
