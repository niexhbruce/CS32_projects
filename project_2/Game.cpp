// Game.cpp



#include "provided.h"
#include <iostream>
using namespace std;

int getConnectNumber(const Scaffold& scaff, int column, int N, int color);

class GameImpl
{
  public:
    GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black); 
    bool completed(int& winner) const; 
    bool takeTurn(); 
    void play(); 
    int checkerAt(int c, int r) const;
  private :
    Player* m_red, *m_black;
    Scaffold m_sc; // a scaffold private variable
    int m_count;   // m_count is counting the number of empty position like in scaffold
    int m_winner;
	int m_turn;
	int m_N;
};



GameImpl::GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black)
:m_red(red), m_black(black), m_sc(nColumns, nLevels), m_count(nColumns * nLevels), m_winner(-2), m_turn(RED), m_N(N)

{
}

bool GameImpl::completed(int& winner) const
{
	winner = m_winner;
    if(m_winner == -2)
		return false;
	else
		return true;  //  This is not always correct; it's just here to compile
}

bool GameImpl::takeTurn()
{
	int winner;
	if(completed(winner))
		return false;
	else
	{
		if(m_turn == RED)
		{
			int choose_col = m_red->chooseMove(m_sc, m_N, RED);
			m_sc.makeMove(choose_col, RED);
			int c_N = getConnectNumber(m_sc, choose_col, m_N, RED);
			if(c_N == m_N)
				m_winner = RED;
			else if(m_sc.numberEmpty() == 0)
				m_winner = VACANT;
			m_turn = BLACK;
		}
		else
		{
			int choose_col = m_black->chooseMove(m_sc, m_N, BLACK);
			m_sc.makeMove(choose_col, BLACK);
			int c_N = getConnectNumber(m_sc, choose_col, m_N, BLACK);
			if(c_N == m_N)
				m_winner = BLACK;
			else if(m_sc.numberEmpty() == 0)
				m_winner = VACANT;
			m_turn = RED;
		}
	}

}

void GameImpl::play()
{
	int winner;
	bool redTurn = true;
	while(!completed(winner))
	{
		if(redTurn)
			cout << "press any key to put red checker.\n";
		else
			cout << "press any key to put black checker.\n";
		redTurn = !redTurn;
		cin.ignore();
		takeTurn();
		m_sc.display();
	}
	if(winner == RED)
		cout << "red win!\n";
	else if(winner == BLACK)
		cout << "black win!\n";
	else if(winner == TIE_GAME)
		cout << "tie game!\n";
	else
		cout << "error: " << winner << endl;
    
}

int GameImpl::checkerAt(int c, int r) const
{
	return m_sc.checkerAt(c, r);  //  This is not always correct; it's just here to compile
}








//******************** Game functions *******************************

// These functions simply delegate to GameImpl's functions.
// You probably don't want to change any of this code.

Game::Game(int nColumns, int nLevels, int N, Player* red, Player* black)
{
    m_impl = new GameImpl(nColumns, nLevels, N, red, black);
}
 
Game::~Game()
{
    delete m_impl;
}
 
bool Game::completed(int& winner) const
{
    return m_impl->completed(winner);
}

bool Game::takeTurn()
{
    return m_impl->takeTurn();
}

void Game::play()
{
    m_impl->play();
}
 
int Game::checkerAt(int c, int r) const
{
    return m_impl->checkerAt(c, r);
}
