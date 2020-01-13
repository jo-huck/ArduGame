//Pacman
//Written By: Jean Malha
//http://forum.snootlab.com/viewtopic.php?f=34&t=207
//

//#include "Wire.h" // insertion de la librairie I2C (obligatoire)
//#include <Deuligne.h> // insertion de la librairie Deuligne (obligatoire)
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "pacman.h"
#include "../../include/pins.h"

#define VITESSE_PAC 150
#define VITESSE_FANT 2000
#define MAXX 15
#define MAXY 1

// #define BUTTON_RIGHT     3
// #define BUTTON_UP        2
// #define BUTTON_DOWN      4
// #define BUTTON_LEFT      7
#define btnSelect 2
#define btnNone 5

void (*resetFunc)(void) = 0;
LiquidCrystal_I2C pacman_lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void gagne();
void initLevel();
//Deuligne pacman_lcd; // Déclaration de l'objet pacman_lcd

// Charactère spécifique pacman
byte pacman[8] = {
    B00000,
    B00000,
    B01110,
    B11011,
    B11100,
    B01110,
    B00000,
    B00000};

// Charactère spécifique fantome
byte fantome[8] = {
    B00000,
    B00000,
    B01110,
    B10101,
    B11111,
    B11111,
    B10101,
    B00000};

byte point[8] = {
    B00000,
    B00000,
    B00000,
    B01110,
    B01110,
    B00000,
    B00000,
    B00000};

// Tableau des pacman_points à manger
byte pacman_points[MAXX + 1][MAXY + 1];

int xpac = 2;              // Position de pacman en X (colone)
int ypac = 1;              //position de pacmanen y (ligne)
int xfant = 15;            // Position du fantome en X (colone)
int yfant = 0;             // Position du fantome en Y (ligne)
byte light = true;         //Eclairage
long keystruck = 0;        //dernier appui sur un bouton
long poursuite = 0;        //dernier movement du fantome
byte partieEnCours = true; // pour eviter de boucler sur la fin
byte vide = false;         // pour tester si tout est manger

byte level = 0; // niveau
int score = 0;  // niveau

void bouge(int x, int y) // fonction pour bouger pacman
{
  int oldx = xpac;
  int oldy = ypac;
  if (((xpac + x) >= 0) & ((xpac + x) <= MAXX))
    xpac = xpac + x; //Si pas sorti d'ecran, on change x
  if (((ypac + y) >= 0) & ((ypac + y) <= MAXY))
    ypac = ypac + y;                //Si pas sorti d'ecran, on change y
  pacman_lcd.setCursor(xpac, ypac); // On se place en nouvelle position
  pacman_lcd.write(byte(0));        // et on pose le caractere 0 (Pacman)
  pacman_lcd.setCursor(oldx, oldy); // On se place en ancienne position
  if ((xpac != oldx) || (ypac != oldy))
    pacman_lcd.print(" "); // et on efface Pacman (s'il a bougé)
  if (pacman_points[xpac][ypac])
  {
    pacman_points[xpac][ypac] = false; // mange le truc
    score++;
  }
  vide = true;
  for (int i = 0; i <= MAXX; i = i + 1)
    for (int j = 0; j <= MAXY; j = j + 1)
      if (pacman_points[i][j])
        vide = false;
  if ((vide) && (partieEnCours))
    gagne();
}

void perdu()
{
  pacman_lcd.setCursor(0, 0);           // on se place au point 0,0 (1ere ligne, 1er caractere)
  pacman_lcd.print("***Game Over****"); // on écrit le début du texte de début
  pacman_lcd.setCursor(0, 1);           // on se place au point 0,1 (2eme ligne, 1er caractere)
  pacman_lcd.print("***");
  pacman_lcd.print(score);
  pacman_lcd.print("***");
  delay(2000);
  resetFunc();
}

void gagne()
{
  level++;
  pacman_lcd.setCursor(0, 0);             // on se place au point 0,0 (1ere ligne, 1er caractere)
  pacman_lcd.print("*** Next level ***"); // on écrit le début du texte de début
  pacman_lcd.setCursor(0, 1);             // on se place au point 0,0 (1ere ligne, 1er caractere)
  pacman_lcd.print("*** ");
  pacman_lcd.print(level, DEC);
  pacman_lcd.print(" ***"); // on écrit le début du texte de début
  delay(2000);              // 2 secondes de pause
  initLevel();              //reinitialisation du tableau
}

void poursuis() // fonction pour bouger fantome
{
  int oldx = xfant;
  int oldy = yfant;
  if (yfant < ypac)
    yfant = yfant + 1;
  else if (yfant > ypac)
    yfant = yfant - 1;
  else if (xfant < xpac)
    xfant = xfant + 1;
  else if (xfant > xpac)
    xfant = xfant - 1;
  pacman_lcd.setCursor(xfant, yfant);     // On se place en nouvelle position
  pacman_lcd.write(1);                    // et on pose le caractere 0 (Fantome)
  pacman_lcd.setCursor(oldx, oldy);       // On se place en ancienne position
  if ((oldx != xfant) || (oldy != yfant)) // et on efface Fantome (s'il a bougé)
  {
    if (pacman_points[oldx][oldy])
      pacman_lcd.write(2); // remplacé par un point si pas mangé
    else
      pacman_lcd.print(" "); // remplacé par un espace si déja magé
  }
}

