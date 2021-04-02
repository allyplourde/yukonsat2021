
// This code opens a serial communication link between
// the computer and arduino (connected through USB)
// 
// The main program can be used to setup a customized
// unit test based on the components and functions 
// connected to the Arduino board.
//
// Serial Communication setup based on:
// https://www.xanthium.in/Serial-Port-Programming-using-Win32-API  
//
// Written By:  Ally Plourde
// Created:     June 16th, 2020
//
// Modifications: (Date, Author, Description)

#include <iostream>
#include <windows.h>
#include <string>


using namespace std;

int main() {
    cout << "Hello world!\n";
    /*
    // Initialze Device Control Block (DCB)
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    HANDLE comm = ::CreateFile( 
        "COM3",                  // Port name, for ports higher than 9 "\\\\.\\COM24"
        GENERIC_READ|GENERIC_WRITE,  // access ( read and write)
        0,                           // (share) 0:cannot share the COM port
        0,                           // security  (None)
        OPEN_EXISTING,               // creation : open_existing
        FILE_FLAG_OVERLAPPED,        // we want overlapped operation
        0                            // no templates file for COM port
        );

    if (comm == INVALID_HANDLE_VALUE)
        cout << ("Error opening communication port\n");
    else {
        cout << ("Successfully opened serial port!\n");

        // Configure DCB Structure
        dcbSerialParams.BaudRate = CBR_9600;
        dcbSerialParams.ByteSize = 8;
        dcbSerialParams.StopBits = ONESTOPBIT;
        dcbSerialParams.Parity = NOPARITY;

        // Configure serial port with DCB parameters
        SetCommState(comm, &dcbSerialParams);

        // Setup Timeouts
        COMMTIMEOUTS timeouts = {0}; 
        // all timeout values in milliseconds
        timeouts.ReadIntervalTimeout         = 5000; // maximum interval between arrivial Bytes
        // total time-out for read operation based on number of bytes, constant, and multiplier
        timeouts.ReadTotalTimeoutConstant    = 5000; // time-out for read operations
        timeouts.ReadTotalTimeoutMultiplier  = 1000; // multiplied by number of bytes
        timeouts.WriteTotalTimeoutConstant   = 50; // time-out for write operations
        timeouts.WriteTotalTimeoutMultiplier = 10; // multiplied by number of bytes

        SetCommTimeouts(comm, &timeouts);

        // Reading serial port    
        SetCommMask(comm, EV_RXCHAR);    
        DWORD dwEventMask;
        WaitCommEvent(comm, &dwEventMask, NULL);
        char TempChar[1] = {0}; //Temporary character used for reading
        char SerialBuffer[256];//Buffer for storing Rxed Data
        DWORD bytesRead = 1;
        int i = 0;

        do {
            if(!ReadFile(comm, TempChar, sizeof(TempChar), &bytesRead, NULL)){
                printf("Read Error:\t0x%08x\n",GetLastError());
            }
            SerialBuffer[i] = TempChar[0];// Store Tempchar into buffer
            i++;
        } while (bytesRead > 0);
        cout << SerialBuffer << "\n";
        cout << ("Reached program end...\n");


    };

    CloseHandle(comm);//Closing the Serial Port
    */
    return 0;
}


