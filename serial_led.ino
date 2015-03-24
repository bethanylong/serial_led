/*
 * Serial LED "server" for Teensyduino
 * Receive commands over USB serial from some "client",
 * and make LEDs light up accordingly.
 * 
 * I'm a noob at (real) C programming for devices like these, so I compile and
 * upload this through the Arduino IDE (with Teensyduino installed).
 * If you do this too, make sure USB Type under Tools is set to Serial.
 * Also make sure your Board is set correctly.
 *
 * You'll probably need to change the pin numbers.
 */

const int num_lights = 2;
const int num_colors = 3;
int lights[num_lights][num_colors] = { // red pin, green pin, blue pin
                     { 0, 1, 2 }, // D0, D1, D2
                     { 43, 44, 45 } // F5, F6, F7
                   };

void red(int light_num) {
    digitalWrite(lights[light_num][0], LOW);
    digitalWrite(lights[light_num][1], HIGH);
    digitalWrite(lights[light_num][2], HIGH);
}

void yellow(int light_num) {
    digitalWrite(lights[light_num][0], LOW);
    digitalWrite(lights[light_num][1], LOW);
    digitalWrite(lights[light_num][2], HIGH);
}

void green(int light_num) {
    digitalWrite(lights[light_num][0], HIGH);
    digitalWrite(lights[light_num][1], LOW);
    digitalWrite(lights[light_num][2], HIGH);
}

void blue(int light_num) {
    digitalWrite(lights[light_num][0], HIGH);
    digitalWrite(lights[light_num][1], HIGH);
    digitalWrite(lights[light_num][2], LOW);
}

void off(int light_num) {
    digitalWrite(lights[light_num][0], HIGH);
    digitalWrite(lights[light_num][1], HIGH);
    digitalWrite(lights[light_num][2], HIGH);
}

void setup() {
    Serial.begin(9600);

    for (int i = 0; i < num_lights; i++) {
        for (int j = 0; j < num_colors; j++) {
            pinMode(lights[i][j], OUTPUT);
        }
        blue(i);
    }
}

// Receive a number value in a byte.
// Light colors and status descriptions are inspired by Nagios.
// Examples:
//     0 -> OK on light 0
//     1 -> WARN on light 0
//     12 -> CRIT on light 1
//     15 -> UNKNOWN on light 1
//     20 -> no-op (if no light 2 exists)
void loop() {
    if (Serial.available()) {
        char received = Serial.read();
        // If you change light_offset here, change it in the "client" too
        const int light_offset = 10;
        int light = received / light_offset;

        if (received % light_offset == 0) { // OK: green
            green(light);
        } else if (received % light_offset == 1) { // WARN: yellow
            yellow(light);
        } else if (received % light_offset == 2) { // CRIT: red
            red(light);
        } else if (received % light_offset == 4) { // off
            off(light);
        } else { // UNKNOWN: blue
            blue(light);
        }
    }

    delay(100); // don't spin too fast
}

/* vim: set syntax=c: */
