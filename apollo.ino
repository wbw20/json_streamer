// I2C device class (I2Cdev) demonstration Arduino sketch for MPU9150
// 1/4/2013 original by Jeff Rowberg <jeff@rowberg.net> at https://github.com/jrowberg/i2cdevlib
//          modified by Aaron Weiss <aaron@sparkfun.com>
//
// Changelog:
//     2011-10-07 - initial release
//     2013-1-4 - added raw magnetometer output
/*
I2Cdev device library code is placed under the MIT license

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
 This is an example for the BMP085 Barometric Pressure & Temp Sensor

  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> https://www.adafruit.com/products/391

  These displays use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources pr`oviding this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!)
// Connect GND to Ground
// Connect SCL to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
// Connect SDA to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
// EOC is not used, it signifies an end of conversion
// XCLR is a reset pin, also not used here


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
  
  if (!bmp.begin()) {
	SerialUSB.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }
  
  //Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  /*if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }*/
  //Serial.println("card initialized.");

  
  //SerialUSB.println("Testing device connections...");
  //SerialUSB.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  // configure Arduino LED for
  pinMode(LED_PIN, OUTPUT);
  pinMode(bottomRight, INPUT);
  digitalWrite(bottomRight, HIGH);
  pinMode(topRight, INPUT);
  digitalWrite(topRight, HIGH);
  
  pinMode(13,OUTPUT);
}
 

