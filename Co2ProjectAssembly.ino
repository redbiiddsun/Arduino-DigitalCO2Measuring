#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// initialize the library with the numbers of the interface pins

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
SoftwareSerial sensor(8, 9); // RX, TX

int Contrast = 75;
const int analogPin = A0;
const long samplePeriod = 10000L;
const byte requestReading[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
byte result[9];
long lastSampleTime = 0;


void setup() {
  // put your setup code here, to run once:
     lcd.begin(16, 2);
     analogWrite(6,Contrast);
     lcd.setCursor(5, 0);
     lcd.println("Welcome    ");
     lcd.setCursor(2, 1);
     lcd.println("To Co2 Meter    ");
     delay(2000);

     lcd.clear();
     lcd.setCursor(4, 0);
     lcd.println("Owner by     ");
     lcd.setCursor(0, 1);
     lcd.println("Phanasorn Sri...");
     delay(2000);

     lcd.clear();
     lcd.setCursor(3, 1);
     lcd.println("Please Wait  ");
     lcd.setCursor(14, 1);
     lcd.blink();

     Serial.begin(9600);
     sensor.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
    long now = millis();
    if (now > lastSampleTime + samplePeriod) {
    lastSampleTime = now;
    
    int ppmS = readPPMSerial();
    Serial.println("Number of CO2 is ");
    Serial.print(ppmS);
  }

}


//Method Get Value from MH-Z14
int readPPMSerial() {
  for (int i = 0; i < 9; i++) {
    sensor.write(requestReading[i]); 
  }
  //Serial.println("sent request");
  while (sensor.available() < 9) {}; // wait for response
  for (int i = 0; i < 9; i++) {
    result[i] = sensor.read(); 
  }
  int high = result[2];
  int low = result[3];
    //Serial.print(high); Serial.print(" ");Serial.println(low);
  return high * 256 + low;
}
