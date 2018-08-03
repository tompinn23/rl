#include "terminal.h"

#include "SDL.h"
#include "z-graphics.h"

Terminal::Terminal(int width, int height)
{
	mWidth = width;
	mHeight = height;
}

bool Terminal::init(Screen& screen)
{
	
	mRenderer = SDL_CreateRenderer(screen.give_window();)
}