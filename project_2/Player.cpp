// Player.cpp

#include "provided.h"
#include <string>
#include <iostream>
using namespace std;




/////////////////  implementation of Player ///////////////////

/*
 
 
 class Player
 {
 public:
 Player(std::string nm) : m_name(nm) {}
 virtual ~Player() {}
 std::string name() const { return m_name; };
 virtual bool isInteractive() const { return false; }
 virtual int chooseMove(const Scaffold& s, int N, int color) = 0;
 // We prevent any kind of Player object from being copied or assigned by
 // making the copy constructor and assignment operator unavailable in
 // the base class (and thus in any derived class).
 Player(const Player& other) = delete;
 Player& operator=(const Player& other) = delete;
 private:
 std::string m_name;
 };

 
 
 */

//Player::Player(std::string name)
    //Create a Player with the indicated name.
//string Player::name() const
    //Return the name of the player.
//virtual bool Player::isInteractive() const
    //Return false if the player is a computer player. Return true if the player is human. Most kinds of players will be
    //computer players.
//virtual int Player::chooseMove(const Scaffold& s, int N, int color) const = 0;
    //Every concrete class derived from this class must implement this function so that if the player were to be playing the
    //indicated color and have to make a move given scaffold s, with a goal of getting N in a row, the function returns the
    //column the player would choose. If no move is possible, return -1.
//virtual Player::~Player()
      //  Since this class is designed as a base class, it should have a virtual destructor.
    






/////////////////////////

int getScore(Scaffold& scaff, int column, int N, int color, int winColor);

class HumanPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
};





/////////////

class BadPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
};



//////////////

class SmartPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
  private:
    Scaffold* m_scaf;
    
    
};



//////////////

int HumanPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
	Scaffold scaff = s;
	cout << "choose column to move checker:" << endl;
	int c;
	cin >> c;
	while(!scaff.makeMove(c,color))
	{
		cout << "pls choose the column with empty space:" << endl;
		cin >> c;
	}
    return c;  //  This is not always correct; it's just here to compile
}


/////////////

int BadPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
	int c;
	for(int c = 0; c < s.cols(); c++)
		if(s.checkerAt(c+1,s.levels()) == VACANT)
			return c+1;
    return -1;  //  This is not always correct; it's just here to compile
}

/////////////

int SmartPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
	int opScore = INT_MIN, opCol = -1;
    Scaffold m_scaf(s);
    for(int i = 0; i < s.cols(); i++)
    {
		if(m_scaf.checkerAt(i+1, m_scaf.levels()) != VACANT)
			continue;
        int score = getScore(m_scaf, i+1, N, color, color);
		if(score > opScore)
		{
			opScore = score;
			opCol = i+1;
		}
		if(opScore == 1)
			break;
    }
	if(opScore == -1)
		cout << "I cannot win...\n";
	else if(opScore == 0)
		cout << "It is gonna tie!\n";
	else
		cout << "I can win!\n";
    return opCol;
}

//******************** Player derived class functions *************************

// These functions simply delegate to the Impl classes' functions.
// You probably don't want to change any of this code.

HumanPlayer::HumanPlayer(string nm)
 : Player(nm)
{
    m_impl = new HumanPlayerImpl;
}
 
HumanPlayer::~HumanPlayer()
{
    delete m_impl;
}
 
int HumanPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

BadPlayer::BadPlayer(string nm)
 : Player(nm)
{
    m_impl = new BadPlayerImpl;
}
 
BadPlayer::~BadPlayer()
{
    delete m_impl;
}
 
int BadPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

SmartPlayer::SmartPlayer(string nm)
 : Player(nm)
{
    m_impl = new SmartPlayerImpl;
}
 
SmartPlayer::~SmartPlayer()
{
    delete m_impl;
}
 
int SmartPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}
