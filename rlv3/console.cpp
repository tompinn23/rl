#include "console.hpp"

#include "BearLibTerminal.h"
#include "spdlog/spdlog.h"

namespace terminal {

char_t::char_t(char a_character, color_t a_bk_color, color_t a_fg_color)
{
	character = a_character;
	bk_color = a_bk_color;
	fg_color = a_fg_color;
}

char_t::char_t()
{
	character = ' ';
	bk_color = 0xFF000000;
	fg_color = 0xFFFFFFFF;
}

Console::Console()
{
	init(80, 50, 0xF000000, 0xFFFFFFFF);
}

Console::Console(int width, int height) 
{
	init(width, height, 0xFF000000, 0xFFFFFFFF);
}

Console::Console(int width, int height, color_t bk)
{
	init(width, height, bk, 0xFFFFFFFF);
}

Console::Console(int width, int height, color_t bk, color_t fg)
{
	init(width, height, bk, fg);
}

Console::~Console()
{
}

void Console::init(int width, int height, color_t bk, color_t fg)
{
	m_width = width;
	m_height = height;
	m_bk = bk;
	m_fg = fg;
	m_console.resize(m_width, std::vector<char_t>(m_height, char_t(' ', m_bk, m_fg)));	
}

void Console::blit(int destX, int destY, int destX2, int destY2)
{
	auto log = spdlog::get("main");
	if (destX2 - destX < 0 || destY2 - destY < 0)
	{
		log->error("The second coordinate can't be less than the first.");
		throw std::logic_error("The second coordinate can not be less than the first.");
	}
	if (destX2 >= m_width)
		destX2 = m_width - 1;
	if (destY2 >= m_height)
		destY2 = m_height - 1;
	int x = 0;
	for(int i = destX; i <= destX2; i++)
	{
		int y = 0;
		for(int j = destY; j <= destY2; j++)
		{
			char_t c = at(x, y);
			log->info("Character at {},{} is {}", x, y, c.character);
			terminal_bkcolor(c.bk_color);
			terminal_color(c.fg_color);
			terminal_put(i, j, c.character);
			y++;
		}
		x++;
	}
	terminal_refresh();
}

 char_t Console::at(int x, int y)
{
	if(x < 0 || y < 0)
		return char_t(' ', 0xFF000000, 0xFFFFFFFF);
	if(x >= m_width || y >= m_height)
		return char_t(' ', 0xFF000000, 0xFFFFFFFF);
	return m_console[x][y];
}

bool Console::set(int x, int y, char_t val)
{
	if(x < 0 || y < 0)
		return false;
	if(x >= m_width || y >= m_height)
		return false;
	m_console[x][y] = val;
	return true;
}

void Console::put(int x, int y, char val)
{
	char_t c = char_t(val, m_bk, m_fg);
	set(x, y, c);
}

void Console::put(int x, int y, char val, color_t bk, color_t fg)
{
	char_t c = char_t(val, bk, fg);
	set(x, y, c);
}

void Console::put(int x, int y, char val, color_t fg)
{
	char_t c = char_t(val, m_bk, fg);
	set(x, y, c);
}

void Console::print(int x, int y, std::string val)
{
	int i = 0;
	for (auto c : val)
	{
		put(x + i, y, c);
		i++;
	}
}

void Console::bkcolor(color_t bk)
{
	m_bk = bk;
}

	void Console::color(color_t fg)
	{
		m_fg = fg;
	}

	char_t Console::get(int x, int y)
	{
		return at(x, y);
	}


	void open()
	{
		terminal_open();
	}

	int set(std::string val)
	{
		return terminal_set(val.c_str());
	}

	void refresh()
	{
		terminal_refresh();
	}

	color_t get_color(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
	{
		return color_from_argb(a, r, g, b);
	}

	void put(int x, int y, char val)
	{
		terminal_put(x, y, val);
	}

	void print(int x, int y, const std::string& val)
	{
		terminal_print(x, y, val.c_str());
	}

	int has_input()
	{
		return terminal_has_input();
	}

	int read()
	{
		return terminal_read();
	}
	color_t read_str(int x, int y, std::string &buffer, int max)
	{
		auto buf = new char[max];
		color_t size = terminal_read_str(x, y, buf, max);
		buffer = std::string(buf);
		delete[] buf;
		return size;
	}

	int state(int slot)
	{
		return terminal_state(slot);
	}

	void delay(int period)
	{
		terminal_delay(period);
	}
}

