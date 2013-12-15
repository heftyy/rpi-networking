#pragma once

#include <wx/wx.h>

class basic_pane : public wxPanel 
{
public:

	basic_pane(wxFrame* parent) : wxPanel(parent) 
	{
		std::cout << "basic_pane" << std::endl;
	}

	void paintEvent(wxPaintEvent& evt) 
	{
		wxPaintDC dc(this);
		render(dc);
	}

	void paintNow() 
	{
		wxClientDC dc(this);
		render(dc);
	}

	void render(wxDC& dc) 
	{
		std::cout << "print" << std::endl;
		static int y = 0;
		static int y_speed = 2;

		y += y_speed;
		if (y < 0) y_speed = 2;
		if (y > 200) y_speed = -2;

		dc.SetBackground(*wxWHITE_BRUSH);
		dc.Clear();
		dc.DrawText(wxT("Testing"), 40, y);
	}

	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(basic_pane, wxPanel)
EVT_PAINT(basic_pane::paintEvent)
END_EVENT_TABLE()