void loop() {
  digitalWrite(13,HIGH);
  digitalWrite(13,LOW);
  
  //accel, gyro, mag
  accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
  string = "time: ";
  string += millis();
  string += "\t";
  string = "a(x,y,z): ";
  string += ay;
  string += "\t";
  string += ax;
  string += "\t";
  string += az;
  string += "\t";
  string += "g(x,y,z): ";
  string += gy;
  string += "\t";
  string += gx;
  string += "\t";
  string += gz;
  string += "\t";
  string += "m(x,y,z): ";
  string += mx;
  string += "\t";
  string += my;
  string += "\t";
  string += mz;
  string += "\t";
  string += "P: ";
  string += Pa;
  string += "\t";
  string += "T: ";
  string += TC;
  SerialUSB.println(string);
  
  //accel,gyro
  for (int ag=0;ag<=21;ag++){
    accelgyro.getAcceleration(&ax, &ay, &az);
    accelgyro.getRotation(&gx, &gy, &gz);
    string = "time: ";
    string += millis();
    string += "\t";
    string = "a(x,y,z): ";
    string += ay;
    string += "\t";
    string += ax;
    string += "\t";
    string += az;
    string += "\t";
    string += "g(x,y,z): ";
    string += gy;
    string += "\t";
    string += gx;
    string += "\t";
    string += gz;
    string += "\t";
    string += "m(x,y,z): ";
    string += mx;
    string += "\t";
    string += my;
    string += "\t";
    string += mz;
    string += "\t";
    string += "P: ";
    string += Pa;
    string += "\t";
    string += "T: ";
    string += TC;
    SerialUSB.println(string);
  }
  //Pressure
  Pa=bmp.readPressure();
  string = "time: ";
  string += millis();
  string += "\t";
  string = "a(x,y,z): ";
  string += ay;
  string += "\t";
  string += ax;
  string += "\t";
  string += az;
  string += "\t";
  string += "g(x,y,z): ";
  string += gy;
  string += "\t";
  string += gx;
  string += "\t";
  string += gz;
  string += "\t";
  string += "m(x,y,z): ";
  string += mx;
  string += "\t";
  string += my;
  string += "\t";
  string += mz;
  string += "\t";
  string += "P: ";
  string += Pa;
  string += "\t";
  string += "T: ";
  string += TC;
  SerialUSB.println(string);
      
    //accel, gyro
  for (int ag=0;ag<=21;ag++){
    accelgyro.getAcceleration(&ax, &ay, &az);
    accelgyro.getRotation(&gx, &gy, &gz);
    string = "time: ";
    string += millis();
    string += "\t";
    string = "a(x,y,z): ";
    string += ay;
    string += "\t";
    string += ax;
    string += "\t";
    string += az;
    string += "\t";
    string += "g(x,y,z): ";
    string += gy;
    string += "\t";
    string += gx;
    string += "\t";
    string += gz;
    string += "\t";
    string += "m(x,y,z): ";
    string += mx;
    string += "\t";
    string += my;
    string += "\t";
    string += mz;
    string += "\t";
    string += "P: ";
    string += Pa;
    string += "\t";
    string += "T: ";
    string += TC;
    SerialUSB.println(string);
  }
    
    //temperature
  TC=bmp.readTemperature();
  string = "time: ";
  string += millis();
  string += "\t";
  string = "a(x,y,z): ";
  string += ay;
  string += "\t";
  string += ax;
  string += "\t";
  string += az;
  string += "\t";
  string += "g(x,y,z): ";
  string += gy;
  string += "\t";
  string += gx;
  string += "\t";
  string += gz;
  string += "\t";
  string += "m(x,y,z): ";
  string += mx;
  string += "\t";
  string += my;
  string += "\t";
  string += mz;
  string += "\t";
  string += "P: ";
  string += Pa;
  string += "\t";
  string += "T: ";
  string += TC;
  SerialUSB.println(string);
      
    //accel, gyro
  for (int ag=0;ag<=21;ag++){
    accelgyro.getAcceleration(&ax, &ay, &az);
    accelgyro.getRotation(&gx, &gy, &gz);
    string = "time: ";
    string += millis();
    string += "\t";
    string = "a(x,y,z): ";
    string += ay;
    string += "\t";
    string += ax;
    string += "\t";
    string += az;
    string += "\t";
    string += "g(x,y,z): ";
    string += gy;
    string += "\t";
    string += gx;
    string += "\t";
    string += gz;
    string += "\t";
    string += "m(x,y,z): ";
    string += mx;
    string += "\t";
    string += my;
    string += "\t";
    string += mz;
    string += "\t";
    string += "P: ";
    string += Pa;
    string += "\t";
    string += "T: ";
    string += TC;
    SerialUSB.println(string);
    }
      
  
  
  /*
  i=i+1;
  if (i==5){
    i=1
  }
  
  mode=i;
  switch (mode) {
    case 1: //accel, gyro, mag
      n=30;
    break;
    case 2: //accel, gyro
      n=500
    break;
    case 3: //pressure
      n=10;
    break;
    case 4: //temperature
      n=30;
    break;
  }
  
    for (int j=0;j<=n;j++;) {
      switch (mode){
        case 1:
        accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
        break;
        case 2:
        accelgyro.getAcceleration(&ax, &ay, &az);
        accelgyro.getRotation(&gx, &gy, &gz);
        break;
        case 3:
        Pa=bmp.readPressure();
        break;
        case 4:
        TC=bmp.readTemperature();
        break;
      }
        
      string = "time: ";
      string += millis();
      string += "\t";
      string = "a(x,y,z): ";
      string += ay;
      string += "\t";
      string += ax;
      string += "\t";
      string += az;
      string += "\t";
      string += "g(x,y,z): ";
      string += gy;
      string += "\t";
      string += gx;
      string += "\t";
      string += gz
      string += "\t";
      string += "m(x,y,z): ";
      string += mx;
      string += "\t";
      string += my;
      string += "\t";
      string += mz;
      string += "\t";
      string += "P: ";
      string += Pa;
      string += "\t";
      string += "T: ";
      string += TC;
      
      SerialUSB.println(string);
    }
    */
    /*
      accelgyro.getAcceleration(&ax, &ay, &az);
      SerialUSB.print("a(x,y,z):"); SerialUSB.print("/t");
      ayout = ((ay+750.)*(20./32500.)-.4292)*.9644;
      SerialUSB.print(ayout,precision); SerialUSB.print("\t");
      axout = ((ax+750.)*(20./32500.)-.2383)*.9695;
      SerialUSB.print(axout,precision); SerialUSB.print("\t");
      azout = ((az+750.)*(20./32500.)-.1458)*.9655;
      SerialUSB.print(azout,precision); SerialUSB.print("\t");
      
      accelgyro.getRotation(&gx, &gy, &gz);
      SerialUSB.print("g(x,y,z):"); SerialUSB.print("/t");
      gyout = gy*(360./32700.);
      SerialUSB.print(gyout,precision); SerialUSB.print("\t");
      gxout = gx*(360./32700.);
      SerialUSB.print(gxout, precision); SerialUSB.print("\t");
      gzout = gz*(360./32700.);
      SerialUSB.println(gzout,precision);
      */
  
  
    
    
  
  
           /*
        break;
        case 'A':
          for (int j=0;m<100;m++){
            ayout = ((ay+750.)*(20./32500.)-.4292)*.9644;
            SerialUSB.print(ayout,precision); SerialUSB.print("\t");
            axout = ((ax+750.)*(20./32500.)-.2383)*.9695;
            SerialUSB.print(axout,precision); SerialUSB.print("\t");
            azout = ((az+750.)*(20./32500.)-.1458)*.9655;
            SerialUSB.print(azout,precision); SerialUSB.println("\t");
          }
        break;
      }
    }*/
  
  //SerialUSB.print(mx); SerialUSB.print("\t");
  //SerialUSB.print(my); SerialUSB.print("\t");
  //SerialUSB.print(mz); SerialUSB.println("\t");


/*
  if (my > 0) {
    angle = 90 - (float)(atan((float)(mx) / (float)(my)) * (180 / 3.14159));
  }
  if (my < 0) {
    angle = 270 - (float)(atan((float)(mx) / (float)(my)) * (180 / 3.14159));
  }
  if ((my = 0) && mx < 0) {
    angle = 180;
  }
  if ((my = 0) && mx > 0) {
    angle = 0.0;
  }
  SerialUSB.println(angle);*/


  // blink LED to indicate activity
  blinkState = !blinkState;
  digitalWrite(LED_PIN, blinkState);
}

//Xoffset=0.2216
//Yoffset=0.4276
//Zoffset=0.1188
//Xscale=0.9685
//Yscale=0.9645
//Zscale=0.9656

