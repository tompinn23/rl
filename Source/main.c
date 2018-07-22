/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

#include <stdio.h>
#include <stdbool.h>

//Using SDL and standard IO
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "lodepng.h"
#include "z-graphics.h"



int main(int argc, char* argv[])
{
	screen* scr = graphics_init(40, 40, 10, 10, "Taffer_10x10.png");
	if (scr == NULL)
	{
		printf("Failed to initialize graphics.");
		return -1;
	}
	set_fg(scr, 25, 200, 130);
	set_bg(scr, 255, 0, 0);
	draw_char(scr, 0, 0, 'H');
	draw_char(scr, 1, 0, 'E');
	draw_char(scr, 2, 0, 'L');
	draw_char(scr, 3, 0, 'L');
	draw_char(scr, 4, 0, 'O');
	set_fg(scr, 0, 0, 0);
	set_bg(scr, 255, 255, 255);
	print(scr, 2, 2, "Helps Me!");
	redraw(scr);
	SDL_Delay(2000);
	//SDL_Delay(12000);
	//Destroy window

	//Quit SDL subsystems
	destroy_graphics(scr);

	return 0;
}
