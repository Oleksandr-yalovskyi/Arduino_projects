/*
Created by Oleksandr Yalovskyi, based on Arduino UNO
Modules used: piezoelectric buzzer, OLED screen SSD1306, Dot Matrix max7219, analog joystick, bread board
*/

#include <MD_MAX72xx.h>
#include <Buzzer.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define MAX_DEVICES 4

const int maxX = MAX_DEVICES * 8 - 1;
const int maxY = 7;

#define CLK_PIN  13
#define DATA_PIN 11
#define CS_PIN   10

#define VERT_PIN A0
#define HORZ_PIN A1
#define SEL_PIN  2

#define BUZZ_PIN 8

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

MD_MAX72XX mx = MD_MAX72XX(MD_MAX72XX::PAROLA_HW, CS_PIN, MAX_DEVICES);

int x = 0; 
int y = 0;

unsigned int clear_counter = 0;

void setup() {
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, MAX_INTENSITY / 2);
  mx.clear();

  drawName();
  display.display();

  pinMode(VERT_PIN, INPUT);
  pinMode(HORZ_PIN, INPUT);
  pinMode(SEL_PIN, INPUT_PULLUP);
}

void loop() {

  int horizontal = analogRead(HORZ_PIN);
  int vertical   = analogRead(VERT_PIN);

  if (vertical < 300) {
    y = min(y + 1, maxY);
  }

  if (vertical > 700) {
    y = max(y - 1, 0);
  }

  if (horizontal > 700) {
    x = min(x + 1, maxX);
  }

  if (horizontal < 300) {
    x = max(x - 1, 0);
  }

  if (digitalRead(SEL_PIN) == LOW) {
    mx.clear();
    clear_counter += 1;
    tone(BUZZ_PIN, 262, 250);
    display.clearDisplay();
    drawClearCounter();
    display.display();
  }

  mx.setPoint(y, x, true);
  mx.update();
  delay(100);

}

void drawName(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(2, 2);
  display.print(F("Draw on me"));
} 

void drawClearCounter(){
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(2, 2);
  display.print(F("Board cleared for: "));
  display.println(clear_counter);
  display.println(F(" times"));
}
