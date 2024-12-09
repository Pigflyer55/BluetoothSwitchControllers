#pragma once
#include <hidapi.h>

enum class Player {
	Player1,
	Player2,
	Player3,
	Player4
};

enum class ControllerState {
	Connected,
	Disconnected,
	Dropped
};

enum class ControllerSetup {
	Pro,
	TwoJoy,
	LJoy,
	RJoy,
	Nes
};

class Joycon
{
public:
	Joycon(Player playerNum, hid_device* controller, ControllerSetup setup);
	Joycon(Player playerNum, hid_device* leftJoycon, hid_device* rightJoycon);
	~Joycon();

	void setController(hid_device* controller);
	void setOtherController(hid_device* controller);
	void setBatteryLevel(int batteryLevel);
	void setControllerState(ControllerState state);
	void setControllerSetup(ControllerSetup setup);

	hid_device* getController();
	hid_device* getOtherController();
	int getBatteryLevel();
	ControllerState getControllerState();
	ControllerSetup getControllerSetup();

	void disconnectControllers();
	
	

private:
	Player playerNum;
	hid_device* controller;
	hid_device* otherController;
	int batteryLevel;
	ControllerState state;
	ControllerSetup setup;

};

