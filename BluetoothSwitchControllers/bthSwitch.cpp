#include "bthSwitch.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include <initguid.h>
#include <bthsdpdef.h>
#include <bthdef.h>
#include <bluetoothapis.h>

#include <debugapi.h>

bthSwitch::bthSwitch() {
	joy_con_r_socket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_L2CAP); //BTHPROTO_RFCOMM might only work instead of BTHPROTO_L2CAP? SOCK_SEQPACKET
	joy_con_l_socket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_L2CAP); 

	
	WORD versionWinSock = MAKEWORD(2, 2);
	lpWSAData = new WSADATA;
	int out = WSAStartup(versionWinSock, lpWSAData);
	//printf(std::to_string(lpWSAData->wVersion).c_str());
	if (out != 0) {
		OutputDebugStringA("No usable WINSOCK DLL.");
		wsa_started = false;
		//Close(true);
	}
	if (LOBYTE(lpWSAData->wVersion) != 2 || HIBYTE(lpWSAData->wVersion) != 2) {
		OutputDebugStringA("No usable WINSOCK DLL");
		wsa_started = false;
		WSACleanup();
	}
}

bthSwitch::~bthSwitch() {
	if (WSACleanup() == 0) {
		OutputDebugStringA("Socket Disconnected!");
	}
	else {
		OutputDebugStringA(std::to_string(WSAGetLastError()).c_str());
	}
	delete this->lpWSAData;
	closesocket(joy_con_l_socket);
	closesocket(joy_con_r_socket);
}

bool bthSwitch::get_wsa_started() {
	return wsa_started;
}

/**
* 
* 
* 
*/
void bthSwitch::connect_joycons(SOCKADDR_BTH* joy_l, SOCKADDR_BTH* joy_r) {
	int error_code1 = connect(this->joy_con_l_socket, (SOCKADDR*)joy_l, sizeof(joy_l));
	int error_code2 = connect(this->joy_con_r_socket, (SOCKADDR*)joy_r, sizeof(joy_r));
	if (error_code1 == SOCKET_ERROR) {
		OutputDebugStringA(("Error code: " + std::to_string(WSAGetLastError()) + " for socket joy_con_l").c_str());
	}
	if (error_code2 == SOCKET_ERROR) {
		OutputDebugStringA(("Error code: " + std::to_string(WSAGetLastError()) + " for socket joy_con_r").c_str());
	}
}

/**
* Return SOCKETADDR_BTH for Joy-Con(L) and Joy-Con(R)
* 
* 
* @param[out] bth_info the bluetooth device info needed for socket connection
*/
int bthSwitch::find_joycons(SOCKADDR_BTH** bth_info) {
	LPWSAQUERYSETW q = new WSAQUERYSETW();
	memset(q, 0, sizeof(WSAQUERYSETW));
	q->dwSize = sizeof(WSAQUERYSETW);
	q->dwNameSpace = NS_BTH;

	DWORD dwCtrlFlag = LUP_FLUSHCACHE | LUP_CONTAINERS;
	DWORD dwErr;
	HANDLE lookUp = 0;

	int lookUpStatus = WSALookupServiceBeginW(q, dwCtrlFlag, &lookUp);
	dwErr = WSAGetLastError();

	/* 
	bth_query might get data that is bigger than sizeof(WSAQUERYSETA)
	Extra memory is given to account for this
	*/
	char* buff = new char[sizeof(WSAQUERYSETW) + 2000];
	LPWSAQUERYSETW bth_query = (WSAQUERYSETW*)buff;
	int error_code = 0;

	while (1) {
		DWORD nextCtrlFlags = LUP_RETURN_NAME | LUP_RETURN_ADDR | LUP_RETURN_BLOB | LUP_RES_SERVICE | LUP_RETURN_TYPE;
		DWORD size = sizeof(WSAQUERYSETW) + 2000;

		memset(bth_query, 0, sizeof(WSAQUERYSETW) + 2000);
		error_code = WSALookupServiceNextW(lookUp, nextCtrlFlags, &size, bth_query);
		dwErr = WSAGetLastError();
		if (dwErr == WSA_NOT_ENOUGH_MEMORY) {
			OutputDebugStringA("Not enough memory for query");
			break;
		}

		OutputDebugStringA(std::to_string(dwErr).c_str());
		OutputDebugStringA(std::to_string(error_code).c_str());
		if (error_code == SOCKET_ERROR) {
			if (dwErr == WSAEFAULT) {
				OutputDebugStringA("Buffer too small");
				break;
			}
			else if (dwErr == WSA_E_NO_MORE || dwErr == WSAENOMORE || lookUp == NULL) {
				break;
			}
		}

		// Automatically get Joy Con information. Later might add manual bluetooth device selection.
		if (lstrcmpW(bth_query->lpszServiceInstanceName, L"Joy-Con (R)") == 0) {
			OutputDebugStringW(L"Joy-Con (R) found");
			//SOCKADDR_BTH* joy_addr = new SOCKADDR_BTH();
			PSOCKADDR_BTH mac_address = (PSOCKADDR_BTH)bth_query->lpcsaBuffer->RemoteAddr.lpSockaddr;
			bth_info[1]->addressFamily = AF_BTH;
			bth_info[1]->btAddr = mac_address->btAddr;
			bth_info[1]->port = 0;
			bth_info[1]->serviceClassId = mac_address->serviceClassId;

			//bth_info[1] = joy_addr; // This is hardcoded, in the future replace this for dynamic
			int asda = 0;
		} else if (lstrcmpW(bth_query->lpszServiceInstanceName, L"Joy-Con (L)") == 0){
			OutputDebugStringW(L"Joy-Con(L) found");
			//SOCKADDR_BTH* joy_addr = new SOCKADDR_BTH();
			PSOCKADDR_BTH mac_address = (PSOCKADDR_BTH)bth_query->lpcsaBuffer->RemoteAddr.lpSockaddr;
			bth_info[0]->addressFamily = AF_BTH;
			bth_info[0]->btAddr = mac_address->btAddr;
			bth_info[0]->port = 0;
			bth_info[0]->serviceClassId = mac_address->serviceClassId;

			//bth_info[0] = joy_addr; // This is hardcoded, in the future replace this for dynamic
		}

	}

	// End WSA service and release memory
	WSALookupServiceEnd(lookUp);
	delete[] bth_query;
	delete q;
	return 0;
}