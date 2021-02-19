#include "Map.h"
#include <iostream>
#include <random>


namespace MyLib
{
	// ================================member functions =============================================//
	// =============================== constructor ======================================= //
	/* ====================================================================================
	*	Contsuctor for the GUI creates the grids full of rectangles that can be coloured,
	*	creates map for coordinates, blocked map is for the walls
	*	and starts the game loop.
	*///===================================================================================
	Map_window::Map_window(int w, int h, const std::string& title)
		: MyWindow(w, h, title), mStart(1), found(false), searches(0)
	{
		int count1 = 1, count2 = 1, count3 = 1;	// counting iterations separatley maping vector 
		ConstVar cv;
		mFinish = ((cv.x_size / cv.x_cells) * (cv.y_size / cv.y_cells));
		
		std::cout << getFinish() << '\n';
		// == creates initial grid and colours == //
		for (int x = 0; x < cv.x_size; x += cv.x_cells)
		{
			for (int y = 0; y < cv.y_size; y += cv.y_cells)
			{
				mRectangles.push_back(new MyRectangle{ MyLib::Point{x,y},cv.x_cells,cv.y_cells });
				mRectangles[mRectangles.size() - 1].set_color(MyColor::black);
				
				
				if (count1 == getStart()) {
					mRectangles[mRectangles.size() - 1].set_fill_color(MyColor::green);
				}
				else if (count1 == getFinish()) {
					mRectangles[mRectangles.size() - 1].set_fill_color(MyColor::red);
				}
				else {
					mRectangles[mRectangles.size() - 1].set_fill_color(MyColor::invisible);
				}
				count1++;
				attach(mRectangles[mRectangles.size() - 1]);
			}
		}
		// == create map for coordinates == //
		for (int i = 0; i != cv.x_size / cv.x_cells; i++)
		{
			std::vector<int> temp;
			for (int j = 0; j != cv.y_size / cv.y_cells; j++)
			{
				std::cout << count2 << '\t';
				temp.push_back(count2);
				count2++;
			}
			std::cout << '\t';
			mMap.push_back(temp);
		}
		
		// == create blocked map for wall creation == //
		for (int i = 0; i != cv.x_size / cv.x_cells; i++)
		{
			std::vector<int> temp;
			for (int j = 0; j != cv.y_size / cv.y_cells; j++)
			{
				
				if (randDouble(0, 1) < 0.4)
				{
					temp.push_back(0);
				}
				else temp.push_back(count3);
				
				count3++;
				
			}
			mBlocked.push_back(temp);
		}
		mBlocked[15][15] = 1;
		
		for (int i = 0; i != mBlocked.size(); i++)
		{
			for (int j = 0; j != mBlocked[i].size(); j++)
			{
				if (mBlocked[i][j] == 0)
				{
					Cell newCell(Coord{ i, j }, mMap);
					wallSet.push_back(newCell);
				}
			}
		}
		wallSet.pop_back();
		wallSet.pop_back();
		// == start openset with cell == //
		Cell startCell(Coord{ 0, 0 }, getMap());
		
		openSet.push_back(startCell);
		
		
		// == start loop == //
		constexpr double delay = 1.0;                   // Delay before first timeout
		Fl::add_timeout(delay, cb_draw_loop, this);
	}



	/* ========================================================================
	*	cb_draw_loop is a callback that keeps calling the draw_loop for redraw
	*///======================================================================
	void Map_window::cb_draw_loop(Address pw)
	{
		const double delay = 0.05;
		reference_to<Map_window>(pw).draw_loop();
		Fl::repeat_timeout(delay, cb_draw_loop, pw);
	}

