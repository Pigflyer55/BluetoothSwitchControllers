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
	int find_joycons();
	void connect_joycons(BTH_ADDR joy_l, BTH_ADDR joy_r);
	void create_socket();

private:
	LPWSADATA lpWSAData = nullptr;
	SOCKET joy_con_r_socket = NULL;
	SOCKET joy_con_l_socket = NULL;

};
