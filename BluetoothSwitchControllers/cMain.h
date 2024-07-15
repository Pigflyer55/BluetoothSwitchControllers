#pragma once

#include "wx/wx.h"

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

public:
	wxButton *m_btn1 = nullptr;
	wxButton* m_btnFindDevice = nullptr;
	wxTextCtrl* m_txt1 = nullptr;
	wxListBox* m_list1 = nullptr;
	LPWSADATA lpWSAData = nullptr;

	void OnButtonClicked(wxCommandEvent &evt);
	void FindDeviceClicked(wxCommandEvent& evt);
	wxDECLARE_EVENT_TABLE();
};

