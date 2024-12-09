#pragma once

#include "wx/wx.h"
#include "bthSwitch.h"
#include <wx/dcbuffer.h>

class imagePanel : public wxPanel 
{
public:
	// format parameter might be redundant if all images are the same format
	imagePanel(wxWindow* parent, wxString file, wxBitmapType format);
	void paintEvent(wxPaintEvent& evt);
	void paintNow();
	void onSize(wxSizeEvent& evt);
	void render(wxDC& dc);
private:
	wxImage image;
	//wxBoxSizer sizer;
};	

class controllerOrderPanel : public wxPanel
{
public:
	controllerOrderPanel(wxWindow* parent);
	~controllerOrderPanel();

private:
	wxButton* m_btnDisconnect = nullptr;
	wxButton* m_btnConnect = nullptr;
	imagePanel* m_imgController = nullptr;
	wxStaticBitmap* m_staticBitmap = nullptr;
	wxBoxSizer* m_sizer = nullptr;
	imagePanel* imPan = nullptr;
	int playerNum = 0;

	void OnDisconnectController(wxCommandEvent& evt);
	void connectController();
	void disconnectController();
};

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

private:
	//wxButton *m_btn1 = nullptr;
	wxButton* m_btnFindDevice = nullptr;
	wxButton* m_btnDisconnect = nullptr;
	//wxTextCtrl* m_txt1 = nullptr;
	//wxListBox* m_list1 = nullptr;
	bthSwitch* bth_man = nullptr;
	wxBoxSizer* m_mainSizer = nullptr;
	wxBoxSizer* m_topSizer = nullptr;
	wxPanel* m_topPanel = nullptr;
	wxPanel* m_bottomPanel = nullptr;
	controllerOrderPanel* m_contPanel = nullptr;

	//void OnButtonClicked(wxCommandEvent& evt);
	void FindDeviceClicked(wxCommandEvent& evt);
	void DisconnectJoycon(wxCommandEvent& evt);
	//wxDECLARE_EVENT_TABLE();
};


