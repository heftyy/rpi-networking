#pragma once
#include <chrono>
#include <memory>
#include <functional>
#include <vector>
#include <wx/wx.h>
#include <wx/tglbtn.h>
#include <wx/slider.h>
#include "gpio_pin.h"
#include "gui_slider.h"

/*
struct {
    wxSlider* slider;
    wxToggleButton* pwm_button;
    wxToggleButton* state_button;
    wxSizer* slider_sizer, range_sizer;
};
*/

class gui_sliders_panel : public wxPanel {
public:
    //gui_sliders_panel(wxFrame * parent, std::vector<gpio_pin> pins, std::function<void(int)> on_change = nullptr) : wxPanel(parent), on_change_(on_change)

    gui_sliders_panel(wxFrame * parent, std::function<void(int, int, bool) > on_change = nullptr) : wxPanel(parent), on_change_(on_change) {
        main_sizer_ = new wxBoxSizer(wxVERTICAL);
    }

    void render(std::vector<gpio_pin> pins) {
        std::cout << "RENDER" << std::endl;
        main_sizer_->Clear();
		wxSizer* top_sizer = new wxBoxSizer(wxHORIZONTAL);
		range_multiplier_ = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
		top_sizer->Add(range_multiplier_, 0, wxTOP | wxCENTER, 5);
		range_multiplier_->SetValue(wxT("1"));

        wxGridSizer* grid_sizer_ = new wxGridSizer(pins.size()/3, 3, 3, 3);
        int i = 0;
        for (auto pin : pins) {
            wxSizer* slider_widget_sizer = new wxBoxSizer(wxHORIZONTAL);
            gui_slider* slider = new gui_slider(this, slider_widget_sizer, pin, on_change_);
            sliders_.push_back(slider);
            grid_sizer_->Add(slider_widget_sizer, 0, wxALL | wxGROW, 5);
            i++;
        }
		main_sizer_->Add(top_sizer, 0, wxCENTER, 5);
        main_sizer_->Add(grid_sizer_, 0, wxALL, 5);
        SetSizer(main_sizer_);
    }

    void refresh() {
        Refresh();
    }

	int get_range_multiplier()
	{
		return atoi(range_multiplier_->GetValue().c_str());
	}

protected:
    wxSizer* main_sizer_;
    wxSizer* slider_sizer_;
	wxTextCtrl* range_multiplier_;
    std::vector<gui_slider*> sliders_;
    std::function<void(int, int, bool) > on_change_;
    const static int ID_SLIDER_MIN = 200;
    const static int slider_width_ = 150;
    const static int slider_height_ = 40;

	wxSizer* create_sizer_with_text(wxControl *control, wxWindowID id, wxTextCtrl **ppText)
	{
		wxSizer *sizerRow = new wxBoxSizer(wxHORIZONTAL);
		range_multiplier_ = new wxTextCtrl(this, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);

		sizerRow->Add(control, 0, wxCENTER | wxALIGN_CENTRE_VERTICAL, 5);
		sizerRow->Add(range_multiplier_, 1, wxCENTER | wxALIGN_CENTRE_VERTICAL, 5);

		if (ppText) *ppText = range_multiplier_;

		return sizerRow;
	}

	// create a sizer containing a label and a text ctrl
	wxSizer* create_sizer_with_text_and_label(const wxString& label, wxWindowID id, wxTextCtrl **ppText)
	{
		return create_sizer_with_text(new wxStaticText(this, wxID_ANY, label), id, ppText);
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
