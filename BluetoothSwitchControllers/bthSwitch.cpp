#include "bthSwitch.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <initguid.h>
#include <bthsdpdef.h>
#include <bthdef.h>
#include <bluetoothapis.h>

#include <debugapi.h>

bthSwitch::bthSwitch() {

	joy_con_r_socket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_L2CAP); //BTHPROTO_RFCOMM might only work instead of BTHPROTO_L2CAP?
	joy_con_l_socket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_L2CAP);
	
	WORD versionWinSock = MAKEWORD(2, 2);
	lpWSAData = new WSADATA;
	int out = WSAStartup(versionWinSock, lpWSAData);
	//printf(std::to_string(lpWSAData->wVersion).c_str());
	if (out != 0) {
		OutputDebugStringA("No usable WINSOCK DLL.");
		//Close(true);
	}
	if (LOBYTE(lpWSAData->wVersion) != 2 || HIBYTE(lpWSAData->wVersion) != 2) {
		OutputDebugStringA("No usable WINSOCK DLL");
		WSACleanup();
		//Close(true);
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
}

/**
* 
* 
* 
*/
void bthSwitch::connect_joycons(BTH_ADDR joy_l, BTH_ADDR joy_r) {

	int p = 0;
}

void bthSwitch::create_socket() {

}

/**
* Return BthAddress of Joy-Con(L) and Joy-Con(R)
* 
* 
* 
*/
int bthSwitch::find_joycons() {
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


	}

	// End WSA service and release memory
	WSALookupServiceEnd(lookUp);
	delete[] bth_query;
	delete q;
	return 0;
}