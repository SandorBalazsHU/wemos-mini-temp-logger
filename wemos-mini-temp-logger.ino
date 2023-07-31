#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WEMOS_SHT3X.h>

#define OLED_RESET 0
const int chipSelect = D8;
Adafruit_SSD1306 display(OLED_RESET);
SHT3X sht30(0x45);

int tempStore[60];
int humiStore[60];
int i = 0;

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 60000;

int currentScrean = 0;
int robin = 0;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  if (!SD.begin(chipSelect)) {
    // don't do anything more
  }
  for(int i=0; i<60; i++) {
    tempStore[i] = 0;
    humiStore[i] = 0;
  }
  startMillis = millis();
  int currentScrean = 1;
}

void loop() {
  currentMillis = millis();

  if(millis() % 1000 < 10 ) {
    switch (currentScrean) {
      case 1:
        screen01();
        break;
      case 2:
        screen02();
        break;
      case 3:
        screen03();
        break;
      default:
        //--
        break;
    }
    roundRobin();
  }

  timer();
}

void roundRobin(){
  robin++;
  if(robin<20) currentScrean = 1;
  if(robin>=20 and robin<25) currentScrean = 2;
  if(robin>=25 and robin<30) currentScrean = 3;
  if(robin>=30) robin = 0;
}

void timer() {
  if (currentMillis - startMillis >= period) {
    process01();
    dataLogger();
    startMillis = currentMillis;
  }
}

void screen01(){
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
}

void screen02(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.println("HourlyTemp");
  for(int j=0; j<60; j++) {
    display.drawLine(j, 48, j, 48-tempStore[j], WHITE);
  }
  display.display();
}

void screen03(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.println("HourlyHumi");
  for(int j=0; j<60; j++) {
    display.drawLine(j, 48, j, 48-humiStore[j], WHITE);
  }
  display.display();
}

void process01(){
  if(sht30.get()==0){
    tempStore[i] = int(sht30.cTemp);
    humiStore[i] = int(sht30.humidity)/2;
    if(i<60) {
      i++;
    }else{
      i = 0;
    }
  }
}

void dataLogger() {
  if(sht30.get()==0){
    String dataString = "";
    dataString +=  String(sht30.cTemp);
    dataString += ";";
    dataString += String(sht30.humidity);
    dataString += ";";
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
    }
  }
}