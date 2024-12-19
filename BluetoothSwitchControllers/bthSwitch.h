#pragma once

#include <iostream>

#include <windows.h>
#include <tuple>
#include <vector>
#include <thread>
#include <hidapi.h>
#include "Joycon.h"

class bthSwitch
{
public:
	//Consider making connect and disconnect int for error code

	bthSwitch();
	~bthSwitch();
	void connectJoycons(Player player);
	void disconnectJoycons(Player player);
	void disconnectOneJoycon(int player, bool left);
	int* contBatteryLevel(int player);
	int changeKey();

	void test(int playerNum);

// Future functions
// 	   keyboardMapping() get a file containing controller to key mappings
//     controllerDisplay() to display coloured controller on screen
//     selectController() allow user to enumerate through list of available controllers

private:
	// unsigned short pid param
	hid_device* getDeviceNotOpen(hid_device_info* controllers, unsigned short pid);

	//std::vector<hid_device*> devices;
	//Joycon* players [4];
	std::vector<std::tuple<Joycon*, Player>> players;
};

