#pragma once

#include <wx/wx.h>

enum 
{
	ID_Quit = 1,
	ID_About,
};

class gui_frame : public wxFrame 
{
public:

	gui_frame(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame(NULL, wxID_ANY, title, pos, size) 
	{
		wxMenu *menuFile = new wxMenu;

		menuFile->Append(ID_About, _("&aboutski..."));
		menuFile->AppendSeparator();
		menuFile->Append(ID_Quit, _("E&xit"));

		wxMenuBar *menuBar = new wxMenuBar;
		menuBar->Append(menuFile, _("&File"));

		SetMenuBar(menuBar);

		CreateStatusBar();
		SetStatusText(_("Welcome to wxWidgets!"));
	}

	std::string get_ip_dialog()
	{
		wxTextEntryDialog dialog(this,
			wxT("")
			wxT("Connect to [ip:port]:"),
			wxT("R-PI gpio control"),
			wxT("192.168.1.100:8556"),
			wxOK | wxCANCEL);

		if (dialog.ShowModal() == wxID_OK)
		{
			return dialog.GetValue();
		}
		return "";
	}

	void alert(std::string reason, std::string result)
	{
		wxMessageBox(reason, result, wxOK | wxICON_INFORMATION, this);
	}

	void OnQuit(wxCommandEvent& event) 
	{
		Close(TRUE);
	}

	void OnAbout(wxCommandEvent& event) 
	{
		wxMessageBox(_("This is a wxWidgets Hello world sample"), _("About Hello World"), wxOK | wxICON_INFORMATION, this);
	}

	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(gui_frame, wxFrame)
EVT_MENU(ID_Quit, gui_frame::OnQuit)
EVT_MENU(ID_About, gui_frame::OnAbout)
END_EVENT_TABLE()