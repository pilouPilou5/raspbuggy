#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <Windows.h>
#include <iostream>
#include <Xinput.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main()
{
    cout << "Hello World!\n";

    //initialize winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed.\n");
        exit(1);
    }

    //create UDP socket
    std::string hostname{ "192.168.207.206" };
    uint16_t port = 2000;

    SOCKET sock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    sockaddr_in destination;
    destination.sin_family = AF_INET;
    destination.sin_port = htons(port);
    destination.sin_addr.s_addr = inet_addr(hostname.c_str());


    char* buf = (char*)malloc(100 * sizeof(char));
    XINPUT_STATE state;
    short joystick;
    BYTE left_trigger, right_trigger;
    int trigger;
    int error;
    int bytes_sent;
    while (1) {
        error = XInputGetState(0, &state);
        if (error == ERROR_SUCCESS) {
            joystick = state.Gamepad.sThumbLX;
            joystick = (joystick + 32768) * 90 / 32767;     //processing joystick and trigger informations to be between 0 and 180 (standard for servos)
            left_trigger = state.Gamepad.bLeftTrigger;
            right_trigger = state.Gamepad.bRightTrigger;
            trigger = (right_trigger - left_trigger + 256) * 90 / 255;

            cout << "left trigger : " << (int)left_trigger << "    right trigger : " << (int)right_trigger << "   joystick : " << joystick << endl;
            sprintf_s(buf, 100, "%d,%d\n", trigger, joystick);
            bytes_sent = ::sendto(sock, buf, strlen(buf), 0, reinterpret_cast<sockaddr*>(&destination), sizeof(destination));
            if (bytes_sent == SOCKET_ERROR) {
                wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
                closesocket(sock);
                WSACleanup();
                return 1;
            }
        }
        Sleep(100);
    }
}
