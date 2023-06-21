#include "Grid.h"
#include "Square.h"
#include <random>
#include <iostream>
#include <stack>
#include <set>

using namespace std;

// 0 : blocked , 1 : way , 2 : open , 3 :closed , 4 : path done
Grid::Grid()
{
	randomGrid();
}

Grid::Grid(int width, int height)
{
	this->width = width;
	this->height = height;
}

Grid::~Grid()
{
}

void Grid::randomGrid()
{
	srand(time(NULL));
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if ((i == 0 && j == 0) || (i == ROW - 1 && j == COL - 1)) numGrid[i][j] = 1;
			else {
				int rd = rand() % 4;
				numGrid[i][j] = (rd == 0) ? 0 : 1;
			}
		}
	}
}

void Grid::renderGrid(sf::RenderTarget* target)
{
	this->squareArea = (windowWidth - start * 2 - (COL - 1) * gap) / COL;
	if (startAstar && !foundDest && !donePathFind)
	{
		elapsed_time += r.restart();
		if (elapsed_time >= delta_time) {
			delay = false;
			elapsed_time -= delta_time;
		}
		if (!delay)
		{
			aStarSearch(numGrid, make_pair(ROW - 1, COL - 1));
			delay = true;
		}
	}
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			Square square(squareArea, start + (squareArea + gap) * j, start + (squareArea + gap) * i);
			if (!numGrid[i][j]) square.setSquareColor(sf::Color(97, 97, 97));  // 28, 28, 28), sf::Color(54, 54, 54)

			if (numGrid[i][j] == 2) square.setSquareColor(sf::Color(195, 221, 240));
			if (numGrid[i][j] == 3) square.setSquareColor(sf::Color(161, 161, 145));
			if (numGrid[i][j] == 4) square.setSquareColor(sf::Color(204, 4, 252));

			if (i == 0 && j == 0) square.setSquareColor(sf::Color::Green);
			if (i == ROW - 1 && j == COL - 1) square.setSquareColor(sf::Color::Red);

			square.render(target);
		}
	}
}

void Grid::initData() {
	memset(closedList, false, sizeof(closedList));
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			cellDetails[i][j].f = FLT_MAX;
			cellDetails[i][j].g = FLT_MAX;
			cellDetails[i][j].h = FLT_MAX;
			cellDetails[i][j].parent_i = -1;
			cellDetails[i][j].parent_j = -1;
		}
	}
	int i= 0, j = 0;
	cellDetails[i][j].f = 0.0;
	cellDetails[i][j].g = 0.0;
	cellDetails[i][j].h = 0.0;
	cellDetails[i][j].parent_i = i;
	cellDetails[i][j].parent_j = j;

	openList.clear();
	openList.insert(make_pair(0.0, make_pair(0, 0)));
	foundDest = false;
	startAstar = true;
	donePathFind = false;
}

void Grid::doPathFind()
{
	initData();
	enableRdBt = false;
	enableStBt = false;
}

bool Grid::isUnBlocked(int grid[][COL], int row, int col)
{
	return grid[row][col] == 1;
}

bool Grid::isDestination(int row, int col, Pair dest)
{
	return row == dest.first && col == dest.second;
}

double Grid::calculateHValue(int row, int col, Pair dest)
{
	return abs(row - dest.first) + abs(col - dest.second);
}

bool Grid::isValid(int row, int col)
{
	return (row >= 0) && (row < ROW) && (col >= 0) && (col < COL);
}

void Grid::tracePath(cell cellDetails[][COL], Pair dest)
{
	int row = dest.first;
	int col = dest.second;
	stack<Pair> Path;

	while (!(cellDetails[row][col].parent_i == row && cellDetails[row][col].parent_j == col)) {
		Path.push(make_pair(row, col));
		int temp_row = cellDetails[row][col].parent_i;
		int temp_col = cellDetails[row][col].parent_j;
		row = temp_row;
		col = temp_col;
	}

	Path.push(make_pair(row, col));
	while (!Path.empty()) {
		pair<int, int> p = Path.top();
		Path.pop();
		int i = p.first;
		int j = p.second;
		numGrid[i][j] = 4;
	}
	enableRdBt = true;
	enableStBt = true;
	return;
}

