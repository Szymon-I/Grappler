#define OLED_ADDRESS 0x3C
#define MPU_ADDRESS 0x68
#define POTENTIOMETER_PIN A1
#define JOY_X_PIN A2
#define JOY_Y_PIN A3
#define KEY_PIN 7
#define LOOP_INTERVAL 25
#define LED_PIN 13
#define SCREEN_SIZE_W 128
#define SCREEN_SIZE_H 64
#define UP_DATA_LIMIT 1000
#define LOW_DATA_LIMIT -1000
#define JOY_THRESHOLD UP_DATA_LIMIT / 67
#define ACC_THRESHOLD UP_DATA_LIMIT / 100
//#define POT_THRESHOLD UP_DATA_LIMIT/50
#define ACC_UP_LIMIT 16000
#define ACC_DOWN_LIMIT -16000
#define ANALOG_UP_LIMIT 1024
#define ANALOG_DOWN_LIMIT 0
#define OUTPUT_DATA_FORMAT "%+05d/%+05d/%+05d\r\n"
#define SERIAL_SPEED 115200
#define ACC_OFF_X 1146
#define ACC_OFF_Y -714
#define ACC_OFF_Z 1300
#define GYR_OFF_X 99
#define GYR_OFF_Y -48
#define GYR_OFF_Z 8
#define OLED_DISPLAY_INTERVAL 300
#define START_DELAY 2000

#include "I2Cdev.h"
#include "MPU6050.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

// global variables for storing sensor data

int ax, ay, az;
int gx, gy, gz;

int pot_data;
int joy_x, joy_y;

// output buffer string for sending data
char out_buff[50];

// flags for program
bool acc_mode = false;
bool blinkState = false;

// create mpu and oled objects
MPU6050 accelgyro(MPU_ADDRESS);
Adafruit_SSD1306 display(SCREEN_SIZE_W, SCREEN_SIZE_H);

// remove distorion incoming from data
int remove_distortion(int x, int threshold)
{
  if (abs(x) <= threshold)
  {
    return 0;
  }
  else
  {
    return x;
  }
}

// normalize data from potentiometer
void normalize_pot()
{
  pot_data = map(pot_data, ANALOG_DOWN_LIMIT, ANALOG_UP_LIMIT, LOW_DATA_LIMIT, UP_DATA_LIMIT);
}

// normalize data from joystick
void normalize_joy()
{
  joy_x = map(joy_x, ANALOG_DOWN_LIMIT, ANALOG_UP_LIMIT, LOW_DATA_LIMIT, UP_DATA_LIMIT);
  joy_y = map(joy_y, ANALOG_DOWN_LIMIT, ANALOG_UP_LIMIT, LOW_DATA_LIMIT, UP_DATA_LIMIT);
  joy_x = remove_distortion(joy_x, JOY_THRESHOLD);
  joy_y = remove_distortion(joy_y, JOY_THRESHOLD);
}

// normalize data from mpu (using accelerometer)
void normalize_acc(int *ax, int *ay, int *az)
{
  (*ax) = map((*ax), ACC_DOWN_LIMIT, ACC_UP_LIMIT, LOW_DATA_LIMIT, UP_DATA_LIMIT);
  (*ay) = map((*ay), ACC_DOWN_LIMIT, ACC_UP_LIMIT, LOW_DATA_LIMIT, UP_DATA_LIMIT);
  (*az) = map((*az), ACC_DOWN_LIMIT, ACC_UP_LIMIT, LOW_DATA_LIMIT, UP_DATA_LIMIT);
  (*ax) = remove_distortion(*ax, ACC_THRESHOLD);
  (*ay) = remove_distortion(*ay, ACC_THRESHOLD);
  (*az) = remove_distortion(*az, ACC_THRESHOLD);
}

// read analog data from potentiometer
void get_potentiometer_data()
{
  pot_data = analogRead(POTENTIOMETER_PIN);
  normalize_pot();
}

// read mpu data (using accelerometer) from i2c bus
void get_acc_data()
{
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  normalize_acc(&ax, &ay, &az);
}

// read analog data from joystick
void get_joy_data()
{
  joy_x = analogRead(JOY_X_PIN);
  joy_y = analogRead(JOY_Y_PIN);
  normalize_joy();
}

// feed measured data info output string buffer
void bind_buff()
{
  get_potentiometer_data();
  if (acc_mode)
  {
    get_acc_data();
    sprintf(out_buff, OUTPUT_DATA_FORMAT, ax, ay, pot_data);
  }
  else
  {
    get_joy_data();
    sprintf(out_buff, OUTPUT_DATA_FORMAT, joy_x, joy_y, pot_data);
  }
}

// diplay actual data on oled screen
void display_oled()
{
  static int prev_time = 0;
  int actual_time = millis();
  if (actual_time - prev_time < OLED_DISPLAY_INTERVAL)
  {
    return;
  }
  prev_time = actual_time;
  display.clearDisplay();
  display.setCursor(0, 0); // (x,y)
  display.setTextSize(2);
  display.print("Mode: ");
  if (acc_mode)
  {
    display.println("acc");
  }
  else
  {
    display.println("joy");
  }
  display.setCursor(0, 20); // (x,y)
  display.setTextSize(0);
  display.print((const char *)out_buff);
  display.setCursor(0, 50);
  display.print("Baudrate: ");
  display.print(SERIAL_SPEED);
  display.display();
}

// send actual data via serial
void send_serial()
{
  Serial.print(out_buff);
}

// change mode between acc <-> joystick
void handle_mode_change(){
  if(digitalRead(KEY_PIN)==HIGH){
    acc_mode = true;
  }
  else{
    acc_mode = false;
  }
}

// initialize oled screen
void init_oled()
{
  delay(100);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setRotation(0);
  display.dim(0);
}

// initialize mpu6050
void init_mpu()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Initializing I2C...");
  accelgyro.initialize();
  display.setCursor(0, 20);
  display.print("Testing connections...");
  display.setCursor(0, 30);
  display.print(accelgyro.testConnection() ? "MPU6050 OK\n" : "MPU6050 BAD\n");
  display.display();

  // set offsets for mpu6050
  accelgyro.setXAccelOffset(ACC_OFF_X);
  accelgyro.setYAccelOffset(ACC_OFF_Y);
  accelgyro.setZAccelOffset(ACC_OFF_Z);
  accelgyro.setXGyroOffset(GYR_OFF_X);
  accelgyro.setYGyroOffset(GYR_OFF_Y);
  accelgyro.setZGyroOffset(GYR_OFF_Z);
}

// initialize i2c bus
void init_i2c()
{
  // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif
}

// starting setup for uC
void setup()
{
  init_i2c();
  init_oled();
  init_mpu();
  Serial.begin(SERIAL_SPEED);
  pinMode(LED_PIN, OUTPUT);
  delay(START_DELAY);
  pinMode(KEY_PIN, INPUT_PULLUP);
}

// main program loop
// read data according to mode, display on led, send via uart
void loop()
{
  bind_buff();
  send_serial();
  display_oled();
  handle_mode_change();
  delay(LOOP_INTERVAL);
}
