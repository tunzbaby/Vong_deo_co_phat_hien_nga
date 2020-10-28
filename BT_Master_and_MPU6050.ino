#include <SoftwareSerial.h>
SoftwareSerial BTserial(10, 11);
#include "I2Cdev.h"
#include "MPU6050.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

int state = 0;
MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;
uint32_t lastTime;

#define OUTPUT_READABLE_ACCELGYRO
#define ledbao 7
#define nutnhan 12

bool blinkState = false;

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    BTserial.begin(38400);
    Serial.begin(19200);
    accelgyro.initialize();
    //Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
    pinMode(ledbao, OUTPUT);
    pinMode(nutnhan, INPUT_PULLUP);
    digitalWrite(ledbao, LOW);
}

void loop() {
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);

    #ifdef OUTPUT_READABLE_ACCELGYRO
        // Serial.print("a/g:\t");
        /*
        Serial.print(ax); Serial.print("\t");
        Serial.print(ay); Serial.print("\t");
        Serial.print(az); Serial.print("\t");
        
        Serial.print(gx); Serial.print("\t");
        Serial.print(gy); Serial.print("\t");
        Serial.println(gz);
        
        Serial.println(millis());
        */
    #endif

    #ifdef OUTPUT_BINARY_ACCELGYRO
        Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
        Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
        Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
        Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
        Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
        Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
    #endif
    

    // Chuong trinh kiem tra nga
    
    // Nga sang trai - 2019.12.21 - 8:08 PM | 3 gia tri x, y, z khi nga khao sat duoc 32767  11436 -25912
    if(abs(ax) > 32000 && abs(ax) < 45000 && abs(ay) > 15000 && abs(ay) < 33000 && abs(az) > 16000 && abs(az) < 33000 ) // Khi de truc Y qua nho se lam canh bao de nhieu hon
    {
      BTserial.write('1');
      Serial.println("Phat hien nga xay ra");                
      int button_val = 1;
      lastTime = millis();
      button_val = digitalRead(nutnhan);
      while(millis() - lastTime < 5000)
        {
          digitalWrite(ledbao, HIGH);
          button_val = digitalRead(nutnhan);
          delay(50);
          Serial.println(button_val);
          if(!button_val) break;  
        }
      if(!button_val){
        Serial.println("Khong lam gi ca, nut da duoc nhan");
        digitalWrite(ledbao, LOW);
        delay(200);
      }
      
      else{
          digitalWrite(ledbao, HIGH);
         Serial.println("Canh bao da duoc gui"); 
         // xem lai code phan nay de gui du lieu qua bluetooth
         digitalWrite(ledbao, LOW);
      }
   }
delay(1);   
}
