#include "terminal.h"

#include "z-graphics.h"

static screen* term = NULL;

int terminal_init(int width, int height, int tile_width, int tile_height, const char* fontfile)
{
	if (term != NULL)
	{
		printf("Already initialized terminal.\n");
		return 1;
	}
	term = graphics_init(width, height, tile_width, tile_height, fontfile);
	if (term == NULL)
	{
		printf("Failed to initialize terminal.\n");
		return 0;
	}
	return 1;
}

void terminal_destroy()
{
	destroy_graphics(term);
}

void terminal_put(int x, int y, char c)
{
	draw_char(term, x, y, c);
}

void terminal_print(int x, int y, const char* str)
{
	print(term, x, y, str);
}

void terminal_render()
{
	redraw(term);
}

void terminal_setfg(int r, int g, int b)
{
	set_fg(term, r, g, b);
}

void terminal_setbg(int r, int g, int b)
{
	set_bg(term, r, g, b);
}

/*
terminal_blit(console* con, int srcx, int srcy, int srcw, int srch, int x, int y)
{
}
*/