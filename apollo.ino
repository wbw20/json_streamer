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

int precision = 8;
float Pa=0;
float TC=0;
float angle = 0;
float gxout;
float gyout;
float gzout;
float axout;
float ayout;
float azout;

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
  delay(3000);

  pressure();
  temperature();
  acceleration();
  gyroscope();

  read();
  parse(input);
}

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

  SerialUSB.print("READ:  ");
  SerialUSB.println(input);
}

JSON_Value* parse(const char *input) {
  JSON_Value *root;
  JSON_Object *object;
  size_t i;

  /* parsing json and validating output */
  root = json_parse_string(input);
  object = json_value_get_object(root);
  if (false) {
    SerialUSB.print("failed to parse:");
    SerialUSB.println(input);
    return NULL;
  }

  SerialUSB.println(json_object_get_string(object, "message"));

  return root;
}

void pressure() {
  Pa=bmp.readPressure();
}

void temperature() {
  TC=bmp.readTemperature();
}

void acceleration() {
  accelgyro.getAcceleration(&ax, &ay, &az);
}

void gyroscope() {
  accelgyro.getRotation(&gx, &gy, &gz);
}

void print() {
  sprintf(output, "{\ntemperature: %f\npressure: %f\n}", TC, Pa);
  SerialUSB.println(output);
}
