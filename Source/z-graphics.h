#pragma once

#include <string>
#include <vector>

#include "SDL.h"
#include "spdlog/spdlog.h"

class Screen {
public:
	Screen(int scr_width, int scr_height, int tile_w, int tile_h, std::string filename);
	bool init();
	~Screen();
	void draw_clip(int x, int y, SDL_Rect* clip, SDL_Rect* renderQuad);
	void draw_char(int x, int y, int c);
	void redraw();
	void set_bg(int r, int g, int b);
	void set_fg(int r, int g, int b); 
	void print(int x, int y, std::string str);
	SDL_Window* give_window();
private:
	unsigned long mWidth;
	unsigned long mHeight;
	int mTile_width;
	int mTile_height;
	SDL_Renderer* mRender;
	SDL_Surface* mScreen;
	SDL_Window* mWindow;
	std::string mFontfile;
	SDL_Texture* mFont;
	std::vector<SDL_Rect> mFont_clips;
	//size_t clip_length;
	bool mInit;
	std::shared_ptr<spdlog::logger> mLog;
};
