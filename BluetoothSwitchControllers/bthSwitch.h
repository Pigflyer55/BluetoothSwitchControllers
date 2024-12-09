#pragma once

#include <iostream>

#include <windows.h>
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
	void connectJoycons();
	void disconnectJoycons();
	int* contBatteryLevel(int player);
	int changeKey();


// Future functions
// 	   keyboardMapping() get a file containing controller to key mappings
//     controllerDisplay() to display coloured controller on screen
//     selectController() allow user to enumerate through list of available controllers

private:
	hid_device* getDeviceNotOpen(hid_device_info* controllers);

	std::vector<hid_device*> devices;
	Joycon* players [4];
	
};
