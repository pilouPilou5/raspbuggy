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
    std::string hostname{ "10.214.10.140" };
    uint16_t port = 5002;

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


    float buf[10];
    XINPUT_STATE state;
    float joystick;
    float left_trigger, right_trigger;
    int error;
    int bytes_sent;
    while (1) {
        error = XInputGetState(0, &state);
        if (error == ERROR_SUCCESS) {
            joystick = state.Gamepad.sThumbLX / 32768.;
            left_trigger = (state.Gamepad.bLeftTrigger-127)/128.;
            right_trigger = (state.Gamepad.bRightTrigger-127)/128.;
            cout << "left trigger : " << left_trigger << "    right trigger : " << right_trigger << "   joystick : " << joystick << endl;
            buf[0] = 0;
            buf[1] = joystick;
            buf[2] = 0;
            buf[3] = 0;
            buf[4] = 0;
            buf[5] = left_trigger;
            buf[6] = right_trigger;

            bytes_sent = ::sendto(sock, (char*)buf, 7*sizeof(float), 0, reinterpret_cast<sockaddr*>(&destination), sizeof(destination));
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