	/* ======================================================================
	*	draw_loop is the function that searches through each cells neighbours 
	*	placing corisponding searches in either openSet; to be searched or 
	*	closedSet; and changes colours accordingly blue; to be searched 
	*	green; searched, red for quickest path and black for walls.
	*	then uses heuristics to find quickest path.
	*///=====================================================================
	void Map_window::draw_loop()
	{
		Cell curr(Coord{ 0,0 },getMap());	// note: make empty constructor!
		ConstVar cv;
		int x = cv.x_size / cv.x_cells;
		int y = cv.y_size / cv.y_cells;
		Cell finishCell(Coord{x -1,y-1 }, getMap());
		
		//std::cout << "finishCell: " << finishCell.getPosition() << std::endl;
		if (!openSet.empty())
		{
			int winner = 0;
			for (int i = 0; i < openSet.size(); i++)
			{
				if (openSet[i].getF() < openSet[winner].getF())
				{
					winner = i;
				}
			}
			searches++;
			curr = openSet[winner];
			//std::cout << "currCell: " << curr.getPosition() << std::endl;
			if (curr.getPosition() == finishCell.getPosition())
			{
				std::cout << "Searches:" << getSearches() << '\n';
				std::cout << "solution found";
				cameFrom = reconstruct_path(closedSet, curr, getMap());
				std::cout << "Searches:" << getSearches() << '\n';
				// == turns squares red == //
				for (int i = 0; i != cameFrom.size(); i++)
				{
					Cell temp = cameFrom[i];
					mRectangles[temp.getPosition() - 1].set_fill_color(MyColor::red);
				}
				show();
			}
			removeFromVector(openSet, curr);
			
			closedSet.push_back(curr);
			//std::cout << "current: " << curr.getPosition() << '\n';

			// == checks neighbours == //
			std::vector<Coord> neighbours = curr.getNeighbours();
			int count = 0;
			for (int i = 0; i != neighbours.size(); i++)
			{
				Cell neighbour(Coord{ neighbours[i].getX(),neighbours[i].getY() }, getMap());

				neighbour.setPrevious(curr.getCoord().getX(), curr.getCoord().getY());

				// == checks if neigbour is already in closedSet == //
				double tempg = 0;

				if (is_unique(closedSet,neighbour))
				{
					tempg = (curr.getG()) + 1;

					// == checks for best gscore == //
					if (!openSet.empty())
					{
						if (is_unique(openSet, neighbour))
						{
							std::cout << curr.getPosition() << " :-Neighbour: " 
								<< neighbour.getPosition() << std::endl;
							neighbour.setG(tempg);
							// == un-comment for different heuristic == //
							neighbour.setH(man_heuristic(neighbour, finishCell));
							//neighbour.setH(euc_heuristic(neighbour, finishCell));
							//neighbour.setH(euc_heuristic_squared(neighbour, finishCell));
							neighbour.setF(neighbour.getG() + neighbour.getH());

							if(is_blocked(neighbour,mBlocked))openSet.push_back(neighbour);
							
						}
						else {
							if (tempg < neighbour.getG())
								neighbour.setG(tempg);
						}
					}
					else
					{
						neighbour.setG(tempg);
						// == un-comment for different heuristic == //
						neighbour.setH(man_heuristic(neighbour, finishCell));
						//neighbour.setH(euc_heuristic(neighbour, finishCell));
						//neighbour.setH(euc_heuristic_squared(neighbour, finishCell));
						neighbour.setF(neighbour.getG() + neighbour.getH());

						if (is_blocked(neighbour, mBlocked))openSet.push_back(neighbour);
						count++;
					}
				}
			}
		}
		else
		{
			show();
			std::cout << "Searches:" << getSearches() << '\n';
			throw "no solution";
		}

		
		

		// == turns squares unsearched blue == //
		for (int i = 0; i != closedSet.size(); i++)
		{
			Cell temp = closedSet[i];
			mRectangles[temp.getPosition()-1].set_fill_color(MyColor::blue);
		}

		// == turns squares green == //
		for (int i = 0; i != openSet.size(); i++)
		{
			Cell temp = openSet[i];
			mRectangles[temp.getPosition()-1].set_fill_color(MyColor::green);
		}

		//cameFrom =  reconstruct_path(closedSet, curr, getMap());
		
		// == turns squares red == //
		for (int i = 0; i != cameFrom.size(); i++)
		{
			Cell temp = cameFrom[i];
			mRectangles[temp.getPosition() - 1].set_fill_color(MyColor::red);
		}

		// == turns squares black == //
		for(int i = 0; i != wallSet.size(); i++)
		{
			Cell temp = wallSet[i];
			mRectangles[temp.getPosition() - 1].set_fill_color(MyColor::black);
		}

		redraw();
		
	}

	

