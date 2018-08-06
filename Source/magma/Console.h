#pragma once

#include <string>

namespace magma {

class Backend;


class Console
{
public:
	void putc(int x, int y, int c);
	void sub_put(int pixelX, int pixelY, int c);
	void print(int x, int y, std::string str);
	void sub_print(int pixelX, int pixelY, std::string str);
	void set_fg(int r, int g, int b);
	void set_bg(int r, int g, int b);
private:
	Backend* con;
}

}
