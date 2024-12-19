#pragma once
#include <hidapi.h>
#include <thread>


enum class Player {
	Player1,
	Player2,
	Player3,
	Player4
};

enum class CtrlState {
	Connected,
	Disconnected,
	Dropped
};

enum class CtrlSetup {
	Pro,
	TwoJoy,
	LJoy,
	RJoy,
	Nes
};

class Joycon
{
public:
	Joycon(Player playerNum, hid_device* controller, CtrlSetup setup);
	Joycon(Player playerNum, hid_device* leftJoycon, hid_device* rightJoycon);
	~Joycon();

	void setController(hid_device* controller, CtrlSetup newSetup, int Ctrl);
	void setBatteryLevel(int batteryLevel);
	void setState(CtrlState state);
	void setSetup(CtrlSetup setup);
	void setPlayerNum(Player playerNum);

	hid_device* getController();
	hid_device* getOtherController();
	int getBatteryLevel();
	CtrlState getControllerState();
	CtrlSetup getControllerSetup();

	void disconnectControllers();
	void removeController(bool left);

	// Report functions
	void playerLightCmd();
	void controllerColourCmd();

	void readJoycons();

private:
	Player playerNum;
	hid_device* controller;
	hid_device* otherController;

	CtrlState state;
	CtrlSetup setup;

	int batteryLevel;
	int globalPacket;

	void prepareReport(float freq, float amp, unsigned char* buffer);
	bool sendReport();
	void timeout();

	bool threadOn;
	std::thread t1;
};

