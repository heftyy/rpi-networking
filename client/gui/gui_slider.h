#pragma once

#include <wx/wx.h>
#include <wx/tglbtn.h>
#include <wx/slider.h>
#include "gpio_pin.h"

class gui_slider {
public:

    gui_slider(wxPanel* panel, wxSizer* main_sizer, gpio_pin& pin, std::function<void(int, int, bool) >on_change = nullptr) : panel_(panel), on_change_(on_change), pin_(pin)
    {
        wxStaticBox *box = new wxStaticBox(panel_, wxID_ANY, wxT("&GPIO "+std::to_string(pin.pin_label)));
        wxSizer *widget_sizer = new wxStaticBoxSizer(box, wxHORIZONTAL);

        int max_value = pin.pwm ? 100 : 1;
        slider_ = new wxSlider(panel_, wxID_ANY, 0, 0, max_value, wxPoint(0, 0), wxSize(slider_width_, slider_height_), slider_flag_);
        panel->Bind(wxEVT_SCROLL_CHANGED, &gui_slider::on_change, this, slider_->GetId());
        widget_sizer->Add(slider_, 1, wxCENTRE | wxALL, 5);

        pwm_button_ = new wxToggleButton(panel_, wxID_ANY, wxT("PWM"), wxPoint(20, 20));
        panel->Bind(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &gui_slider::pwm_toggle, this, pwm_button_->GetId());
        widget_sizer->Add(pwm_button_, 0, wxALL | wxGROW, 5);

        main_sizer->Add(widget_sizer, 0, wxALL | wxGROW, 5);
    }

//protected:
    wxPanel* panel_;
    wxSlider* slider_;
    wxToggleButton* pwm_button_;
    std::function<void(int, int, bool) >on_change_;
    gpio_pin pin_;
    const static int slider_width_ = 200;
    const static int slider_height_ = 50;
    const static int slider_flag_ = 0 | wxSL_MIN_MAX_LABELS | wxSL_VALUE_LABEL;

    void on_change(wxScrollEvent& event) {
        if (on_change_ != nullptr) {
            on_change_(pin_.pin, event.GetInt(), pwm_button_->GetValue());
        }
    }
    
    void pwm_toggle(wxCommandEvent& event) {
        bool toggle = event.GetInt() > 0 ? true : false;
        if(toggle) slider_->SetMax(100);
        else slider_->SetMax(1);
        slider_->SetValue(0);
    }

    void on_scroll(wxScrollEvent& event) {
        event.GetInt();
    }
};

