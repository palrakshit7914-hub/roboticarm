#include <wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <ArduinoJson.h>

//---Wi-Fi credentials--------------
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

//----pca9685 setup----------------
Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver(0x40);
#define PCA_FREQUENCY 50 // Analog servos run at ~50 Hz updates 
#define SERVOMIN  150 // Minimum pulse (0)
#define SERVOMAX  600 // Maximum pulse (180)

//----Servo pin definitions-----------
#define CH_BASE0 0
#define CH_SHOULDER 1
#define CH_ELBOW 2
#define CH_GRIPPER 3
#define NUM_SERVOS 4

//---L298N motor driver pins----------------
#define MOTOR_IN1 26
#define MOTOR_IN2 27
#define MOTOR_ENA 25
#define PWM_CHANNEL 0
#define PWM_FREQUENCY 1000
#define PWM_RESOLUTION 8

//-----EEPROM layout---------
#define EEPROM_SIZE 64
#define EEPROM_MAGIC 0xAB
#define ADDR_MAGIC 0
#define ADDR_SERVO 1
#define ADDR_SLOT 2
#define NUM_POSE_SLOTS 5

//---Interpolation----------------
#define INTERPOLATION_STEPS 40
#define INTERPOLATION_DELAY_MS 15

// ── State ────────────────────────────────────────────────────
int  currentAngles[NUM_SERVOS]  = {90, 90, 90, 90};
int  targetAngles[NUM_SERVOS]   = {90, 90, 90, 90};
int  motorSpeed   = 0;           // 0-255
int  motorDir     = 0;           // 0=stop 1=fwd -1=rev
bool isMoving     = false;

// Saved poses [slot][servo]
int  savedPoses[NUM_POSE_SLOTS][NUM_SERVOS];

WebServer server(80);
