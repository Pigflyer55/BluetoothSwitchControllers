#pragma once

#include "wx/wx.h"
//#include "bthSwitch.h"

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

private:
	wxButton *m_btn1 = nullptr;
	wxButton* m_btnFindDevice = nullptr;
	wxTextCtrl* m_txt1 = nullptr;
	wxListBox* m_list1 = nullptr;
	LPWSADATA lpWSAData = nullptr;
	//bthSwitch* bth;

	void OnButtonClicked(wxCommandEvent& evt);
	void FindDeviceClicked(wxCommandEvent& evt);
	wxDECLARE_EVENT_TABLE();
};

