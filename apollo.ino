#define topRight    24
#define topLeft     42
#define bottomRight 23
#define bottomLeft  43

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"
#include "parson.h"
#include <Adafruit_BMP085.h>

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
Adafruit_BMP085 bmp;

#define LED_PIN 13

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;

float p;
float t;

char output[512];
char input[512];


void setup() {
  Wire.begin();

  // initialize serial communication
  while (!SerialUSB);
  SerialUSB.begin(115200);
  accelgyro.initialize();

  // verify connection
  SerialUSB.println("Could not find a valid BMP085 sensor, check wiring!");

  if (!bmp.begin()) {
    SerialUSB.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }
}


void loop() {
  JSON_Object *object = waitForCommand();
  const char *command = json_object_get_string(object, "command");

  if (stringEquals(command, "data")) {
    pressure();
    temperature();
    motion();
    print();
  } else if (stringEquals(command, "pinMode")) {
    uint8_t pin = (uint8_t)json_object_get_number(object, "pin");
    uint8_t mode = (uint8_t)json_object_get_number(object, "mode");
    setPinMode(pin, mode);
  } else if (stringEquals(command, "digitalWrite")) {
    uint8_t pin = (uint8_t)json_object_get_number(object, "pin");
    int value = (int)json_object_get_number(object, "value");
    digitalWrite(pin, value);
  } else {
    SerialUSB.println("fuck dude, idk...");
  }
}

bool stringEquals(const char *first, const char *second) {
  return strcmp(first, second) == 0;
}

/*
 *  Block the event loop to wait for input
 */
JSON_Object* waitForCommand() {
  JSON_Object *object;
  while((object = parse(input)) == NULL) {
    read();
  }

  /* clear input array */
  memset(input, 0, 512);

  return object;
}

/*
 *  Read whatever is on the serial line
 */
void read() {
  int index = 0;
  char inChar;

  while(SerialUSB.available() > 0) {
    if(index < 511) {
      inChar = SerialUSB.read(); // Read a character
      input[index] = inChar; // Store it
      index++; // Increment where to write next
      input[index] = '\0'; // Null terminate the string
    }
  }
}

/*
 *  Parse a string into a JSON object
 */
JSON_Object* parse(const char *input) {
  JSON_Value *root;
  JSON_Object *object;
  size_t i;

  /* parsing json and validating output */
  root = json_parse_string(input);
  object = json_value_get_object(root);
  if (json_value_get_type(root) == JSONError) {
    return NULL;
  }

  return object;
}

/*
 *  Set a pin as an input or output
 */
void setPinMode(uint8_t pin, uint8_t mode) {
  pinMode(pin, mode);
}

/*
 *  Set a pin as high or low
 */
void setPinValue(uint8_t pin, uint8_t value) {
  digitalWrite(pin, value);
}

void pressure() {
  p = bmp.readPressure();
}

void temperature() {
  t = bmp.readTemperature();
}

void motion() {
  accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
}

void print() {
  // sprintf(output, "first: %f\nsecond: %f %f %f %f %f %f", 1.0, 2.0, 0, 0, 0, 6.0, 7.0);
  sprintf(output, "{\naccelerometer: {\nx: %i,\ny: %i,\nz: %i\n},\ngyroscope: {\nx: %i,\ny: %i,\nz: %i\n},\ntemperature: %f,\npressure: %f\n}", ax, ay, az, gx, gy, gz, t, p);
  SerialUSB.println(output);
}
