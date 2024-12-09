#include "Joycon.h"

Joycon::Joycon(Player num, hid_device* cont, ControllerSetup inputSetup) {
	playerNum = num;
	controller = cont;
	setup = inputSetup;
	otherController = NULL;

}

//
Joycon::Joycon(Player num, hid_device* leftJoycon, hid_device* rightJoycon) {
	playerNum = num;
	controller = leftJoycon;
	otherController = rightJoycon;
	setup = ControllerSetup::TwoJoy;
	

}

hid_device* Joycon::getController() {
	return this->controller;
}

hid_device* Joycon::getOtherController() {
	return this->otherController;
}

