#pragma once
#include <wx/wx.h>
#include "Joycon.h"
#include "rumbleData.h"
#include <cmath>
#include <chrono>
#include <iostream>
//#include <thread>
#include <mutex>
#include <string>
using namespace std::chrono_literals;
std::mutex m;
//	
// TODO:
// In constructor, send message to controllers to use Standard Input Reports
// setController
// ~Joycon
//

/**
* 
**/
Joycon::Joycon(Player num, hid_device* cont, CtrlSetup inputSetup) {
	this->threadOn = true;

	this->playerNum = num;
	this->controller = cont;
	this->setup = inputSetup;
	this->otherController = nullptr;
	this->globalPacket = 0;

	// Set report mode to standard input
	unsigned char buffer[31] = {};
	this->prepareReport(41.0, 0.45, buffer);
	buffer[10] = 0x03;
	buffer[11] = 0x30;
	hid_write(cont, buffer, 31);

	playerLightCmd();
}

//
Joycon::Joycon(Player num, hid_device* leftJoycon, hid_device* rightJoycon) {
	this->threadOn = true;

	this->playerNum = num;
	this->controller = leftJoycon;
	this->otherController = rightJoycon;
	this->setup = CtrlSetup::TwoJoy;
	this->globalPacket = 0;

	// ###############################################
	/*
	rumble freqAmps = joyRumbleFreqAmp(100.0, 0.45);
	uint16_t hf = freqAmps.hf;
	uint16_t hfAmp = freqAmps.hfAmp;
	uint8_t lf = freqAmps.lf;
	uint8_t lfAmp = freqAmps.lfAmp;

	unsigned char buf[31] = {};
	buf[0] = 0x01;
	buf[1] = this->globalPacket;
	buf[2] = hf & 0xFF;
	buf[3] = hfAmp + ((hf >> 8) & 0xFF);
	buf[4] = lf + ((lfAmp >> 8) & 0xFF);
	buf[5] = lfAmp & 0xFF;
	buf[6] = hf & 0xFF;
	buf[7] = hfAmp + ((hf >> 8) & 0xFF);
	buf[8] = lf + ((lfAmp >> 8) & 0xFF);
	buf[9] = lfAmp & 0xFF;
	buf[10] = 0x30;
	buf[11] = 0x08;

	hid_write(leftJoycon, buf, 32);
	hid_write(rightJoycon, buf, 32);
	*/
	// ################################################

	// Set report mode to standard input

	

	unsigned char buffer[31] = {};
	this->prepareReport(600.0, 0.0, buffer);
	buffer[10] = 0x48;
	buffer[11] = 0x01;
	for (int i = 0; i < 5; i++) {
		hid_write(leftJoycon, buffer, 31);
		hid_write(rightJoycon, buffer, 31);
	}
	
	buffer[10] = 0x03;
	buffer[11] = 0x30;
	for (int i = 0; i < 5; i++) {
		hid_write(leftJoycon, buffer, 31);
		//unsigned char c[2] = { length, 0 };
		//OutputDebugStringA((LPCSTR)c);
		hid_write(rightJoycon, buffer, 31);
	}
	
	
	

	playerLightCmd();
	int l = 0;
	/*
	const auto start = std::chrono::high_resolution_clock::now();
	std::this_thread::sleep_for(5000ms);
	const auto end = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double, std::milli> elapsed = end - start;
	*/
}

Joycon::~Joycon() {
	m.lock();
	this->threadOn = false;
	m.unlock();

	t1.join();
	disconnectControllers();
}

/**
*
* @brief All hid device objects are closed and Joy-Con(s) turned off
* 
**/
void Joycon::disconnectControllers() {
	if (this->setup == CtrlSetup::TwoJoy) {
		hid_close(this->otherController);
	}
	hid_close(this->controller);
	
}

/**
* 
* @brief Remove a controller from a two Joy-con setup
* 
* @param True to remove left controller, False to remove right controller
* 
**/
void Joycon::removeController(bool left) {
	if (this->setup == CtrlSetup::TwoJoy) {
		if (left) {
			hid_close(this->controller);
			this->controller = this->otherController;
			this->setup = CtrlSetup::RJoy;
		}
		else {
			hid_close(this->otherController);
			this->setup = CtrlSetup::LJoy;
		}
		this->otherController = nullptr;
	}
}

