#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int p1 = 0;
int p2 = 0;

int lives = 3;

void setup() {
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen
  // Clear the buffer
  display.clearDisplay();  
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(0,5);
  display.println("PONG X2");
  display.setCursor(0,30);
  display.println("BY MAKER75");
  display.display();
  delay(1000);
  
}

int x = 50;
int y = 20;
int incx = 1;
int incy = 1;
int gioco;

int padw;
int pad1y = (display.height() + padw) / 2;
int pad2y = (display.height() + padw) / 2;
int padx = (display.width() + padw) / 2;

void loop() {
  display.clearDisplay(); 

  randomSeed(A3);

  gioco = analogRead(A2);

  //pong due giocatori
  if (gioco<=512){
  padw=15;
  //pad1:
  pad1y = analogRead(A0);
  pad1y = map(pad1y, 0, 1024, 0, display.height() - padw);
  display.fillRect(display.width() - 10 ,pad1y, 5, padw, INVERSE);
  //endpad

//pad2:
  pad2y = analogRead(A1);
  pad2y = map(pad2y, 0, 1024, 0, display.height() - padw);
  display.fillRect(5 ,pad2y, 5, padw, INVERSE);
  //endpad
  

  display.fillRect(x, y, 5, 5, INVERSE);//palla

  //aggiorna i punteggi
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println(p2);

  display.setCursor(90, 0);
  display.println(p1);
   
  display.display();
  
  //delayMicroseconds(50);

  //collisioni
  if (x > (display.width() - 15)) {
    //tocca il pad
    if ((y > pad1y) && (y < pad1y + padw)) {
      incx = -incx;  
      //Serial.println(".");
    }   
  } 
  if (x < (10)) {
    //tocca il pad
    if ((y > pad2y) && (y < pad2y + padw)) {
      incx = -incx;  
      //Serial.println(".");
    }   
  } 

  //tocca il fondo
  if (x >= (display.width() - 5)) {
    p2++;  
    //riposiziona la palla a caso
    y = random(10, display.height()- 10);
    x = 64;
    delay(1000);
   
  }
  if (x < 4) {
    p1++;  
    //riposiziona la palla a caso
    y = random(10, display.height()- 10);
    x = 64;
    delay(1000);
  }
  if (p1==10){
    display.clearDisplay();
    display.setCursor(10,0);
    display.println("Vince P1");
    display.setTextSize(1);
     display.setCursor(25,35);
      display.println("premere reset");
      display.setCursor(25,45);
      display.println("per ricominciare");
      display.display();
      while(1);
    }
     if (p2==10){
    display.clearDisplay();
    display.setCursor(10,0);
    display.println("Vince P2");
    display.setTextSize(1);
     display.setCursor(25,45);
      display.println("premere reset");
      display.display();
      while(1);
    }
  //movimento della palla
  x = x + incx;
  if ((x > display.width() - 5) || (x < 0)) {
    incx = -incx;
  }
  y = y + incy;
  if ((y > display.height() - 5) || (y < 0)) {
    incy = -incy;
  }
  }

  //pong giocatore singolo
  if (gioco>512){//pad:
  padw=30;
  padx = analogRead(A1);
  padx = map(padx, 0, 1024, 0, display.width() - padw);
  display.fillRect(padx, display.height() - 10, padw, 5, INVERSE);
  //endpad

  display.fillRect(x, y, 5, 5, INVERSE);

  //aggiorna i punteggi
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println(lives);

  display.setCursor(70, 0);
  display.println(p1);
   
  display.display();
  
  //delayMicroseconds(50);

  //collisioni
  if (y > (display.height() - 15)) {
    //tocca il pad
    if ((x > padx) && (x < padx + padw)) {
      incy = -incy;  
      //Serial.println(".");
    }   
  } 

  //tocca il fondo
  if (y >= (display.height() - 5)) {
    lives--;  
    //riposiziona la palla a caso
    x = random(10, display.width()- 10);
    y = 10;
    delay(1000);
    if (lives == -1) {
      display.clearDisplay();
      display.setCursor(10,0);
      display.println("GAME OVER");
      display.setTextSize(1);
      display.setCursor(25,35);
      display.println("Punteggio");
      display.setCursor(90,35);
      display.println(p1);
      display.setCursor(80,35);
      display.write(3);
      display.setCursor(25,45);
      display.println("premere reset");
      display.display();
    
      while(1);  
    }
  }

  //movimento della palla
  x = x + incx;
  if ((x > display.width() - 5) || (x < 0)) {
    incx = -incx;
  }
  y = y + incy;
  if ((y > display.height() - 5) || (y < 0)) {
    incy = -incy;
  }

  //tocca la cima
    if ((incy > 0) && (y == 0)) p1++;

  }
  }

  //tocca la cima
