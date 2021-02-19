#ifndef WINDOW_GUARD
#define WINDOW_GUARD 1

#include "fltk.h"
#include "Point.h"
#include <vector>
#include <string>



namespace MyLib
{
	class MyShape;	// "forward declare" Shape
	class MyWidget;
	class Vehicle;

	class MyWindow : public Fl_Window {
	public:
		MyWindow(int w, int h, const std::string& title);			// let the system pick the location
		MyWindow(Point xy, int w, int h, const std::string& title);	// top left corner in xy
		virtual ~MyWindow() { }

		int x_max() const { return w; }
		int y_max() const { return h; }

		void resize(int ww, int hh) { w = ww, h = hh; size(ww, hh); }

		void set_label(const std::string& s) { label(s.c_str()); }

		void attach(MyShape& s);
		void attach(MyWidget& w);
		
		void detach(MyShape& s);	// remove s from shapes 
		void detach(MyWidget& w);	// remove w from window (deactivate callbacks)
		
		void put_on_top(MyShape& p);	// put p on top of other shapes
		
	protected:
		void draw();

	private:
		std::vector<MyShape*> shapes;	// shapes attached to window
		
		int w, h;					// window size

		void init();
	};

	int gui_main();	// invoke GUI library's main event loop

	inline int x_max() { return Fl::w(); }	// width of screen in pixels
	inline int y_max() { return Fl::h(); }	// height of screen in pixels
	
	
	
}
#endif
#pragma once
