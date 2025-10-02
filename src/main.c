#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <time.h>

#include "queue.h"
#include "executecmd.h"
#include "globals.h"

#define PORT1 8080
#define PORT2 8081
#define MAX_PACKETS 100
Queue* que;

CRITICAL_SECTION queueLock;
Node* front = NULL;
Node* rear = NULL;

// Thread function to receive UDP packets
DWORD WINAPI udpReceiverThread(LPVOID lpParam) {
    int port = *(int*)lpParam;

    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) {
        printf("Socket creation failed on port %d, error: %d\n", port, WSAGetLastError());
        return 1;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        printf("Bind failed on port %d, error: %d\n", port, WSAGetLastError());
        closesocket(sockfd);
        return 1;
    }

    printf("Thread listening on port %d\n", port);

    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    char buffer[1024]; // buffer for multiple packets

    while (1) {
        int n = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                         (struct sockaddr*)&clientAddr, &clientAddrLen);

        if (n == SOCKET_ERROR) {
            printf("recvfrom failed on port %d, error: %d\n", port, WSAGetLastError());
            break;
        }

        // How many packets received
        int numPackets = n / sizeof(DataPacket);
        DataPacket* packets = (DataPacket*)buffer;

        EnterCriticalSection(&queueLock);
        for (int i = 0; i < numPackets; i++) {
            //printf("code: %d\n",packets[i].code);
            sortedEnqueue(&front, &rear, packets[i]);
        }
        //printf("Queue after receiving from port %d:\n", port);
        // printQueue(front);
        LeaveCriticalSection(&queueLock);
    }

    closesocket(sockfd);
    return 0;
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    que = que_init();
    
    if (!que) {
        printf("Failed to initialize queue\n");
        return 1;
    }

    InitializeCriticalSection(&queueLock);

    DWORD threadId1, threadId2;
    int port1 = PORT1;
    int port2 = PORT2;

    HANDLE hThread1 = CreateThread(NULL, 0, udpReceiverThread, &port1, 0, &threadId1);
    HANDLE hThread2 = CreateThread(NULL, 0, udpReceiverThread, &port2, 0, &threadId2);

    DWORD dwThreadId;
    HANDLE hThread = CreateThread(
        NULL,           // default security attributes
        0,              // default stack size
        excutecode,     // thread function
        NULL,           // argument to thread function
        0,              // default creation flags
        &dwThreadId     // thread identifier
    );
    
    if (hThread == NULL) {
        printf("Error creating thread\n");
        return 1;
    }
    
    printf("Thread created with ID: %lu\n", dwThreadId);
    



    if (!hThread1 || !hThread2) {
        printf("Failed to create threads.\n");
        DeleteCriticalSection(&queueLock);
        WSACleanup();
        return 1;
    }

Sleep(100);  // Wait 100ms for threads to start

printf("Starting main processing loop...\n");

DataPacket packet;

while (1) {
    EnterCriticalSection(&queueLock);
    if (front == NULL) {
        LeaveCriticalSection(&queueLock);
        Sleep(10);  // Wait 10ms before checking again
        continue;
    }
    packet = deQueue(&front);
    LeaveCriticalSection(&queueLock);
    
    if (packet.code == 0) {
        continue;
    }
    
    int code = packet.code;
    int* keys = (int*)malloc(6*sizeof(int));
    keys = convert_to_unique_code(code);
    for (int i = 0; i < 6; i++) {
        printf("Key: %d\n", keys[i]);
        if (keys[i] != -1) {
            que_enqueue(que, keys[i]);
        }
    }
     printf("Code: %d\n", code);
}



    // Wait for threads (in real app, you might run infinitely)
    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    DeleteCriticalSection(&queueLock);
    CloseHandle(hThread);
    WSACleanup();

    return 0;
}
