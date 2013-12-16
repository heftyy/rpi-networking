#pragma once
#include <chrono>
#include <memory>
#include <functional>
#include <vector>
#include <wx/wx.h>
#include <wx/slider.h>
#include "gpio_pin.h"

class gui_sliders_panel : public wxPanel
{
public:
	//gui_sliders_panel(wxFrame * parent, std::vector<gpio_pin> pins, std::function<void(int)> on_change = nullptr) : wxPanel(parent), on_change_(on_change)
	gui_sliders_panel(wxFrame * parent, std::function<void(int, int)> on_change = nullptr) : wxPanel(parent), on_change_(on_change)
	{
		main_sizer_ = new wxBoxSizer(wxHORIZONTAL);
		slider_sizer_ = new wxBoxSizer(wxVERTICAL);
	}

	void render(std::vector<gpio_pin> pins)
	{
		slider_sizer_->Clear();
		int i = 0;
		for (auto pin : pins)
		{
			render_slider(ID_SLIDER_MIN + pin.pin);
			i++;
		}

		main_sizer_->Add(slider_sizer_);
		SetSizer(main_sizer_);
	}

	void refresh()
	{
		Refresh();
	}

protected:
	wxSizer* main_sizer_;
	wxSizer* slider_sizer_;
	std::vector<wxSlider*> sliders_;
	std::function<void(int, int)> on_change_;
	const static int ID_SLIDER_MIN = 200;
	const static int slider_width_ = 150;
	const static int slider_height_ = 20;

	void render_slider(int slider_id)
	{
		auto slider = new wxSlider(this, slider_id, 0, 0, 100, wxPoint(20, 0), wxSize(slider_width_, slider_height_), 0);
		sliders_.push_back(slider);

		if (slider->HasFlag(wxSL_VERTICAL))
		{
			slider_sizer_->Add(0, 0, 1);
			slider_sizer_->Add(slider, 0, wxGROW | wxALL, 5);
			slider_sizer_->Add(0, 0, 1);
		}
		else
		{
			slider_sizer_->Add(slider, 1, wxCENTRE | wxALL, 5);
		}

		Connect(slider_id, wxEVT_SCROLL_CHANGED, wxScrollEventHandler(gui_sliders_panel::on_change));
		Connect(slider_id, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(gui_sliders_panel::on_scroll));
		//Connect(wxEVT_PAINT, wxPaintEventHandler(gui_slider::on_paint));
	}

	void on_change(wxScrollEvent& event)
	{
		if (!on_change_._Empty())
		{
			on_change_(event.GetId() - ID_SLIDER_MIN, event.GetInt());
		}
	}

	void on_scroll(wxScrollEvent& event)
	{
		event.GetInt();
	}

	/*
	void on_paint(wxPaintEvent& event)
	{
		wxPaintDC dc(this);

		wxPen pen(wxColour(212, 212, 212));
		dc.SetPen(pen);

		int rect_x, rect_y;
		if (flag_ & wxVERTICAL)
		{
			rect_x = point_.x + size_.GetWidth();
			rect_y = point_.y;
		}
		else if (flag_ & wxHORIZONTAL)
		{
			rect_x = point_.x;
			rect_y = point_.y + size_.GetHeight();
		}

		dc.DrawRectangle(wxRect(rect_x, rect_y, rectangle_width_, rectangle_height_));

		wxBrush brush1(wxColour(30, 180, 30));
		dc.SetBrush(brush1);

		if (flag_ & wxVERTICAL)
		{
			dc.DrawRectangle(wxRect(rect_x, rect_y, rectangle_width_, get_fill_percentage()));
		}
		else if (flag_ & wxHORIZONTAL)
		{
			dc.DrawRectangle(wxRect(rect_x, rect_y, get_fill_percentage(), rectangle_height_));
		}
	}

	int get_fill_percentage()
	{
		int rectangle_length = 0;
		if (flag_ & wxVERTICAL)
		{
			rectangle_length = rectangle_height_;
		}
		else if (flag_ & wxHORIZONTAL)
		{
			rectangle_length = rectangle_width_;
		}
		int value_perc = value_ * 100 / max_value_;
		fill_ = value_perc * rectangle_length / 100;
		return fill_;
	}
	*/
};
