#ifndef GAME_H
#define GAME_H
#include "top-level.hpp"
class Receiver {
public:
	Receiver();
	virtual void input(int key) = 0;
};

class TextBox : Receiver {
public:
	TextBox();
	void input(int key);
	//TODO: Implement display events?
	//void display(Console &cur);
};


class Game {
public:
	Game();
	void add_reciever(Receiver &recv);
private:
	std::vector<Receiver&> receivers;
};
#endif