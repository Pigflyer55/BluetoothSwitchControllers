// TODO
// Determine joy con object class structure
// Add disconnect function
// 
#pragma once

#include "bthSwitch.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cwchar>

#include <debugapi.h>

#include "rumbleData.h"
#include "Joycon.h"

bthSwitch::bthSwitch() {
	// TODO
	// Error handling when initialization fails
	// Determine way for UI to know initialization fails
	// 

	if (!hid_init()) {
		wprintf(hid_error(NULL));
	}

}


bthSwitch::~bthSwitch() {
	for (int x = 0; x < this->players.size(); x++) {
		delete std::get<0>(this->players[x]);
	}
	this->players.clear();

	hid_exit();
}

/*
* @brief Finds and opens a controller that was not open before.
* Consider sending back a serial number instead of opening device automatically
* 
* @param controllers: A linked list containing information about controllers
* 
* @returns A pointer to a hid_device object for a previously not open controller
* 
*/
hid_device* bthSwitch::getDeviceNotOpen(hid_device_info* controllers, unsigned short pid) {
	// Liked list of controllers also contains devices open already
	hid_device_info* curDevInfo = controllers;
	hid_device* unOpenDev = NULL;
	bool open = false;
	OutputDebugStringA("######### MADE IT ########\n");
	// Find a Joy-Con(R) that is not currently open
	// Make sure current device is not one of the open devices
	while (curDevInfo) 
	{
		OutputDebugStringA("### LOOPING ###\n");
		open = false;
		// Check opened device serial number to verify device is not already open
		// Possibly two devices to check, Joycon(L) + Joycon(R)
		for (int x = 0; x < this->players.size(); x++) 
		{
			if (std::get<0>(this->players[x])->getController()) 
			{
				// Compare serial numbers from checked device and current device 
				hid_device_info* openDevInfo = hid_get_device_info(std::get<0>(this->players[x])->getController());

				OutputDebugStringW(curDevInfo->serial_number);

				if (wcscmp(curDevInfo->serial_number, openDevInfo->serial_number) == 0) {
					curDevInfo = curDevInfo->next;
					hid_free_enumeration(openDevInfo);
					open = true;
				}

				// Compare serial numbers to possible other device if previous are not same.
				else if (std::get<0>(this->players[x])->getOtherController()) {
					hid_free_enumeration(openDevInfo);
					openDevInfo = hid_get_device_info(std::get<0>(this->players[x])->getOtherController());
					if (wcscmp(curDevInfo->serial_number, openDevInfo->serial_number) == 0) {
						curDevInfo = curDevInfo->next;
						hid_free_enumeration(openDevInfo);
						open = true;
					}
				}	
			}
			// Device is open, no need to search if current device is open anymore
			if (open) {
				break;
			}
		}
		// Device is not open, so can be used as controller
		if (!open) {
			//
			unOpenDev = hid_open(0x57E, pid, curDevInfo->serial_number);
			break;
		}
	}
	
	return unOpenDev;
}

/**
* @brief Find and open a connection to the Joy-Con(s) not being used
* Consider making function to connect specific joycons?
**/
void bthSwitch::connectJoycons(Player player) {
	// Nintendo VID = 0x57E
	// Joy-Con R PID = 2007
	// Joy-Con L PID = 2006
	// Pro Controller PID = 2009
	// NES Controller PID = ~~~ ADD LATER ~~~

	//hid_device_info* proControllers = hid_enumerate(0x57E, 0x2009);

	hid_device_info* leftJoyCons = hid_enumerate(0x57E, 0x2006);

	hid_device_info* rightJoyCons = hid_enumerate(0x57E, 0x2007);

	// Check if no devices


	//wchar_t* joySerialNum = rightJoyCons->serial_number;
	hid_device_info* rightFirst = rightJoyCons;
	hid_device* right = this->getDeviceNotOpen(rightJoyCons, 0x2007);


	hid_device_info* leftFirst = leftJoyCons;
	hid_device* left = this->getDeviceNotOpen(leftJoyCons, 0x2006);
	
	
	hid_free_enumeration(rightFirst);
	hid_free_enumeration(leftFirst);
	
	// TODO
	// Create Joycon object and append to player array
	//players =
	Joycon* controller = new Joycon(Player::Player4, left, right);
	this->players.push_back(std::make_tuple(controller, player));
}

/**
* @brief Close hid_device to joycon(s) a specific player
* 
* @param playerNum: The number of the player
**/
void bthSwitch::disconnectJoycons(Player playerNum) {
	//hid_device* left = this->players[playerNum];
	for (int x = 0; x < this->players.size(); x++) {
		if (std::get<1>(this->players[x]) == playerNum) {
			std::get<0>(this->players[x])->disconnectControllers();
		}
	}
}

/**
* @brief Return battery level of Joy-Con(s)
* 
* 
* @param[out] bth_info the bluetooth device info needed for socket connection
*/
int* bthSwitch::contBatteryLevel(int player) {
	//Joycon* controller = this->players[player];
	return NULL;
}

void bthSwitch::test(int playerNum) {
	//return 0;
}

