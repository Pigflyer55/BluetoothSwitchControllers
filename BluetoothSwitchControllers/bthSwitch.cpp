#include "bthSwitch.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include <debugapi.h>

#include "Joycon.h"

bthSwitch::bthSwitch() {
	if (!hid_init()) {
		wprintf(hid_error(NULL));
	}

}

bthSwitch::~bthSwitch() {
	for (int x = 0; x < this->devices.size(); x++) {
		hid_close(this->devices[x]);
	}
	this->devices.clear();

	hid_exit();
}

/*
* @brief Finds and opens a controller that was not open before
* 
* @param controllers: A linked list containing information about controllers
* 
* @returns A pointer to a hid_device object for a previously not open controller
* 
*/
hid_device* bthSwitch::getDeviceNotOpen(hid_device_info* controllers) {
	hid_device_info* curDevInfo = controllers;
	hid_device* unOpenDev = NULL;
	bool open = false;

	// Find a Joy-Con(R) that is not currently open
	while (curDevInfo) {
		open = false;
		// Check opened device serial number to verify device is not already open
		for (int x = 0; x < this->devices.size(); x++) {
			hid_device_info* openDevInfo = hid_get_device_info(this->devices[x]);
			if (curDevInfo->serial_number == openDevInfo[x].serial_number) {
				curDevInfo = curDevInfo->next;
				open = true;
			}
			hid_free_enumeration(openDevInfo);
			// Device is open, no need to search if this device is open anymore
			if (open) {
				break;
			}
		}
		// Device is not open, so can be used as controller
		if (!open) {
			unOpenDev = hid_open(0x57E, 0x2007, curDevInfo->serial_number);
			this->devices.push_back(unOpenDev);
			break;
		}
	}

	return unOpenDev;
}

/**
* @brief Find and open a connection to the Joy-Con(s) not being used
**/
void bthSwitch::connectJoycons() {
	// Nintendo VID = 0x57E
	// Joy-Con R PID = 2007
	// Joy-Con L PID = 2006
	// Pro Controller PID = 2009

	//hid_device_info* proControllers = hid_enumerate(0x57E, 0x2009);

	hid_device_info* leftJoyCons = hid_enumerate(0x57E, 0x2006);

	hid_device_info* rightJoyCons = hid_enumerate(0x57E, 0x2007);

	// Check if no devices


	//wchar_t* joySerialNum = rightJoyCons->serial_number;
	hid_device_info* rightFirst = rightJoyCons;
	this->getDeviceNotOpen(rightJoyCons);

	hid_device_info* leftFirst = leftJoyCons;
	this->getDeviceNotOpen(leftJoyCons);
	
	
	hid_free_enumeration(rightFirst);
	hid_free_enumeration(leftFirst);
	
	// Create Joycon object and append to player array
	//players =

}



/**
* @brief Return battery level of Joy-Con(s)
* 
* 
* @param[out] bth_info the bluetooth device info needed for socket connection
*/
int* bthSwitch::contBatteryLevel(int player) {
	Joycon* controller = this->players[player];
	return NULL;
}