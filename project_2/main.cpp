// main.cpp

#include "provided.h"
#include <iostream>
#include <cassert>
using namespace std;

void doScaffoldTests()
{
    Scaffold s(3, 2);
    assert(s.cols() == 3  &&  s.levels() == 2  &&
           s.numberEmpty() == 6);
    
    assert(s.undoMove()==0);// when the grid is empty
    assert(s.makeMove(1, RED));
    assert(s.makeMove(1, BLACK));
    assert(!s.makeMove(1, RED));
    assert(s.numberEmpty() == 4);
    assert(s.checkerAt(1, 1) == RED  &&  s.checkerAt(1, 2) == BLACK);
    assert(s.checkerAt(2, 1) == VACANT);
    s.display();
    assert(s.undoMove()==1);
    s.display();
    
    assert(s.makeMove(2, RED));
    assert(s.makeMove(2, BLACK));
    assert(s.makeMove(3, RED));
    assert(s.makeMove(3, BLACK));
    
    assert(s.makeMove(1, RED));
    s.display();
}

void doPlayerTests()
{
	HumanPlayer hp("Marge");
	assert(hp.name() == "Marge"  &&  hp.isInteractive());
	BadPlayer bp("Homer");
	assert(bp.name() == "Homer"  &&  !bp.isInteractive());
	SmartPlayer sp("Lisa");
	assert(sp.name() == "Lisa"  &&  !sp.isInteractive());
	Scaffold s(3, 2);
	s.makeMove(1, RED);
	s.makeMove(1, BLACK);
	s.display();
	cout << "=========" << endl;
	int n = hp.chooseMove(s, 3, RED);
	cout << "=========" << endl;
	assert(n == 2  ||  n == 3);
	n = bp.chooseMove(s, 3, RED);
	assert(n == 2  ||  n == 3);
	n = sp.chooseMove(s, 3, RED);
	assert(n == 2  ||  n == 3);
}

void doGameTests()
{
	BadPlayer bp1("Bart");
	BadPlayer bp2("Homer");
	Game g(2, 2, 2, &bp1, &bp2);
	int winner;
	assert(!g.completed(winner));
	g.takeTurn();  // Red's first move
	assert(!g.completed(winner)  &&
		(g.checkerAt(1, 1) == RED  ||  g.checkerAt(2, 1) == RED));
	g.takeTurn();  // Black's first move
	assert(!g.completed(winner));
	g.takeTurn();  // Red's second move; Red must win
	assert(g.completed(winner)  &&  winner == RED);
}

void doMyTest()
{
	BadPlayer sp1("me");
	SmartPlayer sp2("you");
	Game g(10,1,3,&sp1,&sp2);
	g.play();
	cout << "game is end. press any key to continue...";
	cin.ignore();
}
int main()
{
    // doScaffoldTests();
	// doPlayerTests();
	// doGameTests();
	doMyTest();
    cout << "Passed all tests" << endl;
}