void Joycon::setController(hid_device* controller, CtrlSetup newSetup, int Ctrl = -1){
	//
}

void Joycon::setPlayerNum(Player playerNum) {
	this->playerNum = playerNum;
	playerLightCmd();
}

hid_device* Joycon::getController() {
	return this->controller;
}

hid_device* Joycon::getOtherController() {
	return this->otherController;
}

/**
* 
* @brief Process for reading input reports from a given hid device.
* 
* @param 
* 
**/
void Joycon::readJoycons() {
	m.lock();
	while (this->threadOn) {
		m.unlock();

		unsigned char buffer[31];
		buffer[0] = 0x30;
		hid_read(this->controller, buffer, 31);



		m.lock();
	}
	m.unlock();
}

/**
* In the future, maybe prepare different kinds of output reports
* @brief Creates template for an output report. Delete report array when finished
* 
* @param freq: Frequency of controller rumble
* @param amp: Amplitude of controller rumble. 0 for no rumble
* 
* @return A memory allocated unsigned char array
* 
**/
void Joycon::prepareReport(float freq, float amp, unsigned char* buf) {
	rumble freqAmps = joyRumbleFreqAmp(freq, 0.0);
	uint16_t hf = freqAmps.hf;
	uint16_t hfAmp = freqAmps.hfAmp;
	uint8_t lf = freqAmps.lf;
	uint8_t lfAmp = freqAmps.lfAmp;
	
	buf[0] = 0x01;
	buf[1] = this->globalPacket;
	buf[2] = hf & 0xFF;
	buf[3] = hfAmp + ((hf >> 8) & 0xFF);
	buf[4] = lf + ((lfAmp >> 8) & 0xFF);
	buf[5] = lfAmp & 0xFF;
	buf[6] = hf & 0xFF;
	buf[7] = hfAmp + ((hf >> 8) & 0xFF);
	buf[8] = lf + ((lfAmp >> 8) & 0xFF);
	buf[9] = lfAmp & 0xFF;
	// Pos 10 for Cmd id
	// Pos 11- for Cmd data
}

/**
* 
* @brief Sends report to controllers to turn on specified lights
* 
**/
void Joycon::playerLightCmd() {
	unsigned char buffer[31] = {};
	this->prepareReport(120.0, 0.3, buffer);
	buffer[10] = 0x30;
	if (playerNum == Player::Player1) {
		buffer[11] = 0x01;
	}
	else if (playerNum == Player::Player2) {
		buffer[11] = 0x02;
	}
	else if (playerNum == Player::Player3) {
		buffer[11] = 0x04;
	}
	else {
		buffer[11] = 0x08;
	}
	
	bool cont = false;
	bool otherCont = false;
	unsigned char readBuffer[31];
	int loops = 0;
	while (!cont || !otherCont) {
		loops++;
		if (!cont) {
			hid_write(this->controller, buffer, 31);
			for (int x = 0; x < 40; x++) {
				hid_read(this->controller, readBuffer, 31);
				if (readBuffer[14] == 0x30) {
					cont = true;
					break;
				}
			}
		}

		if (!otherCont) {
			hid_write(this->otherController, buffer, 31);
			for (int x = 0; x < 40; x++) {
				hid_read(this->otherController, readBuffer, 31);
				if (readBuffer[14] == 0x30) {
					otherCont = true;
					break;
				}
			}
		}
	}
	/*
	std::string c = std::to_string(loops);
	c += '\n';
	OutputDebugStringA(c.c_str());
	unsigned char data[31];
	data[0] = 0x21;
	hid_get_feature_report(this->controller, data, 31);
	OutputDebugStringA(std::to_string(data[14]).c_str());

	OutputDebugStringA(std::to_string(readBuffer[14]).c_str());
	*/
	/*
	for(int x = 0; x < 10; x++){
		if (this->setup == CtrlSetup::TwoJoy) {
			hid_write(this->otherController, buffer, 31);
		}

		if (hid_write(this->controller, buffer, 31) == -1) {
			wxLogDebug("Write Failed\n");
		}
	}
	*/
}
