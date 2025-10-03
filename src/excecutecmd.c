// executecmd.c
#include "executecmd.h"
#include "globals.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <windows.h>
#include <time.h>

// Thread parameter struct
typedef struct {
    WORD scanCode;
    DWORD holdTimeMs;
    int isMouse;   // 0 = keyboard, 1 = left mouse, 2 = right mouse
} ScanThreadParam;

// Worker thread: press, hold, release one scancode or mouse button
static DWORD WINAPI SendScanThread(LPVOID arg) {
    ScanThreadParam *p = (ScanThreadParam*)arg;

    if (p->isMouse == 0) {
        // ---- Keyboard key ----
        INPUT ip[2] = {0};

        // Key down (scancode)
        ip[0].type           = INPUT_KEYBOARD;
        ip[0].ki.wVk         = 0;
        ip[0].ki.wScan       = p->scanCode;
        ip[0].ki.dwFlags     = KEYEVENTF_SCANCODE;
        ip[0].ki.dwExtraInfo = 0;

        // Key up
        ip[1] = ip[0];
        ip[1].ki.dwFlags |= KEYEVENTF_KEYUP;

        SendInput(1, &ip[0], sizeof(ip[0]));
        Sleep(p->holdTimeMs);
        SendInput(1, &ip[1], sizeof(ip[1]));
    } 
    else {
        // ---- Mouse click ----
        INPUT ip[2] = {0};
        ip[0].type = INPUT_MOUSE;
        ip[1].type = INPUT_MOUSE;

        if (p->isMouse == 1) { // Left mouse
            ip[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            ip[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
        } 
        else if (p->isMouse == 2) { // Right mouse
            ip[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
            ip[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
        }

        SendInput(1, &ip[0], sizeof(ip[0]));
        Sleep(p->holdTimeMs);
        SendInput(1, &ip[1], sizeof(ip[1]));
    }

    free(p);
    return 0;
}

void executecmd(int key) {
    // scancode map for indices 0–11
    static const WORD scanMap[12] = {
        0x20,  // [0] D
        0x1E,  // [1] A
        0,0,
        0x39,  // [4] SPACE
        0x39,  // [5] SPACE
        0,0,
        0x11,  // [8] W
        0x1F,  // [9] S
        0,     // [10] Left Click
        0      // [11] Right Click
    };

    if (key < 0 || key >= 12 || (scanMap[key] == 0 && key < 10)) {
        // invalid or unmapped
        return;
    }

    // Allocate and fill parameter
    ScanThreadParam *param = malloc(sizeof(*param));
    if (!param) return;

    if (key == 10) {         // Left click
        param->isMouse   = 1;
        param->scanCode  = 0;
        param->holdTimeMs = 150;
    }
    else if (key == 11) {    // Right click
        param->isMouse   = 2;
        param->scanCode  = 0;
        param->holdTimeMs = 150;
    }
    else {
        param->isMouse   = 0;
        param->scanCode  = scanMap[key];
        if (key == 0 || key == 1) { // D or A
            param->holdTimeMs = 250;  // same intensity as before
        }
        else {
            param->holdTimeMs = 150;
        }
    }

    // Launch detached thread to send this input
    HANDLE h = CreateThread(
        NULL, 0,
        SendScanThread,
        param,
        0, NULL
    );
    if (h) CloseHandle(h);
}

DWORD WINAPI excutecode(LPVOID arg) {
    int key;
    while (1) {
        key = que_dequeue(que);
        if (key >= 0) {
            executecmd(key);
        }
        Sleep(1);
    }
    return 0;
}

