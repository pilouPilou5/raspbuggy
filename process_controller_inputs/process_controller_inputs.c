#include <stdbool>
#include "process_controller_inputs.h"

struct controller_inputs processControllerInputs(char* message){
    struct controller_inputs inputs;

    int buttons = *(int*)message;
    inputs.x = buttons & 1;
    inputs.y = buttons & 1<<1;
    inputs.a = buttons & 1<<2;
    inputs.b = buttons & 1<<3;
    inputs.lb = buttons & 1<<4;
    inputs.rb = buttons & 1<<5;
    inputs.share = buttons & 1<<6;
    inputs.select = buttons & 1<<7;
    inputs.xbox = buttons & 1<<8;
    inputs.up_arrow = buttons & 1<<9;
    inputs.down_arrow = buttons & 1<<10;
    inputs.left_arrow = buttons & 1<<11;
    inputs.right_arrow = buttons & 1<<12;
    inputs.left_joystick = buttons & 1<<13;
    inputs.right_joystick = buttons & 1<<14;

    inputs.left_joystick_x_axis = *(float*)message+32;
    inputs.left_joystick_x_axis = *(float*)message+2*32;
    inputs.left_joystick_x_axis = *(float*)message+3*32;
    inputs.left_joystick_x_axis = *(float*)message+4*32;
    inputs.lt = *(float*)message+5*32;
    inputs.rt = *(float*)message+6*32;

    return inputs;
}