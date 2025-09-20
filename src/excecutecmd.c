#include <windows.h>
#include <stdio.h>
#include "queue.h"
#include "globals.h"
#include "executecmd.h"

void executecmd(int key){
    static WORD keyMap[12] = {
        0x41,
        0x44,
        0,0,0,0,
        0x57,
        0x53,0,0,0,0
    };
    if (key < 0 || key >= 12 || keyMap[key] == 0) {
        printf("Invalid key index: %d\n", key);
        return;
    }

    // Setup key input structure
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    // Press key
    ip.ki.wVk = keyMap[key];
    ip.ki.dwFlags = 0; // key down
    SendInput(1, &ip, sizeof(INPUT));

    // Release key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // key up
    SendInput(1, &ip, sizeof(INPUT));

}
void* excutecode( void *arg){
  

    int key=-1;
    
    while (1) {
        key = que_dequeue(&que);
        if(key!=-1){
            executecmd(key);
        }
        // Sleep for 1 millisecond
        Sleep(1);
    }

    return NULL;
    
    

}