	//============================================ operators =========================================//
	bool operator==(const Coord& a, const Coord& b)
	{
		return a.getX() == b.getX() &&
			a.getY() == b.getY();
			
	}

	bool operator!=(const Coord& a, const Coord& b)
	{
		return !(a == b);
	}

	bool operator==(const Cell& a, const Cell& b)
	{
		return a.getCoord() == b.getCoord() &&
			a.getF() == b.getF() &&
			a.getG() == b.getG() &&
			a.getH() == b.getH() &&
			a.getPosition() == b.getPosition();
	}

	bool operator!=(const Cell& a, const Cell& b)
	{
		return !(a==b);
	}


	// ========================================== helper functions ==================================== //
	std::vector<Cell> reconstruct_path(std::vector<Cell>& closed,  Cell current, std::vector<std::vector<int>> map)
	{
		std::vector<Cell> total_path;
		total_path.push_back(current);

		while (current.getPosition() != 1)
		{
			
			for (int i = 0; i < closed.size(); i++)
			{
				if (current.getPosition() == closed[i].getPosition())
				{
					Cell previous(Coord{ closed[i].getPrevious() }, map);
					current = previous;
					total_path.push_back(current);
				}
			}
		}
		return total_path;
	}
	
	//=============removefromvector=========================//
	/*loops backwards because the positions can move and 
	* and objects may be missed
	//=====================================================*/
	void removeFromVector(std::vector<Cell>& vec, Cell& cell)
	{
		for (int i = vec.size() - 1; i >= 0; i--) 
		{
			if (vec[i] == cell)
			{
				vec.erase(vec.begin() + i);
			}
		}
	}
	
	/* =========================== randint =========================================//
	*   "random number engine  is a function that generates uniformly distributed
	*	sequence of integer values."
	*	"Distribution is a function that generates a sequence of values according
	*	to a mathematical formaula given a sequence of value from an engine."
	*	Stroustrup (2014: p914)
	*///===========================================================================//
	double randDouble(double min, double max)// generates random number
	{
		static std::default_random_engine ran;
		return std::uniform_real_distribution<>{min, max}(ran);
	}

	
	/* ===========================================================================
	*	checks if Cell in a set is unique
	*///==========================================================================
	bool is_unique(const std::vector<Cell>& set, const Cell& cell)
	{
		for (int i = 0; i != set.size(); i++)
		{
			if (set[i].getPosition() == cell.getPosition())return false;
		}
		return true;
	}
	/* ============================================================================
	*	Checks if coordinates is a wall
	*///===========================================================================
	bool is_blocked(const Cell& cell, std::vector<std::vector<int>> blockedMap)
	{
		if (blockedMap[cell.getCoord().getX()][cell.getCoord().getY()] != 0)
			return true;
		else return false;
	}
	/* =============================================================================
	*	Euclidean distance squared heuristic
	*///============================================================================
	
	double euc_heuristic_squared(Cell& cell, const Cell& finish)
	{

		return ((cell.getCoord().getX() - finish.getCoord().getX())
					* (cell.getCoord().getX() - finish.getCoord().getX())
						+ (cell.getCoord().getY() - finish.getCoord().getY())
							* (cell.getCoord().getY() - finish.getCoord().getY()));

	}
	/* ===============================================================================
	*	Euclidean distance heuristic
	*///==============================================================================
	double euc_heuristic(Cell& cell, const Cell& finish)
	{
		
		 return ((double)sqrt( (
			 (cell.getCoord().getX() - finish.getCoord().getX() )
			 * (cell.getCoord().getX() - finish.getCoord().getX() ) )
			 + ( (cell.getCoord().getY() - finish.getCoord().getY() )
				 * (cell.getCoord().getY() - finish.getCoord().getY() ) ) ) );
		 
	}
	/* ===============================================================================
	*	Manhatton distance heuristic 
	*///==============================================================================

	double man_heuristic(Cell& cell, const Cell& finish)
	{
		return abs(cell.getCoord().getX() - finish.getCoord().getX())
			+  abs(cell.getCoord().getY() - finish.getCoord().getY());

	}

}

