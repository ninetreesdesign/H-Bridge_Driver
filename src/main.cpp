/** brief Motor Driver test code for Adafruit version DRV8871 and Pololu MAX14870
    details TBD

    https://learn.adafruit.com/adafruit-drv8871-brushed-dc-motor-driver-breakout/pinouts
    https://www.pololu.com/product/2961


*/

/* describe
    one takes two inputs: PWM a, LOW b or vice versa
    other takes two inputs PWM and DIR

    */

#include <Arduino.h>

#define HWSERIAL Serial1
#define CONSOLE Serial  // USB port debug console (IDE)
#define E echoString
const uint32_t BAUDRATE = 115200;
const uint16_t ADC_BITS = 16;  // use 16 to match

//const char compileDATE[] PROGMEM = __DATE__;
//const char compileTIME[] PROGMEM = __TIME__;
//const String COMPILE_DATE = ((String(compileDATE)).replace(" ", ",").replace("-", ""));  // convert char array to a string type
//const String COMPILE_TIME = (String(compileTIME));
const String NL = "\n";

/////////////////////////////////////////////////////////////////////////////////////
const String VERSION_NUM = "1.0";  //
const uint32_t LED_INTERVAL = 500;
/////////////////////////////////////////////////////////////////////////////////////

// for MAX14870
const uint8_t PWM_PIN = 3;  // PWM-able on Teensy 3.2
const uint8_t DIR_PIN = 4;  // PWM-able
const uint8_t IN1_PIN = 5;  // PWM-able
const uint8_t IN2_PIN = 6;  // PWM-able
const uint8_t STATUS_LED_PIN = 13;

// timer variables
elapsedMillis since_led = 0;  // ms

// function prototypes
//float readVoltage(int16_t ch, uint8_t num_readings);
double mapFloat(double x, double in_min, double in_max, double out_min, double out_max);

void setup() {
    int t0 = millis();  // find how long to init port
    pinMode(PWM_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(IN1_PIN, OUTPUT);
    pinMode(IN2_PIN, OUTPUT);

    // initialize serial ports
    HWSERIAL.begin(BAUDRATE, SERIAL_8N1);  // optional: configure Teensy HW UART serial port Rx/Tx
    HWSERIAL.println("\x1b[2J \x1b[H");    // clear screen, cursor to home
    sprintf(msg, "\n@   %06lu ms: hardware serial port initialized.\n", millis() - t0);
    E(msg);

    t0 = millis();  // find how long to init port
    // initialize console Serial port
    if (CONSOLE_FLAG) {
        Serial.begin(BAUDRATE);
        while (!Serial && (millis() - t0 < 5000)) {
        }  // include timeout if print console isn't opened
        sprintf(msg, "\n@   %6lu ms, Console Serial port initialized.\n", millis() - t0);
        E(msg);
    }
}

// --- main loop --------------------------------------------------------------------------------
void loop() {
    if (since_led >= LED_INTERVAL) {
        since_led = 0;
        digitalWriteFast(STATUS_LED_PIN, !digitalReadFast(STATUS_LED_PIN));
        delayMicroseconds(1);
    }
    //  if (since_led > 12)
    //      digitalWriteFast(STATUS_LED_PIN,0);

    // ramp up forward
    digitalWrite(IN1_PIN, LOW);
    for (int i = 0; i < 255; i++) {
        analogWrite(IN2_PIN, i);
        delay(10);
    }

    // forward full speed for one second
    delay(1000);

    // ramp down forward
    for (int i = 255; i >= 0; i--) {
        analogWrite(IN2_PIN, i);
        delay(10);
    }

    // ramp up backward
    digitalWrite(IN2_PIN, LOW);
    for (int i = 0; i < 255; i++) {
        analogWrite(IN1_PIN, i);
        delay(10);
    }

    // backward full speed for one second
    delay(1000);

    // ramp down backward
    for (int i = 255; i >= 0; i--) {
        analogWrite(IN1_PIN, i);
        delay(10);
    }
}

/// Output text string to either or both serial ports: hardware serial, USB serial
void echoString(String str) {
    // prints msg to selected port(s)
    if (PRINT_FLAG) {
        if (CONSOLE_FLAG) CONSOLE.print(str);    // print to console
        if (HWSERIAL_FLAG) HWSERIAL.print(str);  // print to HW serial port
    }
}

void echoString(char* str) {
    // prints msg to selected port(s)
    if (PRINT_FLAG) {
        if (CONSOLE_FLAG) CONSOLE.print(str);    // print to console
        if (HWSERIAL_FLAG) HWSERIAL.print(str);  // print to HW serial port
    }
}

v
    /// map one numerical span to another with floating point values
    double
    mapFloat(double x, double in_min, double in_max, double out_min, double out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/

    /// EOF