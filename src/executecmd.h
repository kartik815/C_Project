// executecmd.h
#pragma once

#include <windows.h>    // brings in DWORD, LPVOID, WINAPI
#include "queue.h"

// Spawns the worker thread that dequeues keys and calls executecmd()
DWORD WINAPI excutecode(LPVOID arg);

// Sends a single key via a detached thread
void executecmd(int key);

