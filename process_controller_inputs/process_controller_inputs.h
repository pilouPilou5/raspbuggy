#ifndef PROCESS_CONTROLLER_INPUTS_H
#define PROCESS_CONTROLLER_INPUTS_H

#include <stdbool.h>

struct controller_inputs{
    bool right_joystick;
    bool left_joystick;
    bool right_arrow;
    bool left_arrow;
    bool down_arrow;
    bool up_arrow;
    bool xbox;
    bool select;
    bool share;
    bool rb;
    bool lb;
    bool b;
    bool a;
    bool y;
    bool x;
    float left_joystick_x_axis;
    float left_joystick_y_axis;
    float right_joystick_x_axis;
    float right_joystick_y_axis;
    float lt;
    float rt;
};

struct controller_inputs processControllerInputs(char* message);

#endif