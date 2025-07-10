#include "eadk.h"
#include "engine.h"
#include "start.h"
#include "shape.h"

const char *shape[] = {
    "cube",
    "pyramid"
};

int start(void) {
    eadk_display_push_rect_uniform(eadk_screen_rect, eadk_color_white);
    int index = 0;

    for (int i = 0; i < sizeof(shape) / sizeof(shape[0]); i++) {
        if (i == index) {
            eadk_display_draw_string(shape[i], (eadk_point_t){10, 10 + i * 20}, true, eadk_color_red, eadk_color_white);
        } else {
            eadk_display_draw_string(shape[i], (eadk_point_t){10, 10 + i * 20}, true, eadk_color_black, eadk_color_white);
        }
    }

    while (eadk_keyboard_scan() != 0) {}
    
    while (true) {
        eadk_keyboard_state_t keys = eadk_keyboard_scan();
        
        if (eadk_keyboard_key_down(keys, eadk_key_ok)) {
            return index;
        }
        else if (eadk_keyboard_key_down(keys, eadk_key_right)) {
            if (index != sizeof(shape) / sizeof(shape[0]) - 1) {
                index++;
            }
        }
        else if (eadk_keyboard_key_down(keys, eadk_key_up)) {
            if (index != 0) {
                index--;
            }
        } 
        else if (eadk_keyboard_key_down(keys, eadk_key_down)) {
            if (index != sizeof(shape) / sizeof(shape[0]) - 1) {
                index++;
            }
        }
        else {
            continue; 
        }

        for (int i = 0; i < sizeof(shape) / sizeof(shape[0]); i++) {
            if (i == index) {
                eadk_display_draw_string(shape[i], (eadk_point_t){10, 10 + i * 20}, true, eadk_color_red, eadk_color_white);
            } else {
                eadk_display_draw_string(shape[i], (eadk_point_t){10, 10 + i * 20}, true, eadk_color_black, eadk_color_white);
            }
        }
        
        eadk_timing_msleep(200);

    }
}