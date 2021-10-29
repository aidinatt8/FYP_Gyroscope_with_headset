// getting angles using DMP (digital motion processor)
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps_V6_12.h"
#include "Wire.h"
#include "EEPROMex.h"
#define MOTOR_PIN 2
#define BUTTON_PIN 3
#define BUTTON_PIN_RESET 4
//***********************************************************************************************************************************
//***********************************************************************************************************************************
#define BUFFER_SIZE 100
#define START_BYTE 1010
MPU6050 mpu;
float angleX = 0;
float angleY = 0;
float angleZ = 0;
long int start_time = 0;
long int start_button = 0;
long int start_button_reset = 0;
bool start = false;
//***********************************************************************************************************************************
//***********************************************************************************************************************************
void(* resetFunc) (void) = 0;
//***********************************************************************************************************************************
//***********************************************************************************************************************************
void setup() 
{
Wire.begin();
Wire.setClock(400000); 
Serial.begin(115200);
mpu.initialize();

//----------------- SETTING OFFSETS ------------------------
int offsets[6];
for (byte i = 0; i < 6; i++) { // setting the calibration array into zero // by default offsets are saved in 1010, 1012, 1014, 1016, 1018, 1020
  offsets[i] = EEPROM.readInt(START_BYTE + i * 2);
} // setting offsets from the board memory
mpu.setXAccelOffset(offsets[0]);
mpu.setYAccelOffset(offsets[1]);
mpu.setZAccelOffset(offsets[2]);
mpu.setXGyroOffset(offsets[3]);
mpu.setYGyroOffset(offsets[4]);
mpu.setZGyroOffset(offsets[5]);
initDMP(); 
//----------------- SETTING OFFSETS ------------------------
pinMode(MOTOR_PIN, OUTPUT);
pinMode(BUTTON_PIN, INPUT_PULLUP);
//pinMode(BUTTON_PIN_RESET, INPUT_PULLUP); 
digitalWrite(MOTOR_PIN, HIGH);
delay(2000);
digitalWrite(MOTOR_PIN, LOW);
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
void loop() {
getAngles();
Serial.print(angleX); Serial.print(',');
Serial.print(angleY); Serial.print(',');
Serial.print(angleZ); Serial.print(',');
Serial.print(180); Serial.print(','); 
Serial.print(30); Serial.print(','); // Serial.print(180); Serial.print(',');
Serial.println(-180);
if (angleY < 30) //if angle is less than 30, then we stop counting time 
{
  start = false;
}
else 
{ 
  if (start == false) //if there was no timing before, we start the timer
      start_time = millis();
      start = true; 
}
if ((start == true) && (millis() - start_time) > 5000) //if more than 5 seconds have passed, we turn on the vibration motor
    digitalWrite(MOTOR_PIN, HIGH);
else
    digitalWrite(MOTOR_PIN, LOW);

delay(20);

if (digitalRead(BUTTON_PIN) == HIGH)
    start_button = millis();
if (millis() - start_button > 2000) //if the button is pressed more than 2 seconds, calibration starts
{
    calibration();
    start_button = millis();
}
// if (digitalRead(BUTTON_PIN_RESET) == HIGH)//если зажали вторую кнопку более 2х секунд то вызываем перезагрузку устройства
// start_button_reset = millis();
// if (millis() - start_button_reset > 2000)
// {
// resetFunc(); //вызываем reset
// start_button_reset = millis();
// } 
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
// VARIABLES
const float toDeg = 180.0 / M_PI;
uint8_t mpuIntStatus; // holds act
ual interrupt status byte from MPU
uint8_t devStatus; // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize; // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount; // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
Quaternion q; // [w, x, y, z] quaternion container
VectorFloat gravity; // [x, y, z] gravity vector
float ypr[3]; // [yaw, pitch, roll] yaw/pitch/roll container and gravity vector
//***********************************************************************************************************************************
//***********************************************************************************************************************************
//INITIALIZATION 
void initDMP() {
    devStatus = mpu.dmpInitialize(); //load and configure the DMP
    mpu.setDMPEnabled(true); //turn on the DMP
    mpuIntStatus = mpu.getIntStatus();
    packetSize = mpu.dmpGetFIFOPacketSize(); //get expected DMP packet size for later comparison
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
// getting angles вangleX, angleY, angleZ
void getAngles() {
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) { // if can read a packet from FIFO
      mpu.dmpGetQuaternion(&q, fifoBuffer); //get quaternion values in matrix form
      mpu.dmpGetGravity(&gravity, &q); //extracting gravity component
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity); //passing gravity to to get the pitch 
      angleX = ypr[2] * toDeg; //converting to degrees
      angleY = ypr[1] * toDeg;
      angleZ = ypr[0] * toDeg;
    }
}
//***********************************************************************************************************************************
//***********************************************************************************************************************************
// ======= CALIBRATION OF DATA INTO EEPROM LIBRARY =======
void calibration() {
long offsets[6];
long offsetsOld[6];
int16_t mpuGet[6];
// by default sensitivity of accelerometer is 2g and gyroscope is 250degrees
mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
// setting offsets to zero
mpu.setXAccelOffset(0);
mpu.setYAccelOffset(0);
mpu.setZAccelOffset(0);
mpu.setXGyroOffset(0);
mpu.setYGyroOffset(0);
mpu.setZGyroOffset(0);
delay(5);
for (byte n = 0; n < 10; n++) { // 10 calibration iterations
  for (byte j = 0; j < 6; j++) { // set the array to zero 
    offsets[j] = 0;
}
for (byte i = 0; i < 100 + BUFFER_SIZE; i++) { // make BUFFER_SIZE measurements for averaging mpu.getMotion6(&mpuGet[0], &mpuGet[1], &mpuGet[2], &mpuGet[3], &mpuGet[4], &mpuGet[5]); if (i >= 99) // пропускаем первые 99 измерений
   for (byte j = 0; j < 6; j++) {
    offsets[j] += (long)mpuGet[j]; // appending into calibration array
  }
}
for (byte i = 0; i < 6; i++) {
  offsets[i] = offsetsOld[i] - ((long)offsets[i] / BUFFER_SIZE); // include previous calibration 
  if (i == 2) offsets[i] += 16384; // if axis is Z, calibrate to 16384
  offsetsOld[i] = offsets[i];
}
// put new offset values
mpu.setXAccelOffset(offsets[0] / 8);
mpu.setYAccelOffset(offsets[1] / 8);
mpu.setZAccelOffset(offsets[2] / 8);
mpu.setXGyroOffset(offsets[3] / 4);
mpu.setYGyroOffset(offsets[4] / 4);
mpu.setZGyroOffset(offsets[5] / 4);
delay(2);
}
// writing into memory
for (byte i = 0; i < 6; i++) { 
// offset values are stored in 1010, 1012, 1014, 1016, 1018, 1020
  if (i < 3)
    EEPROM.updateInt(START_BYTE + i * 2, offsets[i] / 8);
  else
    EEPROM.updateInt(START_BYTE + i * 2, offsets[i] / 4);
}
initDMP();
}
