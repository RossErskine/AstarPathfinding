#ifndef TOOL_GUARD
#define TOOL_GUARD 1

#include "fltk.h"
#include "Point.h"
#include <vector>
#include <string>


namespace MyLib
{
	struct MyColor {
		enum Color_type {
			red = FL_RED, blue = FL_BLUE, green = FL_GREEN,
			yellow = FL_YELLOW, white = FL_WHITE, black = FL_BLACK,
			magenta = FL_MAGENTA, cyan = FL_CYAN, dark_red = FL_DARK_RED,
			dark_green = FL_DARK_GREEN, dark_yellow = FL_DARK_YELLOW, dark_blue = FL_DARK_BLUE,
			dark_magenta = FL_DARK_MAGENTA, dark_cyan = FL_DARK_CYAN
		};
		enum Transparency { invisible = 0, visible = 255 };

		MyColor(Color_type cc) :c(Fl_Color(cc)), v(visible) { }
		MyColor(Color_type cc, Transparency vv) :c(Fl_Color(cc)), v(vv) { }
		MyColor(int cc) :c(Fl_Color(cc)), v(visible) { }
		MyColor(Transparency vv) :c(Fl_Color()), v(vv) { }

		int as_int() const { return c; }
		char visibility() const { return v; }
		void set_visibility(Transparency vv) { v = vv; }
	private:
		unsigned char v;	// 0 or 1 for now
		Fl_Color c;
	};

	

	struct MyLine_style {
		enum Line_style_type {
			solid = FL_SOLID,				// -------
			dash = FL_DASH,				// - - - -
			dot = FL_DOT,					// ....... 
			dashdot = FL_DASHDOT,			// - . - . 
			dashdotdot = FL_DASHDOTDOT,	// -..-..
		};
		MyLine_style(Line_style_type ss) :s(ss), w(0) { }
		MyLine_style(Line_style_type lst, int ww) :s(lst), w(ww) { }
		MyLine_style(int ss) :s(ss), w(0) { }

		int width() const { return w; }
		int style() const { return s; }
	private:
		int s;
		int w;
	};

	
	
	template<class T> class Vector_ref {
		std::vector<T*> v;
		std::vector<T*> owned;
	public:
		Vector_ref() {}

		Vector_ref(T* a, T* b = 0, T* c = 0, T* d = 0)
		{
			if (a) push_back(a);
			if (b) push_back(b);
			if (c) push_back(c);
			if (d) push_back(d);
		}

		~Vector_ref() { for (int i = 0; i < owned.size(); ++i) delete owned[i]; }

		void push_back(T& s) { v.push_back(&s); }
		void push_back(T* p) { v.push_back(p); owned.push_back(p); }

		// ???void erase(???)

		T& operator[](int i) { return *v[i]; }
		const T& operator[](int i) const { return *v[i]; }
		int size() const { return v.size(); }
	};
	typedef double Fct(double);
	
	class MyShape {	// deals with color and style, and holds sequence of lines
	protected:
		MyShape() { }
		MyShape(std::vector<Point> lst);  // add() the Points to this Shape


	//	Shape() : lcolor(fl_color()),
	//		ls(0),
	//		fcolor(Color::invisible) { }

		void add(Point p) { points.push_back(p); }
		void set_point(int i, Point p) { points[i] = p; }
	public:
		void draw() const;					// deal with color and draw_lines
		
	protected:
		virtual void draw_lines() const;	// simply draw the appropriate lines

	public:
		virtual void move(int dx, int dy);	// move the shape +=dx and +=dy

		void set_color(MyColor col) { lcolor = col; }
		MyColor color() const { return lcolor; }

		void set_style(MyLine_style sty) { ls = sty; }
		MyLine_style style() const { return ls; }

		void set_fill_color(MyColor col) { fcolor = col; }
		MyColor fill_color() const { return fcolor; }

		Point point(int i) const { return points[i]; }
		int number_of_points() const { return int(points.size()); }

		virtual ~MyShape() { }
		/*
		struct Window* attached;
		MyShape(const MyShape& a)
			:attached(a.attached), points(a.points), line_color(a.line_color), ls(a.ls)
		{
			if (a.attached)throw("attempt to copy attached shape");
		}
		*/
		
	private:
		std::vector<Point> points;	// not used by all shapes
		MyColor lcolor{ fl_color() };
		MyLine_style ls{ 0 };
		MyColor fcolor{ MyColor::invisible };

	};

	

	struct Fill {
		Fill() :no_fill(true), fcolor(0) { }
		Fill(MyColor c) :no_fill(false), fcolor(c) { }

		void set_fill_color(MyColor col) { fcolor = col; }
		MyColor fill_color() { return fcolor; }
	protected:
		bool no_fill;
		MyColor fcolor;
	};

	

	struct MyLine : MyShape {
		MyLine(Point p1, Point p2) { add(p1); add(p2); }
	};

	struct MyLines : MyShape {	// indepentdent lines
		MyLines() {}
		MyLines(std::initializer_list<Point> lst) : MyShape{ lst } { if (lst.size() % 2) throw "odd number of points for Lines"; }
		void draw_lines() const;
		void add(Point p1, Point p2) { MyShape::add(p1); MyShape::add(p2); }
	};

	struct MyRectangle : MyShape {

		MyRectangle(Point xy, int ww, int hh) :w{ ww }, h{ hh }
		{
			if (h <= 0 || w <= 0) throw("Bad rectangle: non-positive side");
			add(xy);
		}
		MyRectangle(Point x, Point y) :w{ y.x - x.x }, h{ y.y - x.y }
		{
			if (h <= 0 || w <= 0) throw("Bad rectangle: first point is not top left");
			add(x);
		}
		void draw_lines() const;

		//	void set_fill_color(MyColor col) { fcolor = col; }
		//	MyColor fill_color() { return fcolor; }

		int height() const { return h; }
		int width() const { return w; }
	private:
		int h;			// height
		int w;			// width
		//MyColor fcolor;	// fill color; 0 means "no fill"
	};

	

	
}
#endif