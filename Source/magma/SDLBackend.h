#pragma once

#include "Backend.h"

#include <iostream>
#include <vector>
#include <cinttypes>
#include <stdexcept>

#include "SDL.h"

#define MAX_LETTER_CODE 255

namespace magma {
	class SDLBackend : public Backend {
	public:
		static SDLBackend* root;
		SDLBackend(int width, int height, std::string font, int tile_size);
		~SDLBackend();
		int get_width() { return mWidth; }
		int get_height() { return mHeight; }
		void putc(int x, int y, int c);
		void print(int x, int y, std::string str);
		void set_fg(int r, int g, int b);
		void set_bg(int r, int g, int b);
		void refresh();
	protected:
		std::string mFontFile = "";
		int mTileSize = 0;
		std::shared_ptr<std::vector<SDL_Rect>> mFontClips;
		SDL_Window* mWindow = NULL;
		SDL_Renderer* mRenderer = NULL;
		SDL_Texture* mFont = NULL;
		friend class SDLBackend;
	};
}
