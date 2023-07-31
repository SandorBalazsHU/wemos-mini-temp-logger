#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WEMOS_SHT3X.h>

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);
SHT3X sht30(0x45);

int tempStore[60];
int humiStore[60];
int i = 0;

void setup() {
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  for(int i=0; i<60; i++) {
    tempStore[i] = 0;
    humiStore[i] = 0;
  }

}

void loop() {
  
  // Clear the buffer.
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(WHITE);

  if(sht30.get()==0){
    display.println("T: ");
    display.setTextSize(2);
    display.println(sht30.cTemp);
    tempStore[i] = int(sht30.cTemp);

    display.setTextSize(1);
    display.println("H: ");
    display.setTextSize(2);
    display.println(sht30.humidity);
    humiStore[i] = int(sht30.humidity)/2;

    for(int j=0; j<60; j++) {
      display.drawLine(j, 48, j, tempStore[j], WHITE);
    }

    if(i<60) {
      i++;
    }else{
      i = 0;
    }
  }
  else
  {
    display.println("Error!");
  }
  display.display();
  delay(1000);

}
