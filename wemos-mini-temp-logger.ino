#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WEMOS_SHT3X.h>

#define OLED_RESET 0
Adafruit_SSD1306 display(OLED_RESET);
SHT3X sht30(0x45);

int tempStore[60];
int humiStore[60];
int i = 0;

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 10000;

unsigned long startMillis2;
unsigned long currentMillis2;
const unsigned long period2 = 60000;

int skip = 0;

void setup() {
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  for(int i=0; i<60; i++) {
    tempStore[i] = 0;
    humiStore[i] = 0;
  }
  startMillis = millis();
  startMillis2 = millis();
}

void loop() {
  currentMillis = millis();
  currentMillis2 = millis(); 

  if(millis() % 1000 < 10 ) {
    Serial.println(skip);
    if(skip == 0){
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.setTextColor(WHITE);
      if(sht30.get()==0){
        display.println("Temp: ");
        display.setTextSize(2);
        display.println(sht30.cTemp);
        display.setTextSize(1);
        display.println("Humidity: ");
        display.setTextSize(2);
        display.println(sht30.humidity);
      }else {
        display.println("Error!");
      }
      display.display();
    }else{
      skip--;
    }
  }

  if (currentMillis - startMillis >= period) {
    skip = 4;
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.setTextColor(WHITE);
    display.println("HourlyTemp");
    for(int j=0; j<60; j++) {
      display.drawLine(j, 48, j, 48-tempStore[j], WHITE);
    }
    display.display();
    startMillis = currentMillis;
  }

  if (currentMillis2 - startMillis2 >= period2) {
    if(sht30.get()==0){
      tempStore[i] = int(sht30.cTemp);
      humiStore[i] = int(sht30.humidity)/2;
      if(i<60) {
        i++;
      }else{
        i = 0;
      }
    }
    startMillis2 = currentMillis2;
  }
}
