// test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// Nintendo VID = 0x57E
// Joy-Con R PID = 2007
// Joy-Con L PID = 2006
// Pro Controller PID = 2009
//
// Y:0x#1 X:0x#2 A:0X#8 B:0x#4 Report Mode 0x30
// Y:0x#8 X:0x#2 A:0x#1 B:0x#4 Report Mode 0x3F
// 
// HCI State, page scan mode controller expecting a message? Possibly reboot.
// By default 0x3F report is used. Sends a report on button press.


#include <stdio.h> // printf
#include <wchar.h>

#include <iostream>
#include <hidapi.h>
#include <thread>

#include <Windows.h>
#include <WinUser.h>

int main()
{
    (void)&hid_get_input_report;
    wchar_t wstr[255];
    std::cout << "Hello World!\n";
    hid_init();
    //HID\{00001124 - 0000 - 1000 - 8000 - 00805f9b34fb}_VID & 0002057e_PID & 2007

    hid_device *a = hid_open(0x57E, 0x2007, NULL);
    if (!a) {
        wprintf(hid_error(a));
        hid_close(a);
        hid_exit();
    }
    hid_get_manufacturer_string(a, wstr, 255);

    //hid_set_nonblocking()

    printf("%ls", wstr);
    unsigned char buf[HID_API_MAX_REPORT_DESCRIPTOR_SIZE];
    int res = hid_get_report_descriptor(a, buf, HID_API_MAX_REPORT_DESCRIPTOR_SIZE); // 728757824
    
    for (int i = 0; i < res; i++) {
        if (i % 10 == 0) {
            printf("\n");
        }
        printf("0x%02x, ", buf[i]);
    }

    /*
    unsigned char buf4[31];
    buf4[0] = 0x01;
    buf4[1] = 1;
    buf4[5] = 0;
    buf4[9] = 0;
    buf4[10] = 0x01;
    buf4[11] = 0x01;
    buf4[12] = 0x2C;
    buf4[13] = 0x5C;
    buf4[14] = 0x71;
    buf4[15] = 0x76;
    buf4[16] = 0xB6;
    buf4[17] = 0xB4;
    buf4[18] = 0x00;
    buf4[19] = 0x00;
    if (hid_write(a, buf4, 32) == -1) {
        wprintf(hid_error(a));
    }

    std::cin.ignore();
    
    unsigned char buf5[31];
    buf5[0] = 0x01;
    buf5[1] = 1;
    buf5[5] = 0;
    buf5[9] = 0;
    buf5[10] = 0x01;
    buf5[11] = 0x03;
    if (hid_write(a, buf5, 32) == -1) {
        wprintf(hid_error(a));
    }
    */
    uint16_t hf1 = 0x01a8; //Set H.Frequency
    uint8_t hf1_amp = 0x88; //Set H.Frequency amplitude
    uint8_t lf1 = 0x63; //Set L.Frequency
    uint16_t lf1_amp = 0x804d; //Set L.Frequency amplitude

    unsigned char buf6[31];
    buf6[0] = 0x01;
    buf6[1] = 1;
    buf6[2] = hf1 & 0xFF;
    buf6[3] = hf1_amp + ((hf1 >> 8) & 0xFF);
    buf6[4] = lf1 + ((lf1_amp >> 8) & 0xFF);
    buf6[5] = lf1_amp & 0xFF;
    buf6[6] = hf1 & 0xFF;
    buf6[7] = hf1_amp + ((hf1 >> 8) & 0xFF);
    buf6[8] = lf1 + ((lf1_amp >> 8) & 0xFF);
    buf6[9] = lf1_amp & 0xFF;
    buf6[10] = 0x03;
    buf6[11] = 0x30;
    
    if (!hid_write(a, buf6, 31)) {
        wprintf(hid_error(a));
    }

    unsigned char buf1[32];
    RtlZeroMemory(buf1, 32);
    //buf1[0] = 48;
    hid_set_nonblocking(a, 0);
    int read = 0;
    if (!a) {
        read = 101;
    }
    bool yUp = true;
    for (int x = 0; read < 1000;)
    {
        //printf("made it");
        res = hid_read(a, buf1, 32); // 728757824
        //printf("%d\n", read);
        /*
        printf("%d\n", res);
        for (int i = 0; i < res; i++) {
            if (i % 10 == 0) {
                printf("\n");
            }
            printf("0x%02x, ", buf1[i]);
        }
        */
        printf("Report Mode: 0x%02x\n", buf1[0]);
        if (buf1[3] != 0) {
            printf("0x%02x 0x%02x\n", buf1[3], buf1[4]);
        }
        // Scan Codes: A:30 Left:75
        if (buf1[3] == 0x01) {
            yUp = false;
            INPUT inputs[1];
            RtlZeroMemory(inputs, sizeof(inputs));
            inputs[0].type = INPUT_KEYBOARD;
            inputs[0].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_EXTENDEDKEY;
            inputs[0].ki.wScan = 0x4B;
            inputs[0].ki.wVk = 0;
            SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
            read++;
            printf("%d\n", read);
        }
        else if (!yUp){
            INPUT inputs[1];
            RtlZeroMemory(inputs, sizeof(inputs));
            inputs[0].type = INPUT_KEYBOARD;
            inputs[0].ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE | KEYEVENTF_EXTENDEDKEY;
            inputs[0].ki.wScan = 0x4B;
            inputs[0].ki.wVk = 0;
            SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
            yUp = true;
        }
        
        //wprintf(hid_error(a));
    }
    

    std::cin.ignore();

    unsigned char buf3[31];
    buf3[0] = 0x01;
    buf3[1] = 1;
    buf3[9] = 0;
    buf3[10] = 0x31;
    //buf3[11] = 0x00;

    //%%%%%%%%%%%%%%% From dekuNukem %%%%%%%%%%%%%%%%
    //Float frequency to hex conversion

    float freq = 300.0f;
    float amp = 0.22f;
    if (freq < 0.0f)
        freq = 0.0f;
    else if (freq > 1252.0f)
        freq = 1252.0f;
    uint8_t encoded_hex_freq = (uint8_t)round(log2((double)freq / 10.0) * 32.0);

    //Convert to Joy-Con HF range. Range in big-endian: 0x0004-0x01FC with +0x0004 steps.
    uint16_t hf = (encoded_hex_freq - 0x60) * 4;
    //Convert to Joy-Con LF range. Range: 0x01-0x7F.
    uint8_t lf = encoded_hex_freq - 0x40;

    // Float amplitude to hex conversion
    uint8_t encoded_hex_amp = 0;
    if (amp > 0.23f)
        encoded_hex_amp = (uint8_t)round(log2f(amp * 8.7f) * 32.f);
    else if (amp > 0.12f)
        encoded_hex_amp = (uint8_t)round(log2f(amp * 17.f) * 16.f);
    else {
        // TBD
    }
    uint16_t hf_amp = encoded_hex_amp * 2;    // encoded_hex_amp<<1;
    uint8_t lf_amp = encoded_hex_amp / 2 + 64;// (encoded_hex_amp>>1)+0x40;

    //Left linear actuator
    //uint16_t hf = 0x01a8; //Set H.Frequency
    //uint8_t hf_amp = 0x88; //Set H.Frequency amplitude
    //Byte swapping
    buf3[6] = hf & 0xFF;
    buf3[7] = hf_amp + ((hf >> 8) & 0xFF); //Add amp + 1st byte of frequency to amplitude byte

    //uint8_t lf = 0x63; //Set L.Frequency
    //uint16_t lf_amp = 0x804d; //Set L.Frequency amplitude
    //Byte swapping
    buf3[8] = lf + ((lf_amp >> 8) & 0xFF); //Add freq + 1st byte of LF amplitude to the frequency byte
    buf3[9] = lf_amp & 0xFF;

    //%%%%%%%%%%%%%%% From dekuNukem %%%%%%%%%%%%%%%%

    printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
    res = hid_write(a, buf3, 32);
    
    memset(buf1, 0, 32);
    buf1[0] = 0x21;
    for (int x = 0; x < 100; x++)
    {
        res = hid_read(a, buf1, 32); // 728757824
        if (buf1[0] == 0x21) {
            printf("FOUND FOUND FOUND FOUND\n");
            for (int i = 0; i < res; i++) {
                if (i % 10 == 0) {
                    printf("\n");
                }
                printf("0x%02x, ", buf1[i]);
            }
            printf("\n");
            if (buf1[15] == 0x01) {
                printf("Player 1");
            }
            else if (buf[15] == 0x02) {
                printf("Player 2");
            }

            break;
        }
    }
    buf3[10] = 0x30;
    buf3[11] = 0x12; //0b10010
    
    res = hid_write(a, buf3, 32);

    std::cin.ignore();

    /* Disconnect
    unsigned char buf2[31];
    buf2[0] = 0x01;
    buf2[1] = 1;
    buf2[9] = 0;
    buf2[10] = 0x06;
    buf2[11] = 0x01;
    res = hid_write(a, buf2, 32);
    if (res == -1) {
        wprintf(hid_error(a));
    }
    */

    hid_close(a);
    //printf("Close device");
    hid_exit();
}

