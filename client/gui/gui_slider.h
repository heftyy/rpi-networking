#pragma once
#include <chrono>

#include <wx/wx.h>
#include <wx/slider.h>

class gui_slider : public wxPanel
{
public:
	gui_slider::gui_slider(wxFrame * parent, wxPoint& point, wxSize& size, int init_value, int max_value, int flag, std::function<void(int)> on_change = nullptr)
		: wxPanel(parent, wxID_ANY), point_(point), size_(size), fill_(init_value), value_(init_value), max_value_(max_value), flag_(flag), on_change_(on_change)
	{
		slider_ = std::unique_ptr<wxSlider>(new wxSlider(this, ID_SLIDER, value_, 0, max_value_, point_, size_, flag_));

		Connect(ID_SLIDER, wxEVT_SCROLL_CHANGED, wxScrollEventHandler(gui_slider::on_change));
		Connect(ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(gui_slider::on_scroll));
		Connect(wxEVT_PAINT, wxPaintEventHandler(gui_slider::on_paint));

		rectangle_width_ = size_.GetWidth();
		rectangle_height_ = size_.GetHeight();
	}

	void refresh()
	{
		Refresh();
	}

protected:
	int fill_;
	int value_;
	int max_value_;
	int flag_;
	int rectangle_width_;
	int rectangle_height_;
	std::unique_ptr<wxSlider> slider_;
	std::function<void(int)> on_change_;
	wxPoint point_;
	wxSize size_;
	const static int ID_SLIDER = 100;

	void on_change(wxScrollEvent& event)
	{
		if (!on_change_._Empty())
		{
			on_change_(event.GetInt());
		}
	}

	void on_scroll(wxScrollEvent& event)
	{
		value_ = slider_->GetValue();
	}

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
		auto duration = std::chrono::system_clock::now().time_since_epoch();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

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
};
