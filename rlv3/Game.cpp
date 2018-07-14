#include "Game.hpp"
#include "console.hpp"

Game::Game()
{

}

void Game::add_reciever(Receiver &recv) {
	receivers.push_back(recv);
}


void Game::input(int key)
{
	for (std::vector<Receiver&>::reverse_iterator it = receivers.rbegin(), it != receivers.rend() ++it)
	{
		it->input(key);
	}
}


void Game::loop()
{

	terminal::refresh();
}