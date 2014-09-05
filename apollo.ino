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
bool blinkState = false;
float angle = 0;
float gxout;
float gyout;
float gzout;
float axout;
float ayout;
float azout;
int i=0;
String string;
int mode;
int n;


void setup() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
  Wire.begin();

  // initialize serial communication
  // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
  // it's really up to you depending on your project)
  while (!SerialUSB);
  SerialUSB.begin(115200);
  
  /*SerialUSB.println('a');
  int a = 'b';
  
  while (a!='a')
  {
    a=SerialUSB.read();
  }
*/
  // initialize device
  //SerialUSB.println("Initializing I2C devices...");
  accelgyro.initialize();

  // verify connection
  
  SerialUSB.println("Could not find a valid BMP085 sensor, check wiring!");

  if (!bmp.begin()) {
	SerialUSB.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }

  pinMode(10, OUTPUT);

  pinMode(LED_PIN, OUTPUT);
  pinMode(bottomRight, INPUT);
  digitalWrite(bottomRight, HIGH);
  pinMode(topRight, INPUT);
  digitalWrite(topRight, HIGH);
  
  pinMode(13,OUTPUT);
}
 

void loop() {
  pressure();
  temperature();
  acceleration();
  gyroscope();

  print();
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
  char output[512];
  sprintf(output, "{temperature: %f}", TC);
  SerialUSB.println(output);
}