/*
0x05, 0x01, 0x09, 0x05, 0xa1, 0x01, 0x85, 0x21, 0x06, 0x01,
0xff, 0x09, 0x21, 0x15, 0x00, 0x25, 0x00, 0x75, 0x08, 0x95,
0x30, 0x81, 0x02, 0x85, 0x30, 0x09, 0x30, 0x15, 0x00, 0x25,
0x00, 0x75, 0x08, 0x95, 0x30, 0x81, 0x02, 0x85, 0x31, 0x09,
0x31, 0x15, 0x00, 0x25, 0x00, 0x75, 0x08, 0x96, 0x69, 0x01,
0x81, 0x02, 0x85, 0x32, 0x09, 0x32, 0x15, 0x00, 0x25, 0x00,
0x75, 0x08, 0x96, 0x69, 0x01, 0x81, 0x02, 0x85, 0x33, 0x09,
0x33, 0x15, 0x00, 0x25, 0x00, 0x75, 0x08, 0x96, 0x69, 0x01,
0x81, 0x02, 0x85, 0x3f, 0x05, 0x09, 0x19, 0x01, 0x29, 0x10,
0x15, 0x00, 0x25, 0x01, 0x75, 0x01, 0x95, 0x10, 0x81, 0x02,
0x05, 0x01, 0x09, 0x39, 0x15, 0x00, 0x25, 0x07, 0x75, 0x04,
0x95, 0x01, 0x81, 0x42, 0x75, 0x04, 0x95, 0x01, 0x81, 0x03,
0x09, 0x30, 0x09, 0x31, 0x09, 0x33, 0x09, 0x34, 0x15, 0x00,
0x27, 0xff, 0xff, 0x00, 0x00, 0x75, 0x10, 0x95, 0x04, 0x81,
0x02, 0x85, 0x01, 0x06, 0x01, 0xff, 0x09, 0x01, 0x15, 0x00,
0x27, 0xff, 0xff, 0x00, 0x00, 0x75, 0x08, 0x95, 0x30, 0x91,
0x02, 0x85, 0x10, 0x09, 0x10, 0x15, 0x00, 0x27, 0xff, 0xff,
0x00, 0x00, 0x75, 0x08, 0x95, 0x30, 0x91, 0x02, 0x85, 0x11,
0x09, 0x11, 0x15, 0x00, 0x27, 0xff, 0xff, 0x00, 0x00, 0x75,
0x08, 0x95, 0x30, 0x91, 0x02, 0x85, 0x12, 0x09, 0x12, 0x15,
0x00, 0x27, 0xff, 0xff, 0x00, 0x00, 0x75, 0x08, 0x95, 0x30,
0x91, 0x02, 0xc0,
*/

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
