// Using new puts data on heap not stack. Need to manually delete to prevent memory leaks.

#pragma once

#include "cMain.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include <windows.h>

#include <debugapi.h>

#include <wx/stdpaths.h>
/*
wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_BUTTON(10001, OnButtonClicked)
	EVT_BUTTON(10002, FindDeviceClicked)
	EVT_BUTTON(10003, DisconnectJoycon)
wxEND_EVENT_TABLE()
*/

imagePanel::imagePanel(wxWindow* parent, wxString file, wxBitmapType format) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(100,100)) {
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
	//sizer = new wxBoxSizer();
	wxJPEGHandler* jpgHandler = new wxJPEGHandler();
	image.AddHandler(jpgHandler);
	image.LoadFile(file, format);
	//image.Rescale(100, 100, wxIMAGE_QUALITY_HIGH);
	//image.Rescale(this->GetSize().GetWidth(), this->GetSize().GetHeight());
	
	Bind(wxEVT_PAINT, &imagePanel::paintEvent, this);
	Bind(wxEVT_SIZE, &imagePanel::onSize, this);

	
}

void imagePanel::paintEvent(wxPaintEvent &evt) {
	wxBufferedPaintDC dc(this);
	render(dc);
	evt.Skip();
}

void imagePanel::paintNow() {
	wxClientDC dc(this);
	render(dc);
}

void imagePanel::onSize(wxSizeEvent& evt) {
	Refresh();
	evt.Skip();
}

void imagePanel::render(wxDC& dc) {
	if (!dc.IsOk()) {
		OutputDebugStringA("########################## Not ok ###########################");
	}
	wxBitmap newBitmap(image.Scale(this->GetSize().GetWidth(), this->GetSize().GetHeight()));
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	//dc.SetUserScale(this->GetSize().GetWidth(), this->GetSize().GetHeight());
	dc.DrawBitmap(newBitmap, 0, 0, false);
}


controllerOrderPanel::controllerOrderPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(200,100)) {
	this->SetWindowStyle(wxFULL_REPAINT_ON_RESIZE);
	this->SetBackgroundColour(wxColour(200,20,20));
	m_btnDisconnect = new wxButton(this, wxID_ANY, "Disconnect");
	m_btnConnect = new wxButton(this, wxID_ANY, "Connect");
	m_sizer = new wxBoxSizer(wxVERTICAL);
	//m_imgController = new imagePanel(this, wxString("C:/Users/SAMSAUNG/source/repos/BluetoothSwitchControllers/eeyore-smile.jpg"), wxBITMAP_TYPE_JPEG);

	//wxString path = wxStandardPaths::GetExecutablePath();

	//wxBitmap* b = new wxBitmap(*m_imgController);
	imPan = new imagePanel(this, wxString("C:/Users/SAMSAUNG/source/repos/BluetoothSwitchControllers/joycon.jpg"), wxBITMAP_TYPE_JPEG);

	m_sizer->Add(imPan, 1, wxEXPAND);
	m_sizer->Add(m_btnDisconnect, 1, wxEXPAND);
	m_sizer->Add(m_btnConnect, 1, wxEXPAND);
	this->SetSizer(m_sizer);
	
}

controllerOrderPanel::~controllerOrderPanel() {
	//Close(true);
}

void controllerOrderPanel::OnDisconnectController(wxCommandEvent &evt) {
	
}

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Bluetooth Switch", wxPoint(30, 30), wxSize(800,600))
{
	// m_mainSizer vertically holds top with connection stuff, bottom panel for other stuff?
	// 
	bth_man = new bthSwitch();
	//m_btn1 = new wxButton(this, 10001, "Click Me", wxPoint(10, 10), wxSize(150, 50));
	m_btnFindDevice = new wxButton(this, 10002, "Find Device", wxPoint(this->GetSize().GetWidth() - 200, 10), wxSize(150,50));
	m_btnDisconnect = new wxButton(this, 10003, "Disconnect", wxPoint(m_btnFindDevice->GetPosition().x, m_btnFindDevice->GetPosition().y + 60), wxSize(150, 50));
	//m_txt1 = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 70), wxSize(300, 30));
	//m_list1 = new wxListBox(this, wxID_ANY, wxPoint(10, 110), wxSize(300, 300));
	
	m_topPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition);
	m_bottomPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(0,20));
	m_contPanel = new controllerOrderPanel(m_topPanel);
	m_mainSizer = new wxBoxSizer(wxVERTICAL);
	m_topSizer = new wxBoxSizer(wxHORIZONTAL);
	
	m_topPanel->SetBackgroundColour(wxColour(20,20,100));
	m_bottomPanel->SetBackgroundColour(wxColour(20, 100, 20));
	m_mainSizer->Add(m_topPanel, 1, wxEXPAND);
	m_mainSizer->Add(m_bottomPanel, 0, wxEXPAND);
	m_topPanel->SetSizer(m_topSizer);
	
	m_topSizer->AddStretchSpacer();
	m_topSizer->Add(m_contPanel, 0, wxEXPAND | wxALL);
	m_topSizer->AddStretchSpacer();
	m_topPanel->SetSizer(m_topSizer);

	this->SetSizer(m_mainSizer);
	this->Layout();
	m_topPanel->Layout();
	//this->SetSizerAndFit(m_mainSizer);
	//m_mainSizer->AddStretchSpacer(1);

	
	if (!bth_man) {
		OutputDebugStringA("Window closed due to WSAStartup error.");
		Close(true);
	}
	//m_btn1->Bind(wxEVT_BUTTON, &cMain::OnButtonClicked, this, m_btn1->GetId());
	m_btnDisconnect->Bind(wxEVT_BUTTON, &cMain::DisconnectJoycon, this, m_btnDisconnect->GetId());
	m_btnFindDevice->Bind(wxEVT_BUTTON, &cMain::FindDeviceClicked, this, m_btnFindDevice->GetId());

	this->SetMinSize(wxSize(this->GetSize().GetWidth(), this->GetSize().GetHeight()));
	//printf(std::to_string(WSAGetLastError()).c_str());
}

cMain::~cMain()
{
	//wxDELETE(m_contPanel);
	//m_contPanel->DestroyChildren();
	delete bth_man;
	Close(true);
}

/*
void cMain::OnButtonClicked(wxCommandEvent& evt)
{
	std::string widthS = std::to_string(this->GetSize().GetWidth());
	const char* widthC = widthS.c_str();
	wxLogDebug(widthC);
	OutputDebugStringA("HELLO WORLD!");
	m_list1->AppendString(m_txt1->GetValue());
	evt.Skip();
}
*/

void cMain::FindDeviceClicked(wxCommandEvent& evt)
{
	bth_man->connectJoycons();
	evt.Skip();
	
}

void cMain::DisconnectJoycon(wxCommandEvent& evt) {
	evt.Skip();
}