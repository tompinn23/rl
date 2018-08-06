#pragma once

#include "spdlog/spdlog.h"

namespace magma {
	class Backend {
		public:
			Backend();
			Backend(int width, int height, std::string font, int tile_size, bool rootWindow);
			virtual	void putc(int x, int y, int c) = 0;
			virtual void sub_putc(int pixelX, int pixelY, int c) = 0;
			virtual void print(int x, int y, int c) = 0;
			virtual void sub_print(int pixelX, int pixelY, int c) = 0;
			virtual void set_fg(int r, int g, int b) = 0;
			virtual void set_bg(int r, int g, int b) = 0;
			virtual void refresh() = 0;
		protected:
			int mWidth;
			int mHeight;
			std::shared_ptr<spdlog::logger> mLog;
	};
}
