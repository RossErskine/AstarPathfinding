#ifndef MAP_GUARD
#define MAP_GUARD 1

#include "GUI.h"
#include "MyTools.h"
#include <iostream>


namespace MyLib
{
	/* =======================================================
	*	ConstVar is the const variable into a simple class.
	*///======================================================
	struct ConstVar
	{
		// == grid width & height == //
		const int x_size = 800;	
		const int y_size = 800;
		// == cell width & height == //
		const int x_cells = 50;
		const int y_cells = 50;
	};

	/* =======================================================
	*	Coord class is just a way to simlify X & Y coordinates 
	*///======================================================
	class Coord
	{
	public:
		Coord(int x, int y) :mX(x), mY(y)
		{}
		// == non-modifying operations == //
		int getX() const { return mX; }
		int getY() const { return mY; }
			
	private:
		int mX;
		int mY;
		
	};
	// === Coord operator === //
	bool operator==(const Coord& a, const Coord& b);
	bool operator!=(const Coord& a, const Coord& b);

	
	/* =======================================================
	*	Each Cell holds the Coords, fValue, gValue, hValue for
	*	the heuristics. all the neighbour cells in a vector and 
	*	previous cells for finding quickedt path.
	*///======================================================
	class Cell
	{
	public:
		Cell(Coord coord,std::vector<std::vector<int>> map)
			:mCoord(coord),fValue(0)
			,gValue(0),hValue(0),mPrevious(0,0)
		{
			mPosition = getMapPosition(map, getCoord());
			ConstVar cv;

			// == sets neigbours Cell == //  
			Coord above(mCoord.getX(), mCoord.getY()-1);
			Coord left(mCoord.getX() -1, mCoord.getY());
			Coord right(mCoord.getX() +1, mCoord.getY());
			Coord below(mCoord.getX(), mCoord.getY()+1);
			if (mCoord.getY() > 0) mNeighbours.push_back(above);
			if (mCoord.getX() > 0) mNeighbours.push_back(left);
			if (mCoord.getX() < (cv.y_size / cv.y_cells) - 1) mNeighbours.push_back(right);
			if (mCoord.getY() < (cv.x_size / cv.x_cells) - 1) mNeighbours.push_back(below);

			
			// == for diagonals == //
			Coord leftAbove(mCoord.getX() - 1, mCoord.getY() - 1);
			Coord rightAbove(mCoord.getX() + 1, mCoord.getY() - 1);
			Coord leftBelow(mCoord.getX() - 1, mCoord.getY() + 1);
			Coord rightBelow(mCoord.getX() + 1, mCoord.getY() + 1);
			if (mCoord.getX() > 0 && mCoord.getY() > 0) mNeighbours.push_back(leftAbove);
			if (mCoord.getX() > 0 && mCoord.getY() < (cv.y_size / cv.y_cells) - 1)
				mNeighbours.push_back(rightAbove);
			if (mCoord.getX() < (cv.x_size / cv.x_cells) - 1 && mCoord.getY() > 0)
				mNeighbours.push_back(leftBelow);
			if (mCoord.getX() < (cv.x_size / cv.x_cells) - 1 
				&& mCoord.getY() < (cv.y_size / cv.y_cells) - 1) 
				mNeighbours.push_back(rightBelow);
			

			
		}
		// == non-modifying operations == //
		Coord getCoord() const { return mCoord; }
		double getF() const { return fValue; }
		double getG() const { return gValue; }
		double getH() const { return hValue; }
		std::vector<Coord> getNeighbours() const { return mNeighbours; }
		int getMapPosition(std::vector<std::vector<int>>& vec, Coord coord)
		{
			if (coord.getX() < 0 || coord.getY() < 0) return 0;// throw"getMapPosition: out of range!";
			return vec[coord.getX()][coord.getY()];
		}
		int getPosition() const { return mPosition; }
		Coord getPrevious() const { return mPrevious; }
		
		// == modifying operations == //
		void setF(double f) { fValue = f; }
		void setG(double g) { gValue = g; }
		void setH(double h) { hValue = h; }
		void setPosition(int pos) { mPosition = pos; }
		void setPrevious(int x, int y) 
		{
			Coord p(x, y);
			mPrevious = p;
		}
		
	private:
		Coord mCoord;
		double fValue;
		double gValue;
		double hValue;
		std::vector<Coord> mNeighbours;
		Coord mPrevious;
		int mPosition;
		

	};
	// === Cell operator === //
	bool operator==(const Cell& a, const Cell& b);
	bool operator!=(const Cell& a, const Cell& b);
	
	/* =======================================================
	*	Creates the GUI window and holds the sets for 
	*	A* pathfinding
	*///======================================================
	struct Map_window : MyWindow
	{
		Map_window(int w, int h, const std::string& title);

		// == non-modifying operations == //
		int getStart() const { return mStart; }
		int getFinish() const { return mFinish; }
		std::vector<std::vector<int>> getMap() const { return mMap; }
		long long int getSearches() const { return searches; }
		
		// == modifying operations == //
		void setStart(int start) { mStart = start; }
		void setFinish(int finish) { mFinish = finish; }
		
		
	private:
		// == for drawing gui == //
		Vector_ref<MyRectangle> mRectangles;

		bool found;
		static void cb_draw_loop(Address pw);
		void draw_loop();
		

		//==pathfinding sets== //
		std::vector<std::vector<int>> mMap;
		std::vector<std::vector<int>> mBlocked;
		std::vector<Cell> openSet;
		std::vector<Cell> closedSet;
		std::vector<Cell> cameFrom;
		std::vector<Cell> wallSet;

		int mStart;
		int mFinish;
		long long int searches;
	};

	
	// == helper functions == //
	std::vector<Cell> reconstruct_path(std::vector<Cell>& closed, Cell current, std::vector<std::vector<int>> map);
	void removeFromVector(std::vector<Cell>& vec, Cell& cell);
	double randDouble(double min, double max);
	bool is_blocked(const Cell&, std::vector<std::vector<int>>);

	bool is_unique(const std::vector<Cell>&, const Cell&);
	double euc_heuristic_squared(Cell& cell, const Cell& finish);
	double euc_heuristic(Cell&, const Cell&);
	double man_heuristic(Cell&, const Cell&);
	
	void testRun();

	
	
}
#endif