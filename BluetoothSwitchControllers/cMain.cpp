#include "cMain.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include <WinSock2.h>
#include <ws2bth.h>
#include <windows.h>
#include <initguid.h>
#include <bthsdpdef.h>
#include <bthdef.h>
#include <bluetoothapis.h>


using namespace std;

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_BUTTON(10001, OnButtonClicked)
	EVT_BUTTON(10002, FindDeviceClicked)
wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Bluetooth Switch", wxPoint(30, 30), wxSize(800,600))
{
	m_btn1 = new wxButton(this, 10001, "Click Me", wxPoint(10, 10), wxSize(150, 50));
	m_btnFindDevice = new wxButton(this, 10002, "Find Device", wxPoint(this->GetSize().GetWidth() - 110, 10), wxSize(150,50));
	m_txt1 = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 70), wxSize(300, 30));
	m_list1 = new wxListBox(this, wxID_ANY, wxPoint(10, 110), wxSize(300, 300));
	lpWSAData = new WSADATA;
	WORD v = MAKEWORD(2,2);
	int out = WSAStartup(0x202, lpWSAData);
	if (out != 0) {
		wxLogDebug("Error trying to startup WSA.");
	}
	wxLogDebug(to_string(WSAGetLastError()).c_str());
}

cMain::~cMain()
{
	if (WSACleanup() == 0) {
		wxLogDebug("Socket Disconnected!");
	}
	else {
		printf(to_string(WSAGetLastError()).c_str());
	}
	delete this->lpWSAData;
}

void cMain::OnButtonClicked(wxCommandEvent& evt)
{
	string widthS = to_string(this->GetSize().GetWidth());
	const char* widthC = widthS.c_str();
	wxLogDebug(widthC);
	wxLogDebug("HELLO WORLD!");
	m_list1->AppendString(m_txt1->GetValue());
	evt.Skip();
}

void cMain::FindDeviceClicked(wxCommandEvent& evt)
{
	// Using new puts data on heap not stack. Need to manually delete to prevent memory leaks.
	LPWSAQUERYSETA q = new WSAQUERYSETA();
	memset(q, 0, sizeof(WSAQUERYSETA));
	q->dwSize = sizeof(WSAQUERYSETA);
	q->dwNameSpace = NS_BTH;
	
	DWORD dwCtrlFlag = LUP_FLUSHCACHE | LUP_CONTAINERS;
	DWORD dwErr;
	HANDLE lookUp = 0;

	int lookUpStatus = WSALookupServiceBeginA(q, dwCtrlFlag, &lookUp);
	dwErr = WSAGetLastError();
	wxLogDebug(to_string(dwErr).c_str());
	
	//LPWSAQUERYSETA bth_query = (LPWSAQUERYSETA)malloc(sizeof(WSAQUERYSETA) + 2000);//new WSAQUERYSETA();
	char* buff = new char[sizeof(WSAQUERYSETA) + 2000];
	LPWSAQUERYSETA bth_query = (WSAQUERYSETA*)buff;
	int error_code = 0;
	while (1) {
		DWORD nextCtrlFlags = LUP_RETURN_NAME | LUP_RETURN_ADDR | LUP_RETURN_BLOB | LUP_RES_SERVICE;
		DWORD size = sizeof(WSAQUERYSETA) + 2000;
		
		memset(bth_query, 0, sizeof(WSAQUERYSETA) + 2000);
		//bth_query->dwNameSpace = NS_BTH;
		//bth_query->dwSize = size;
		error_code = WSALookupServiceNextA(lookUp, nextCtrlFlags, &size, bth_query);
		dwErr = WSAGetLastError();
		wxLogDebug(to_string(dwErr).c_str());
		wxLogDebug(to_string(error_code).c_str());
		if (error_code == SOCKET_ERROR) {
			if (dwErr == WSAEFAULT) {
				printf("Buffer too small");
				break;
			}
			else if (dwErr == WSA_E_NO_MORE || dwErr == WSAENOMORE || lookUp == NULL) {
				break;
			}
		}
	}
	WSALookupServiceEnd(lookUp);
	delete[] bth_query;
	delete q;
	evt.Skip();
	
}
