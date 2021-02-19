#include "MyWindow.h"
#include "GUI.h"
#include "MyTools.h"


namespace MyLib 
{
	MyWindow::MyWindow(int ww, int hh, const std::string& title)
		:Fl_Window(ww, hh, title.c_str()), w(ww), h(hh)
	{
		init();
	}

	MyWindow::MyWindow(Point xy, int ww, int hh, const std::string& title)
		: Fl_Window(xy.x, xy.y, ww, hh, title.c_str()), w(ww), h(hh)
	{
		init();
	}

	void MyWindow::init()
	{
		resizable(this);
		show();
	}

	//---------------------------------------------------- 

	void MyWindow::draw()
	{
		Fl_Window::draw();
		for (unsigned int i = 0; i < shapes.size(); ++i) shapes[i]->draw();
	}

	void MyWindow::attach(MyWidget& w)
	{
		begin();			// FTLK: begin attaching new Fl_Wigets to this window
		w.attach(*this);	// let the Widget create its Fl_Wigits
		end();				// FTLK: stop attaching new Fl_Wigets to this window
	}

	

	void MyWindow::detach(MyWidget& b)
	{
		b.hide();
	}

	void MyWindow::attach(MyShape& s)
	{
		shapes.push_back(&s);
		//		s.attached = this;
	}
	void MyWindow::detach(MyShape& s)
	{
		for (unsigned int i = shapes.size(); 0 < i; --i)	// guess last attached will be first released
			if (shapes[i - 1] == &s)
				shapes.erase(shapes.begin() + (i - 1));//&shapes[i-1]);
	}


	void MyWindow::put_on_top(MyShape& p) {
		for (int i = 0; i < shapes.size(); ++i) {
			if (&p == shapes[i]) {
				for (++i; i < shapes.size(); ++i)
					shapes[i - 1] = shapes[i];
				shapes[shapes.size() - 1] = &p;
				return;
			}
		}
	}
	

	int gui_main() { return Fl::run(); }


	
	

}