#pragma once

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

#include <debugapi.h>

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_BUTTON(10001, OnButtonClicked)
	EVT_BUTTON(10002, FindDeviceClicked)
wxEND_EVENT_TABLE()


cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Bluetooth Switch", wxPoint(30, 30), wxSize(800,600))
{
	bth_man = new bthSwitch();
	m_btn1 = new wxButton(this, 10001, "Click Me", wxPoint(10, 10), wxSize(150, 50));
	m_btnFindDevice = new wxButton(this, 10002, "Find Device", wxPoint(this->GetSize().GetWidth() - 110, 10), wxSize(150,50));
	m_txt1 = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 70), wxSize(300, 30));
	m_list1 = new wxListBox(this, wxID_ANY, wxPoint(10, 110), wxSize(300, 300));
	if (!bth_man->get_wsa_started()) {
		OutputDebugStringA("Window closed due to WSAStartup error.");
		Close(true);
	}
	//printf(std::to_string(WSAGetLastError()).c_str());
}

cMain::~cMain()
{
	delete bth_man;
	Close(true);
}

void cMain::OnButtonClicked(wxCommandEvent& evt)
{
	std::string widthS = std::to_string(this->GetSize().GetWidth());
	const char* widthC = widthS.c_str();
	wxLogDebug(widthC);
	OutputDebugStringA("HELLO WORLD!");
	m_list1->AppendString(m_txt1->GetValue());
	evt.Skip();
}

void cMain::FindDeviceClicked(wxCommandEvent& evt)
{
	// Using new puts data on heap not stack. Need to manually delete to prevent memory leaks.
	/*
	LPWSAQUERYSETW q = new WSAQUERYSETW();
	memset(q, 0, sizeof(WSAQUERYSETW));
	q->dwSize = sizeof(WSAQUERYSETW);
	q->dwNameSpace = NS_BTH;
	
	DWORD dwCtrlFlag = LUP_FLUSHCACHE | LUP_CONTAINERS;
	DWORD dwErr;
	HANDLE lookUp = 0;

	int lookUpStatus = WSALookupServiceBeginW(q, dwCtrlFlag, &lookUp);
	dwErr = WSAGetLastError();
	wxLogDebug(std::to_string(dwErr).c_str());
	
	//LPWSAQUERYSETA bth_query = (LPWSAQUERYSETA)malloc(sizeof(WSAQUERYSETA) + 2000);//new WSAQUERYSETA();
	char* buff = new char[sizeof(WSAQUERYSETW) + 2000];
	LPWSAQUERYSETW bth_query = (WSAQUERYSETW*)buff;
	int error_code = 0;
 	while (1) {
		DWORD nextCtrlFlags = LUP_RETURN_NAME | LUP_RETURN_ADDR | LUP_RETURN_BLOB | LUP_RES_SERVICE | LUP_RETURN_TYPE;
		DWORD size = sizeof(WSAQUERYSETW) + 2000;
		
		memset(bth_query, 0, sizeof(WSAQUERYSETW) + 2000);
		//bth_query->dwNameSpace = NS_BTH;
		//bth_query->dwSize = size;
		error_code = WSALookupServiceNextW(lookUp, nextCtrlFlags, &size, bth_query);
		dwErr = WSAGetLastError();
		wxLogDebug(std::to_string(dwErr).c_str());
		wxLogDebug(std::to_string(error_code).c_str());
		OutputDebugStringA(std::to_string(bth_query->lpServiceClassId->Data1).c_str());

		if (lstrcmpW(bth_query->lpszServiceInstanceName, L"Nintendo Switch") == 0) {
			OutputDebugStringW(L"True");
		}
		else {
			OutputDebugStringW(L"False");
		}
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
	*/

	SOCKADDR_BTH** bth_info = new SOCKADDR_BTH*[2];
	bth_info[0] = new SOCKADDR_BTH;
	bth_info[1] = new SOCKADDR_BTH;
	bth_man->find_joycons(bth_info);

	/*
	if (bth_info[0] != nullptr) {
		delete bth_info[1];
		delete bth_info[0];
	}
	*/
	delete bth_info[0];
	delete bth_info[1];
	delete[] bth_info; //testing memory deletion

	evt.Skip();
	
}
