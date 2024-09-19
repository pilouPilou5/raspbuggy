#include <stdio.h>
#include <stdlib.h>
#include "wifi_api.h"

#define CMD_MAX_SIZE 100

void generate_hotspot(char ssid[], char name[], char pass[]) {
    char cmd[CMD_MAX_SIZE];
    char ip[20] = "10.214.0.1/24";
    printf("Creating your hotspot...\n");
    sprintf(cmd, "sudo nmcli device wifi hotspot ssid %s password %s con-name %s ifname wlan0", ssid, pass, name);
    system(cmd);
    printf("Assigning IPv4 address...\n");
    sprintf(cmd, "sudo nmcli connection modify id %s ipv4.addresses %s", name, ip);
    system(cmd);
    printf("Rebooting hotspot...\n");
    sprintf(cmd, "sudo nmcli device connect wlan0");
    system(cmd);
    sprintf(cmd, "sudo nmcli device connect wlan0");
    system(cmd);
    printf("Network information:\n");
    sprintf(cmd, "nmcli connection show");
    system(cmd);
    printf("RaspberryPi hotspot's new network interface:\n");
    sprintf(cmd, "ifconfig wlan0");
    system(cmd);
}

void delete_hotspot(char name[]) {
    char cmd[CMD_MAX_SIZE];
    printf("Deleting hotspot %s", name);
    sprintf(cmd, "sudo nmcli connection delete %s", name);
    system(cmd);
}