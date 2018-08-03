#pragma once

class Screen;

class Terminal {
public:
	Terminal(int width, int height);
	bool init(Screen& screen);
private:
	int mWidth;
	int mHeight;
	SDL_Texture* mTexture;
};