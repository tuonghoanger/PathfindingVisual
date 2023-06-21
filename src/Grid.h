#ifndef GRID_H
#define GRID_H

#include <utility>
#include <SFML/Graphics.hpp>
#include "Square.h"
#include <set>

using namespace std;

extern int windowWidth, windowHeight;
extern bool enableRdBt, enableStBt;

#define ROW 20
#define COL 50

typedef pair<int, int> Pair;
typedef pair<double, pair<int, int>> pPair;

struct cell {
	int parent_i, parent_j;
	double f, g, h; 	// f = g + h
};

class Grid
{
private:
	int numGrid[ROW][COL];
	int start = 15, gap = 3;
	int squareArea;
	int width = 0, height = 0;
	bool closedList[ROW][COL];
	cell cellDetails[ROW][COL];
	bool foundDest;
	bool donePathFind = false;
	bool startAstar = false;
	bool delay = false;

	set<pPair> openList;

	sf::Time elapsed_time;
	sf::Clock r;
	sf::Time delta_time = sf::milliseconds(70);

public:
	Grid();
	Grid(int width,int height);
	~Grid();

	void randomGrid();
	void initData();
	void doPathFind(); 
	void renderGrid(sf::RenderTarget* target);
	void renderRandomGrid(sf::RenderTarget* target);
	bool isValid(int row, int col);
	bool isUnBlocked(int grid[][COL], int row, int col);
	bool isDestination(int row, int col, Pair dest);
	double calculateHValue(int row, int col, Pair dest);
	void tracePath(cell cellDetails[][COL], Pair dest);
	void aStarSearch(int grid[][COL], Pair dest);
};

#endif // !GRID_H