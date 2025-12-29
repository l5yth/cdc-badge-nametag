#include "app.h"
#include "badge.h"
#include "display.h"
#include "settings.h"
#include <Arduino.h>

int calculate_brightness_step(int current_brightness) { // Kinda logarithmic brightness
    if (current_brightness < 10)
        return 1;
    if (current_brightness < 50)
        return 5;
    if (current_brightness < 150)
        return 15;
    if (current_brightness < 400)
        return 30;
    if (current_brightness < 700)
        return 50;
    return 80;
}

void app_setup(app_state_t *state) { state->is_initialized = true; }

void app_loop(app_state_t *state) {

    // --- Pulsing Logic ---
    static unsigned long last_pulse_time = 0;
    static int pulse_direction = 1; // 1 for up, -1 for down
    static int pulse_value = 0;
    const int pulse_speed = 15;      // Increase to pulse faster
    const int pulse_interval = 15;  // Milliseconds between steps

    if (millis() - last_pulse_time > pulse_interval) {
        last_pulse_time = millis();
        
        pulse_value += (pulse_speed * pulse_direction);

        // Reverse direction at limits
        if (pulse_value >= 1023) {
            pulse_value = 1023;
            pulse_direction = -1;
        } else if (pulse_value <= 0) {
            pulse_value = 0;
            pulse_direction = 1;
        }

        set_setting_brightness(pulse_value);
        display_show();
        // Note: Frequent display_show() calls might cause flickering
        // depending on your hardware.
    }
    // --- End Pulsing Logic ---

    char pressed = pin_expander_process_irq();
    if (pressed != 'x') {
        Serial.print("Button pressed: ");
        Serial.println(pressed);
        switch (pressed) {
        case '1': {
            Serial.println("Brightness +");
            int current_brightness = get_setting_brightness();
            int step = calculate_brightness_step(current_brightness);
            int new_brightness = current_brightness + step;
            if (new_brightness >= 1023) {
                new_brightness = 1023;
                Serial.println("already at maximum brightness");
            }
            set_setting_brightness(new_brightness);
            display_show();
            brightness_osd(new_brightness);
            break;
        }
        case '2': {
            Serial.println("Brightness -");
            int current_brightness = get_setting_brightness();
            int step = calculate_brightness_step(current_brightness);
            int new_brightness = current_brightness - step;
            if (new_brightness <= 0) {
                new_brightness = 0;
                Serial.println("already at minimum brightness");
            }
            set_setting_brightness(new_brightness);
            display_show();
            brightness_osd(new_brightness);
            break;
        }

        default:
            break;
        }
    }

    // Switch battery OFF if FLASH button is pressed
    powerdown_process_irq();

    // Once we have some menu, we can update it here
    // update_menu();
}