//initialisation du tableau
void initLevel()
{
  for (int i = 0; i <= MAXX; i = i + 1)
    for (int j = 0; j <= MAXY; j = j + 1)
    {
      pacman_points[i][j] = true;         //initialisation du tableau des trucs à manger
      pacman_lcd.setCursor(i - 1, j - 1); // on se place au point j,i
      pacman_lcd.write(2);                // on écrit les pacman_points
    }
  pacman_lcd.setCursor(xpac, ypac);   // On se place en position de départ de pacman
  pacman_lcd.write(byte(0));          // et on pose le caractere 0 (Pacman)
  pacman_lcd.setCursor(xfant, yfant); // On se place en position de départ du fantome
  pacman_lcd.write(1);                // et on pose le caractere 1 (fantome)
  poursuite = millis();               // On initialise le timer de poursuite (pour eviter un mouvement immédiat)
  vide = false;
}

int getKey()
{
  //   int b = analogRead(A0);
  //   if (b > 1000) return btnNone;
  //   delay(8);
  //   if (b < 50) return BUTTON_RIGHT;
  //   if (b < 180) return BUTTON_UP;
  //   if (b < 330) return BUTTON_DOWN;
  //   if (b < 520) return BUTTON_LEFT;
  //   if (b < 700) return btnSelect;
  if (digitalRead(BUTTON_DOWN) == 0)
  {
    return BUTTON_DOWN;
  }
  else if (digitalRead(BUTTON_LEFT) == 0)
  {
    return BUTTON_LEFT;
  }
  else if (digitalRead(BUTTON_RIGHT) == 0)
  {
    return BUTTON_RIGHT;
  }
  else if (digitalRead(BUTTON_UP) == 0)
  {
    return BUTTON_UP;
  }
  else
  {
    return btnNone;
  }
  if (btnSelect == 0)
  {
    return btnSelect;
  }
}

void pacman_setup()
{
  Serial.begin(9600);
  //Wire.begin(); // initialisation I2C (obligatoire)
  //pacman_lcd.init(); // initialisation LCD (obligatoire)
  pacman_lcd.begin(16, 2);
  pacman_lcd.createChar(0, pacman);  // creation du caractere pacman et affectation au numéro 0
  pacman_lcd.createChar(1, fantome); // creation du caractere de fantome et affectation au numéro 1
  pacman_lcd.createChar(2, point);   // creation du caractere de point et affectation au numéro 2
  //pacman_lcd.backLight(true); // on allume le retro eclairage
  pacman_lcd.setCursor(0, 0);  // on se place au point 0,0 (1ere ligne, 1er caractere)
  pacman_lcd.print("Pacman!"); // on écrit le début du texte de début
  delay(1000);                 // Splash screen
  initLevel();                 // initialisation du tableau
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
}

void pacman_loop()
{
  int thisChar = Serial.read();
  switch (thisChar)
  {
  case 'r':
    pacman_lcd.scrollDisplayRight();
    break;
  case 'l':
    pacman_lcd.scrollDisplayLeft();
    break;
  }
  if ((thisChar > 'a') & (thisChar < 'z'))
  {
    pacman_lcd.setCursor(1, 1);
    pacman_lcd.write(thisChar);
  }
  if (millis() - keystruck > VITESSE_PAC) // Si plus de 200ms depuis le dernier mouvement de joystick
  {
    int joy = getKey();
    switch (joy)
    {
    case btnNone:
      break;
    case BUTTON_LEFT:
      Serial.print("Pacman bouge à gauche.\n"); // envoi de controle sur liaison série
      Serial.print(keystruck);
      bouge(-1, 0);         // déplacement
      keystruck = millis(); // remise à zero du timer de mouvement
      break;
    case BUTTON_RIGHT:
      Serial.print("Pacman bouge à droite\n"); // envoi de controle sur liaison série
      bouge(1, 0);                             // déplacement
      keystruck = millis();                    // remise à zero du timer de mouvement
      break;
    case BUTTON_UP:
      Serial.print("Pacman bouge en haut\n"); // envoi de controle sur liaison série
      bouge(0, -1);                           // déplacement
      keystruck = millis();                   // remise à zero du timer de mouvement
      break;
    case BUTTON_DOWN:
      Serial.print("Pacman bouge en bas\n");
      bouge(0, 1);          // déplacement
      keystruck = millis(); // remise à zero du timer de mouvement
      break;
      /*case 4:
       Serial.print("centre\n");
       light=!light; //On inverse le statut d'allumage
       pacman_lcd.backLight(light); // on applique
       keystruck=millis(); // remise à zero du timer de mouvement
       break;*/
    default:
      Serial.print(joy);    //au cas ou...
      keystruck = millis(); // remise à zero du timer de mouvement
    };
  };
  if (millis() - poursuite > VITESSE_FANT / (level + 1) + 10)
  {
    poursuis();
    poursuite = millis();
  }
  if ((xpac == xfant) && (ypac == yfant) && (partieEnCours))
  {
    perdu();
  }
}
