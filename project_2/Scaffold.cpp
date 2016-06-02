// Scaffold.cpp

#include "provided.h"
#include <vector>
#include <iostream>
#include <utility>
#include <stack>
#include <cassert>
using namespace std;


class ScaffoldImpl
{
  public:
    ScaffoldImpl(int nColumns, int nLevels);
    int cols() const;
    int levels() const;
    int numberEmpty() const;
    int checkerAt(int column, int level) const; 
    void display() const; 
    bool makeMove(int column, int color); 
    int undoMove();
    ///// add more fcn /////////
    int getScore(int column, int N);
    // column, level  are both from 1
private:
    int m_column, m_level, m_count;
    vector<vector<double>> grid;//use vector of vector to creat the grid
    
    stack<pair<int, int>> m_pre;
    //vector<int> preCol;
    //vector<int> preLevel;
    
    // m_count is counting the number of empty position
    
    
};

ScaffoldImpl::ScaffoldImpl(int nColumns, int nLevels)
:m_column(nColumns), m_level(nLevels), m_count(nColumns * nLevels)
{
    //vector<vector<double>> grid(m_level,vector<double>(m_column,-1));
    grid.resize(m_level);  // grid now has m_level empty rows
    for(int i = 0; i < m_level; i++)
        grid[i].resize(m_column);  // row now has m_column columns
    
    for (int i = 0; i < m_level; i++)
        for (int j = 0; j < m_column; j++)
            grid[i][j] = VACANT;
}

int ScaffoldImpl::cols() const
{
    return m_column;  // return the number of column
}

int ScaffoldImpl::levels() const
{
    return m_level;  //  return the number of levels
}

int ScaffoldImpl::numberEmpty() const
{
    return m_count;
}

int ScaffoldImpl::checkerAt(int column, int level) const
{
    return grid[level-1][column-1]; // the index shoule be the # of cols and levels minus 1
    
}

void ScaffoldImpl::display() const
{
    vector<vector<char>> gridchar(m_level,vector<char>(m_column,' '));
    for(int i = 0; i < m_level; i++)
        for (int j = 0; j < m_column; j++)
        {
            if(grid[i][j] == RED)
                gridchar[i][j] = 'R';
            if(grid[i][j] == BLACK)
                gridchar[i][j] = 'B';
            if(grid[i][j] == VACANT)
                gridchar[i][j] = ' ';
            
        }//initial the items in each cell of the grid
    
    for(int i = m_level-1; i >=0; i--)
    {    for (int j = 0;  j< m_column; j++)
        {
            cout <<'|' << gridchar[i][j];
            
        }
        cout << '|'<<endl;
    } // draw | to seprate the cells
    
    for(int i = 0; i < m_column; i++)
        cout << '+' << '-';
    cout << '+' << endl; // draw the bottom
}

bool ScaffoldImpl::makeMove(int column, int color)
{
    if(column > 0 && column <= m_column)
        for( int i = 0; i < m_level; i++)
        {
            if (grid[i][column-1] == VACANT) // check whether the cell is available
            {
                grid[i][column-1] = color;
                m_count--;
                m_pre.push(make_pair(i, column-1));
               // preCol.push_back(column-1);
               // preLevel.push_back(i);
                return true;
                
            }
            
        }
    return false;
}



int ScaffoldImpl::undoMove()
{
    if(m_count == m_column * m_level)  // if there is no color in the grid
        return 0;
    else
    {
        int r, c;
        r = m_pre.top().first;  // the most recent level
        c = m_pre.top().second; // the most rescent column
        
        grid[r][c] = VACANT;
        m_pre.pop();           // remove the checker
		m_count++;
        //preCol.pop_back();
        //preLevel.pop_back();
        return c+1;  // return the number of the column from which that checker was removed
    }
    
}

//******************** Scaffold functions *******************************

// These functions simply delegate to ScaffoldImpl's functions.
// You probably don't want to change any of this code.

Scaffold::Scaffold(int nColumns, int nLevels)
{
    m_impl = new ScaffoldImpl(nColumns, nLevels);
}
 
Scaffold::~Scaffold()
{
    delete m_impl;
}
 
