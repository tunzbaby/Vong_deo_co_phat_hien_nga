#include <SoftwareSerial.h>
SoftwareSerial sim(10, 11); // Chan TX va RX cua Module SIM 800a
SoftwareSerial BTserial(2, 3); // Chan TX va RX cua module Bluetooth HC 05
int state = 0;
int hc05 = 0;

#define led 8
#define loa 6

String number = "0866531953"; // +880 is the country code
// String number = "+880XXXXXXXXXX"; // +880 is the country code

void setup() {
  Serial.begin(9600);
  Serial.println("Doi vai giay....");
  delay(5000);
  Serial.println("Da khoi dong he thong SIM...");
  sim.begin(9600);
  delay(1000);
  Serial.println("Nhap c de thuc hien cuoc goi va s de gui SMS");
  
  // Thiet lap cong bluetooth
  BTserial.begin(38400);
  pinMode(led, OUTPUT);
  pinMode(loa, OUTPUT);
}

void callNumber() {
  sim.print (F("ATD"));
  sim.print (number);
  sim.print (F(";\r\n"));
}

void SendMessage()
{
  sim.println("AT+CMGF=1");
  delay(1000);
  sim.println("AT+CMGS=\"" + number + "\"\r");
  delay(1000);
  String SMS = "SOS - Tinh trang khan cap";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);
  delay(1000);
}

void loop() {
  delay(50);
  if (BTserial.available() > 0) { // Kiem tra xem co data vao cong serial khong?
    hc05 = BTserial.read(); // Doc gia tri tu BT Master
    // Serial.read();
  }
  // Dieu khien LED
  if (hc05 == '1') {
    digitalWrite(led, HIGH); // Bat den      
    hc05 = 0;
    delay(50);
    callNumber();
    delay(1000);
    //SendMessage(); 
    digitalWrite(led, LOW); // Tat den
    delay(50);
  }
  /*
  else if (hc05 == '2') {
    digitalWrite(loa, HIGH); // Bat loa
    SendMessage();
    hc05 = 0;
  */
}

/*
void BT(){
 if (BTserial.available() > 0) { // Kiem tra xem co data vao cong serial khong?
    hc05 = BTserial.read(); // Doc gia tri tu BT Master
  }
  // Dieu khien LED
  if (hc05 == '1') {
    digitalWrite(led, HIGH); // Bat den  
    delay(1000);  
    hc05 = 0;
  }
  else if (hc05 == '0') {
    digitalWrite(loa, HIGH); // Bat loa
    delay(500);
    hc05 = 0;
}
}
*/
