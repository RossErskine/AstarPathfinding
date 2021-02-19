#ifndef GUI_GUARD
#define GUI_GUARD

#include "fltk.h"
#include "MyWindow.h"

namespace MyLib
{
	//------------------------------------------------------------------------------

	typedef void* Address;    // Address is a synonym for void*
	typedef void(*Callback)(Address, Address);    // FLTK's required function type for all callbacks

//------------------------------------------------------------------------------

	template<class W> W& reference_to(Address pw)
		// treat an address as a reference to a W
	{
		return *static_cast<W*>(pw);
	}

	//------------------------------------------------------------------------------

	class MyWidget {
		// Widget is a handle to an Fl_widget - it is *not* an Fl_widget
		// We try to keep our interface classes at arm's length from FLTK
	public:
		MyWidget(Point xy, int w, int h, const std::string& s, Callback cb)
			: loc(xy), width(w), height(h), label(s), do_it(cb)
		{}

		virtual void move(int dx, int dy) { hide(); pw->position(loc.x += dx, loc.y += dy); show(); }// missing hide();
		virtual void hide() { pw->hide(); }
		virtual void show() { pw->show(); }
		virtual void attach(MyWindow&) = 0;

		Point loc;
		int width;
		int height;
		std::string label;
		Callback do_it;

		virtual ~MyWidget() { }

	protected:
		MyWindow* own;    // every Widget belongs to a Window
		Fl_Widget* pw;  // connection to the FLTK Widget
	private:
		MyWidget& operator=(const MyWidget&); // don't copy Widgets
		MyWidget(const MyWidget&);
	};

	//------------------------------------------------------------------------------

	struct Button : MyWidget {
		Button(Point xy, int w, int h, const std::string& label, Callback cb)
			: MyWidget(xy, w, h, label, cb)
		{}

		void attach(MyWindow&);
	};

	//------------------------------------------------------------------------------

	struct In_box : MyWidget {
		In_box(Point xy, int w, int h, const std::string& s)
			:MyWidget(xy, w, h, s, 0) { }
		int get_int();
		std::string get_string();

		void attach(MyWindow& win);
	};

	//------------------------------------------------------------------------------

	struct Out_box : MyWidget {
		Out_box(Point xy, int w, int h, const std::string& s)
			:MyWidget(xy, w, h, s, 0) { }
		void put(int);
		void put(const std::string&);

		void attach(MyWindow& win);
	};

	

	//------------------------------------------------------------------------------

}
#endif

