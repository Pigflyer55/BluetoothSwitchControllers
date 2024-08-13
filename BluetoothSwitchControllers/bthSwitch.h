#pragma once

#include <iostream>

#include <WinSock2.h>
#include <ws2bth.h>
#include <windows.h>


class bthSwitch
{
public:
	bthSwitch();
	~bthSwitch();
	int find_joycons(SOCKADDR_BTH** bth_info);
	void connect_joycons(SOCKADDR_BTH* joy_l, SOCKADDR_BTH* joy_r);
	bool get_wsa_started();

private:
	LPWSADATA lpWSAData = nullptr;
	SOCKET joy_con_r_socket = NULL;
	SOCKET joy_con_l_socket = NULL;
	bool wsa_started = true;
};
