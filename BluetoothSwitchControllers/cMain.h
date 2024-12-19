#pragma once

#include "wx/wx.h"
#include "bthSwitch.h"
#include <wx/dcbuffer.h>



class imagePanel : public wxPanel 
{
public:
	// format parameter might be redundant if all images are the same format
	imagePanel(wxWindow* parent, wxString file, wxBitmapType format);
	void changeImage(wxString file, wxBitmapType format);
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
	//controllerOrderPanel(wxWindow* parent, wxString file1, wxString file2)
	~controllerOrderPanel();
	void changeImage(wxString file, wxBitmapType format);
	void changeImage(wxString file1, wxString file2, wxBitmapType format);

	

private:
	wxButton* m_btnDisconnect = nullptr;
	wxButton* m_btnConnect = nullptr;
	imagePanel* m_imgController = nullptr;
	wxBoxSizer* m_sizer = nullptr;
	imagePanel* imPan = nullptr;

	Player playerNum;
	//bthSwitch* bthManager;

	void OnDisconnectController(wxCommandEvent& evt);
	void connectController(wxCommandEvent& evt);
	void disconnectController(wxCommandEvent& evt);

};

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();
	bthSwitch* bth_man = nullptr;


private:
	//wxButton *m_btn1 = nullptr;
	wxButton* m_btnFindDevice = nullptr;
	wxButton* m_btnDisconnect = nullptr;
	//wxTextCtrl* m_txt1 = nullptr;
	//wxListBox* m_list1 = nullptr;
	
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


