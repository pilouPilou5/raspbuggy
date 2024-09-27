#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <Windows.h>
#include <iostream>
#include <Xinput.h>
#include <gst/gst.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

#define IP_ADDRESS "10.42.0.73"  // Replace with your IP address
#define PORT "5000"                // Replace with your port number

void gst_thread_receive(void) {

    GstElement* pipeline;
    GstBus* bus;
    GstMessage* msg;
    GstStateChangeReturn ret;
    char command[512];


    // Construct the GStreamer pipeline command with specific IP and port
    snprintf(command, sizeof(command),
        "udpsrc port=%s caps=\"application/x-rtp, media=(string)video, encoding-name=(string)JPEG\" ! "
        "rtpjpegdepay ! jpegdec ! videoconvert ! autovideosink",
        PORT);

    //Create pipeline
    pipeline = gst_parse_launch(command, NULL);
    if (!pipeline) {
        g_printerr("Failed to create pipeline.\n");
    }

    // Set the pipeline to the PLAYING state
    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr("Failed to set pipeline to playing state.\n");
        gst_object_unref(pipeline);
    }

    // Get the bus and wait for EOS or ERROR message
    bus = gst_element_get_bus(pipeline);
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, (GstMessageType)(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

    // Handle the received message
    if (msg != NULL) {
        if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_ERROR) {
            GError* err;
            gchar* debug_info;
            gst_message_parse_error(msg, &err, &debug_info);
            g_printerr("Error: %s\n", err->message);
            g_free(debug_info);
            g_error_free(err);
        }
        else if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_EOS) {
            g_print("End of stream reached.\n");
        }
        gst_message_unref(msg);
    }

    // Clean up
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
}

DWORD WINAPI gstreamerThread(void* p) {
    gst_thread_receive();
    return 0;
}

int main(int argc, char** argv){
    gst_init(&argc, &argv);
    HANDLE thread = CreateThread(NULL, 0, gstreamerThread, NULL, 0, NULL);

    //initialize winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed.\n");
        exit(1);
    }

    //create UDP socket
    //std::string hostname{ "10.214.10.140" };
    std::string hostname{ "10.214.0.1" };
    uint16_t port = 5004;

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
    int buttons;
    while (1) {
        error = XInputGetState(0, &state);
        if (error == ERROR_SUCCESS) {
            joystick = state.Gamepad.sThumbLX / 32768.;
            left_trigger = (state.Gamepad.bLeftTrigger - 127) / 128.;
            right_trigger = (state.Gamepad.bRightTrigger - 127) / 128.;
            buttons = !!(state.Gamepad.wButtons & XINPUT_GAMEPAD_X) + 2 * !!(state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) + 4 * !!(state.Gamepad.wButtons & XINPUT_GAMEPAD_A) + 8 * !!(state.Gamepad.wButtons & XINPUT_GAMEPAD_B);
            cout << "left trigger : " << left_trigger << "    right trigger : " << right_trigger << "   joystick : " << joystick << "   buttons : " << buttons << endl;
            buf[0] = *(float*)&buttons;
            buf[1] = joystick;
            buf[2] = 0;
            buf[3] = 0;
            buf[4] = 0;
            buf[5] = left_trigger;
            buf[6] = right_trigger;

            bytes_sent = ::sendto(sock, (char*)buf, 7 * sizeof(float), 0, reinterpret_cast<sockaddr*>(&destination), sizeof(destination));
            if (bytes_sent == SOCKET_ERROR) {
                wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
                closesocket(sock);
                WSACleanup();
                return 1;
            }
        }
        Sleep(20);
    }
}