Scaffold::Scaffold(const Scaffold& other)
{
    m_impl = new ScaffoldImpl(*other.m_impl);
}
 
Scaffold& Scaffold::operator=(const Scaffold& rhs)
{
    if (this != &rhs)
    {
        Scaffold temp(rhs);
        swap(m_impl, temp.m_impl);
    }
    return *this;
}
 
int Scaffold::cols() const
{
    return m_impl->cols();
}

int Scaffold::levels() const
{
    return m_impl->levels();
}

int Scaffold::numberEmpty() const
{
    return m_impl->numberEmpty();
}

int Scaffold::checkerAt(int column, int level) const
{
    return m_impl->checkerAt(column, level);
}
 
void Scaffold::display() const
{
    m_impl->display();
}
 
bool Scaffold::makeMove(int column, int color)
{
    return m_impl->makeMove(column, color);
}
 
int Scaffold::undoMove()
{
    return m_impl->undoMove();
}

int getConnectNumber(const Scaffold& scaff, int column, int N, int color)
{
	int c = column, r = -1;
	for(int i = scaff.levels()-1; i >=0; i--)
		if(scaff.checkerAt(c,i+1) != VACANT)
		{
			r = i;
			break;
		}
	int connect= INT_MIN;

	int colConnect = 0;
    for(int i = r; i >=0; i--)
    {
		if(scaff.checkerAt(c,i+1) == color)
            colConnect++;
        else
            break;
    }
    connect = max(colConnect, connect);

	// also need to write the other directions
	int diaConnect1 = 0;
	for(int i = r, j = c-1; i < scaff.levels() && j < scaff.cols(); i++, j++)
	{
		if(scaff.checkerAt(j+1,i+1) == color)
			diaConnect1++;
		else
			break;
	}
	for(int i = r, j = c-1; i >= 0 && j >= 0; i--, j--)
	{
		if(scaff.checkerAt(j+1,i+1) == color)
			diaConnect1++;
		else
			break;
	}
	connect = max(diaConnect1-1, connect);

	int diaConnect2 = 0;
	for(int i = r, j = c-1; i < scaff.levels() && j >= 0; i++, j--)
	{
		if(scaff.checkerAt(j+1,i+1) == color)
			diaConnect2++;
		else
			break;
	}
	for(int i = r, j = c-1; i >= 0 && j < scaff.cols(); i--, j++)
	{
		if(scaff.checkerAt(j+1,i+1) == color)
			diaConnect2++;
		else
			break;
	}
	connect = max(diaConnect2-1, connect);

	int levelConnect = 0;
	for(int j = c-1; j < scaff.cols(); j++)
	{
		if(scaff.checkerAt(j+1,r+1) == color)
			levelConnect++;
		else
			break;
	}
	for(int j = c-1; j >= 0; j--)
	{
		if(scaff.checkerAt(j+1,r+1) == color)
			levelConnect++;
		else
			break;
	}
	connect = max(levelConnect-1, connect);
	return connect;
}

int getScore(Scaffold& scaff, int column, int N, int color, int winColor)
{
    int c;
    c = column;
	bool moved = scaff.makeMove(c, color);
	//scaff.display();
	assert(moved == true);

    int connect = getConnectNumber(scaff, c, N, color);
    
    if(connect == N)
	{
		scaff.undoMove();
		if(color == winColor)
			return 1;
		else
			return -1;
	}
	if(scaff.numberEmpty() == 0)
	{
		scaff.undoMove();
		return 0;
	}

	int nextColor = (color == RED ? BLACK : RED);
	int score = INT_MAX;
	if(nextColor == winColor)
		score = INT_MIN;
	for(int j = 0; j < scaff.cols(); j++)
	{
		if(scaff.checkerAt(j+1,scaff.levels()) == VACANT)
		{
			int tmp = getScore(scaff, j+1, N, nextColor, winColor);
			if(nextColor == winColor)
			{
				score = max(score, tmp);
				if(score == 1)
					break;
			}
			else
			{
				score = min(score, tmp);
				if(score == -1)
					break;
			}
		}
	}
	scaff.undoMove();
	return score;
}