void Grid::aStarSearch(int grid[][COL], Pair dest)
{
	if (!openList.empty()) {
		pPair p = *openList.begin();
		int i = p.second.first;
		int j = p.second.second;
		openList.erase(openList.begin());
		closedList[i][j] = true;
		numGrid[i][j] = 3;

		double gNew, hNew, fNew;

		if (isValid(i - 1, j) == true) {
			if (isDestination(i - 1, j, dest) == true) {
				cellDetails[i - 1][j].parent_i = i;
				cellDetails[i - 1][j].parent_j = j;
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}
			else if (closedList[i - 1][j] == false && isUnBlocked(grid, i - 1, j) == true) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue(i - 1, j, dest);
				fNew = gNew + hNew;

				if (cellDetails[i - 1][j].f == FLT_MAX || cellDetails[i - 1][j].h > hNew) {
					openList.insert(make_pair(hNew, make_pair(i - 1, j)));
					numGrid[i - 1][j] = 2;

					cellDetails[i - 1][j].f = fNew;
					cellDetails[i - 1][j].g = gNew;
					cellDetails[i - 1][j].h = hNew;
					cellDetails[i - 1][j].parent_i = i;
					cellDetails[i - 1][j].parent_j = j;
				}
			}
		}

		if (isValid(i + 1, j) == true) {
			if (isDestination(i + 1, j, dest) == true) {
				cellDetails[i + 1][j].parent_i = i;
				cellDetails[i + 1][j].parent_j = j;
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}
			else if (closedList[i + 1][j] == false && isUnBlocked(grid, i + 1, j) == true) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue(i + 1, j, dest);
				fNew = gNew + hNew;

				if (cellDetails[i + 1][j].f == FLT_MAX || cellDetails[i + 1][j].h > hNew) {
					openList.insert(make_pair(hNew, make_pair(i + 1, j)));
					numGrid[i + 1][j] = 2;

					cellDetails[i + 1][j].f = fNew;
					cellDetails[i + 1][j].g = gNew;
					cellDetails[i + 1][j].h = hNew;
					cellDetails[i + 1][j].parent_i = i;
					cellDetails[i + 1][j].parent_j = j;
				}
			}
		}

		if (isValid(i, j + 1) == true) {
			if (isDestination(i, j + 1, dest) == true) {
				cellDetails[i][j + 1].parent_i = i;
				cellDetails[i][j + 1].parent_j = j;
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}

			else if (closedList[i][j + 1] == false && isUnBlocked(grid, i, j + 1) == true) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue(i, j + 1, dest);
				fNew = gNew + hNew;

				if (cellDetails[i][j + 1].f == FLT_MAX || cellDetails[i][j + 1].h > hNew) {
					openList.insert(make_pair(hNew, make_pair(i, j + 1)));
					numGrid[i][j + 1] = 2;
					cellDetails[i][j + 1].f = fNew;
					cellDetails[i][j + 1].g = gNew;
					cellDetails[i][j + 1].h = hNew;
					cellDetails[i][j + 1].parent_i = i;
					cellDetails[i][j + 1].parent_j = j;
				}
			}
		}

		if (isValid(i, j - 1) == true) {
			if (isDestination(i, j - 1, dest) == true) {
				cellDetails[i][j - 1].parent_i = i;
				cellDetails[i][j - 1].parent_j = j;
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}
			else if (closedList[i][j - 1] == false && isUnBlocked(grid, i, j - 1) == true) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue(i, j - 1, dest);
				fNew = gNew + hNew;

				if (cellDetails[i][j - 1].f == FLT_MAX || cellDetails[i][j - 1].h > hNew) {
					openList.insert(make_pair(hNew, make_pair(i, j - 1)));
					numGrid[i][j - 1] = 2;

					cellDetails[i][j - 1].f = fNew;
					cellDetails[i][j - 1].g = gNew;
					cellDetails[i][j - 1].h = hNew;
					cellDetails[i][j - 1].parent_i = i;
					cellDetails[i][j - 1].parent_j = j;
				}
			}
		}
	}
	else {
		donePathFind = true;
		enableRdBt = true;
		enableStBt = true;
	}
}