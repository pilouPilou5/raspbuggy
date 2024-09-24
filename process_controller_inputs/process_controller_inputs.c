#include <stdbool.h>
#include <stdio.h>
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

    inputs.left_joystick_x_axis = *(float*)(message+sizeof(float));
    inputs.left_joystick_y_axis = *(float*)(message+2*sizeof(float));
    inputs.right_joystick_x_axis = *(float*)(message+3*sizeof(float));
    inputs.right_joystick_y_axis = *(float*)(message+4*sizeof(float));
    inputs.lt = *(float*)(message+5*sizeof(float));
    inputs.rt = *(float*)(message+6*sizeof(float));

    //printf("joystick: %f  lt: %f  rt: %f\n", inputs.left_joystick_x_axis, inputs.lt, inputs.rt);
    //printf("%f", *(float*)message);
    //printf("%d\n", buttons);

    return inputs;
}