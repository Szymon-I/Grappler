// 0x3C - address of oled display
// 0x68 - address of gy521 - accelerometer
#define OLED_ADDRESS 0x3C
#define MPU_ADDRESS 0x68

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 accelgyro(MPU_ADDRESS);
int16_t ax, ay, az;
int16_t gx, gy, gz;


#define LED_PIN 13
bool blinkState = false;


#include <Adafruit_GFX.h>  // Include core graphics library for the display
#include <Adafruit_SSD1306.h>  // Include Adafruit_SSD1306 library to drive the display


Adafruit_SSD1306 display(128, 64);  // Create display



void setup() {


  delay(100);  // This delay is needed to let the display to initialize

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Initialize display with the I2C address of 0x3C
 
  display.clearDisplay();  // Clear the buffer

  display.setTextColor(WHITE);  // Set color of the text

  display.setRotation(0);  // Set orientation. Goes from 0, 1, 2 or 3

  display.dim(0);  //Set brightness (0 is maximun and 1 is a little dim)


  display.clearDisplay();
  // Print text:
  display.setCursor(0, 10);  // (x,y)
  display.println("Acc data:");  // Text or value to print
  display.display();  // Print everything we set previously
  
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    Serial.begin(115200);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

struct AccOff{
  int x;
  int y;
  int z;
};
typedef struct AccOff AccOff;
AccOff Aoff={1146,-714,1300};

struct GyrOff{
   int x;
  int y;
  int z;
};
//typedef struct GyrOff GyrOff;
struct GyrOff Goff={99,-48,8};

    accelgyro.setXAccelOffset(Aoff.x);
    accelgyro.setYAccelOffset(Aoff.y);
    accelgyro.setZAccelOffset(Aoff.z);
    accelgyro.setXGyroOffset(Goff.x);
    accelgyro.setYGyroOffset(Goff.y);
    accelgyro.setZGyroOffset(Goff.z);
        // configure Arduino LED pin for output
    pinMode(LED_PIN, OUTPUT);
}
void loop() {
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    char out_buff[50];
    sprintf(out_buff, "%+06d/%+06d/%+06d\r\n", ax, ay, az);
    display.clearDisplay();
    Serial.print(out_buff);
    display.setCursor(0, 30);  // (x,y)
    display.print(out_buff);  // Text or value to print
    display.display();  // Print everything we set previously
  
//        Serial.print(ax); Serial.print("\t");
//        Serial.print(ay); Serial.print("\t");
//        Serial.print(az); Serial.print("\t");
//        Serial.print(gx); Serial.print("\t");
//        Serial.print(gy); Serial.print("\t");
//        Serial.println(gz);

    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
    delay(1000);
}
