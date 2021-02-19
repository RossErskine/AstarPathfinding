#include "Map.h"


int main()
{

	try
	{
		MyLib::Map_window win{ 800,800, "Pathfinding algorithm" };
		return MyLib::gui_main();
		
	}
	catch (const char* msg)
	{
		std::cerr << msg << std::endl;
	}
	catch (...)
	{
		std::cerr << "exception\n";
	}
}