#include <Arduino.h>
#include <jump.h>
#include <tetris.h>
#include <pacman.h>
#include <heli.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
//#include <snake.h>
#include <pins.h>
#define CHRISTMAS 0
int game = 0;

byte arrowDownChar[] = {
    B00000,
    B00100,
    B00100,
    B00100,
    B00100,
    B10001,
    B01010,
    B00100};
byte arrowUpChar[] = {
    B00100,
    B01010,
    B10001,
    B00100,
    B00100,
    B00100,
    B00100,
    B00000};

void setup()
{
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(LED_WHITE, OUTPUT);
  analogWrite(LED_WHITE, 25);
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_RED, 255);
  analogWrite(LED_YELLOW, 255);
  analogWrite(LED_BLUE, 25);
  Serial.begin(9600);
  Serial.println("defining menu_lcd");
  LiquidCrystal_I2C menu_lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
  // put your setup code here, to run once:
  Serial.println("menu_lcd definded\n setting Button pins to INPUT_PULLUP");
  Serial.println("PINs set\n printing select screen");
  menu_lcd.begin(16, 2);
  menu_lcd.createChar(1, arrowDownChar);
  menu_lcd.createChar(2, arrowUpChar);
  if (CHRISTMAS)
  {
    menu_lcd.print("Frohe");
    menu_lcd.setCursor(0, 1);
    menu_lcd.print("Weihnachten!");
    delay(3000);
  }
  menu_lcd.clear();
  menu_lcd.home();
  menu_lcd.print("   MicroGame    ");
  menu_lcd.setCursor(0, 1);
  menu_lcd.print("   by johuck    ");
  delay(2000);
  menu_lcd.clear();
  menu_lcd.setCursor(0, 0);
  menu_lcd.write(2);
  menu_lcd.print("Pacman    Heli");
  menu_lcd.write(1);
  menu_lcd.setCursor(0, 1);
  menu_lcd.print("<Tetris    Jump>");
  while (game == 0)
  {
    if (digitalRead(BUTTON_RIGHT) == 0)
    {
      game = 1;
      menu_lcd.clear();
      Serial.println("jump_init");
      jump_setup();
    }
    else if (digitalRead(BUTTON_LEFT) == 0)
    {
      game = 2;
      Serial.println("tetris setup : " + game);
      tetris_setup();
    }
    else if (digitalRead(BUTTON_UP) == 0)
    {
      game = 3;
      pacman_setup();
    }
    else if (digitalRead(BUTTON_DOWN) == 0)
    {
      game = 4;
      heli_setup();
    }

    delay(10);
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (game == 1)
  {
    jump_loop();
  }
  else if (game == 3)
  {
    pacman_loop();
  }
  else if (game == 4)
  {
    heli_loop();
  }

  // if (game == 2) {
  //   tetris_loop();
  // }
}