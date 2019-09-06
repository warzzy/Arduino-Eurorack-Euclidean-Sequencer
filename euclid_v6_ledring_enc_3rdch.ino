#include <Bounce2.h>
#include <Rotary.h>
#include <Adafruit_NeoPixel.h>

#define OUTA 12
#define OUTB 10
#define OUTC 11
#define CLOCK_PIN 2
#define encCLK 4
#define encDT 3
#define encBTN 5
#define RINGPIN 6

//LEDRING COLOR
#define seqAvivid ledring.Color(1,15,0)
#define seqAdim ledring.Color(1,1,0)
#define cursorA ledring.Color(5,15,0)

#define seqBvivid ledring.Color(0,1,15)
#define seqBdim ledring.Color(0,1,1)
#define cursorB ledring.Color(0,15,5)

#define seqCvivid ledring.Color(10,0,20)
#define seqCdim ledring.Color(1,0,1)
#define cursorC ledring.Color(20,0,10)


//SEQ PARAMETEREK
int const maxLeng = 16;

//SEQA
int lengA = 0;
int volatile stepsA = 0;
int volatile clkA = 0;
int seqA[maxLeng];

//SEQB
int lengB = 0;
int volatile stepsB = 0;
int volatile clkB = 0;
int seqB[maxLeng];

//SEQC
int lengC = 0;
int volatile stepsC = 0;
int volatile clkC = 0;
int seqC[maxLeng];

//SEGEDVALTOZOK
int i = 0;
int tempint = 0;
int serialin = 0;

//ENCODER PARAMETEREK
int volatile encAMT =0;
bool encBTNstate = LOW;
unsigned char enc1DIR;
Bounce encBTNbouncer = Bounce ();

//MENU PARAMETEREK
int menuState = 0;
int maxMenuState = 5;

//NEOPIXEL
Adafruit_NeoPixel ledring(16, RINGPIN, NEO_GRB + NEO_KHZ800);

 

void setup() {
  Serial.begin(9600); 
  ledring.begin();
  pinMode(OUTA, OUTPUT);
  pinMode(OUTB, OUTPUT);
  pinMode(OUTC, OUTPUT);
  pinMode(RINGPIN, OUTPUT);
  pinMode(CLOCK_PIN, INPUT); 
  pinMode(encDT, INPUT);
  pinMode(encCLK, INPUT);
  encBTNbouncer.attach(encBTN,INPUT_PULLUP); // Attach the debouncer to a pin with INPUT_PULLUP mode
  encBTNbouncer.interval(25);
  attachInterrupt(digitalPinToInterrupt(CLOCK_PIN), onClock, RISING);
  attachInterrupt(digitalPinToInterrupt(encDT), encTick, FALLING);
  
}

int encTick() {
  int encState = digitalRead(encDT); 
  if (encState != digitalRead(encCLK)) { //CW
    encAMT = 1;
    return;
  } else if (encState == digitalRead(encCLK)) { //CCW
    encAMT = -1;
    return;
  } else {
    encAMT = 0;
    return;
  }
  
}

void onClock() {
  //seqA
  if (clkA>=lengA) { clkA = 0; }
  digitalWrite(OUTA,seqA[clkA]);
  delay(10);
  digitalWrite(OUTA,0);
  Serial.println(seqA[clkA]);
  clkA++;
  
  //seqB
  if (clkB>=lengB) { clkB = 0; }
  digitalWrite(OUTB,seqB[clkB]);
  delay(10);
  digitalWrite(OUTB,0);
  Serial.println(seqB[clkB]);
  clkB++;
  
  //seqC
  if (clkC>=lengC) { clkC = 0; }
  digitalWrite(OUTB,seqC[clkC]);
  delay(10);
  digitalWrite(OUTC,0);
  Serial.println(seqB[clkC]);
  clkC++;

}


void loop() {
    Serial.println(" ");
    Serial.print("MENU: ");
    Serial.print(menuState);
    Serial.print(" encAMT: ");
    Serial.print(encAMT);
    Serial.print(" stepsA: ");
    Serial.print(stepsA);
    Serial.print(" lengA: ");
    Serial.print(lengA);
    Serial.print(" stepsB: ");
    Serial.print(stepsB);
    Serial.print(" lengB: ");
    Serial.print(lengB);
    Serial.print(" stepsC: ");
    Serial.print(stepsC);
    Serial.print(" lengC: ");
    Serial.print(lengC);
    Serial.print(" ARR: ");
    
    
    
    // LEDRING DRIVER
    ledring.clear();
    if ((menuState == 0) || (menuState == 1)) { 
      for ( i=0; i<lengA; i++) {
        if (i == clkA) {
          ledring.setPixelColor(i, cursorA);
        } else {
         if (seqA[i] == 1) {
            ledring.setPixelColor(i, seqAvivid);
          } else {
            ledring.setPixelColor(i, seqAdim);
          }
        } 
      }
    } else if ((menuState == 2) || (menuState == 3)) {
      for ( i=0; i<lengB; i++) {
        if (i == clkB) {
          ledring.setPixelColor(i, cursorB);
        } else {
         if (seqB[i] == 1) {
            ledring.setPixelColor(i, seqBvivid);
          } else {
            ledring.setPixelColor(i, seqBdim);
          }
        } 
      }
    } else if ((menuState == 4) || (menuState == 5)) {
      for ( i=0; i<lengC; i++) {
        if (i == clkC) {
          ledring.setPixelColor(i, cursorC);
        } else {
         if (seqC[i] == 1) {
            ledring.setPixelColor(i, seqCvivid);
          } else {
            ledring.setPixelColor(i, seqCdim);
          }
        } 
      }
  }
  ledring.show();


   //HANDLING THE MENU BUTTON
   encBTNbouncer.update();
    if (encBTNbouncer.fell()) {
      if (menuState < maxMenuState) {
          menuState++;
          encAMT = 0;
        } else {
          menuState = 0;
          encAMT = 0;
        }
    }

    switch (menuState) {    //Menüvizsgálat
      //SEQUENCE A
      case 0:
        stepsA = stepsA + encAMT;
        encAMT = 0;
        stepsA = constrain(stepsA, 0, lengA);  //stepsA sose legyen nagyobb lengA-nál
        delay(50);
        break;
      case 1:
        lengA = lengA + encAMT;
        encAMT = 0;
        lengA = constrain(lengA, 0, maxLeng);
        delay(50);
        break;

      //SEQUENCE B  
      case 2:
        stepsB = stepsB + encAMT;
        encAMT = 0;
        stepsB = constrain(stepsB, 0, lengB);  //stepsB sose legyen nagyobb lengB-nél
        delay(50);
        break;
      case 3:
        lengB = lengB + encAMT;
        encAMT = 0;
        lengB = constrain(lengB, 0, maxLeng);
        delay(50);
        break;

      //SEQUENCE C  
      case 4:
        stepsC = stepsC + encAMT;
        encAMT = 0;
        stepsC = constrain(stepsC, 0, lengC);  //stepsC sose legyen nagyobb lengB-nél
        delay(50);
        break;
      case 5:
        lengC = lengC + encAMT;
        encAMT = 0;
        lengC = constrain(lengC, 0, maxLeng);
        delay(50);
        break;
        
    }
    if (stepsA > lengA ) { stepsA = lengA; }  //lengA csökkentésnél előfordulhat, hogy stepsA nagyobb, félreszámolást elkerülendő
    if (stepsB > lengB ) { stepsB = lengB; }
    if (stepsC > lengC ) { stepsC = lengC; }
    fillArray(seqA, stepsA, lengA);
    fillArray(seqB, stepsB, lengB);
    fillArray(seqC, stepsC, lengC);


    
}


//KIEGÉSZÍTŐ FÜGGVÉNYEK
int printArr(int arrayIn[maxLeng]) {
    i = 0;
    while (i<maxLeng) {
    Serial.print(arrayIn[i]);
    i++;
  }
  Serial.println("");
} 

void fillArray(int arrayIn[maxLeng], int stepsIn, int lengIn) {
  for (int i=0; i<lengIn; i++) {
    tempint = tempint + stepsIn;    //HOZZÁADJUK A BEATEK SZÁMÁT EGY IDEIGLENES VÁLTOZÓHOZ
    if (tempint < lengIn) {      //HA A VÁLTOZÓ KISESEB AZ ÜTEM HOSSZÁNÁL 
      arrayIn[i] = 0;             //AKKOR ITT NINCS ÜTÉS
    } else {
      arrayIn[i] = 1;             //HA NAGYOBB, AKKOR VAN ÜTÉS
      tempint = tempint % lengIn;  //EBBEN AZ ESETBEN A TEMPINT ÉS AZ ÜTEM HOSSZ HÁNYADOSÁNAK MARADÉKÁT VESSZÜK
    }
    }
    //FÜGGVÉNYRENDEZÉS
    int first = arrayIn[0]; //AZ ELSŐ ELEM ELMENTÉSE EGY VÁLTOZÓBA
  if ((stepsIn>0) && (arrayIn[0]!=1)) { //HA A BEATEK SZÁMA NAGYOBB MINT 0, ÉS AZ ELSŐ ELEME A TÖMBNEK 0, ELKEZDJÜK A RENDEZÉST
  for (int f=0; f<lengIn; f++) {
    for (int i=0; i<lengIn; i++) {
      arrayIn[i] = arrayIn[i+1];  //MINDEN ELEMET A BALRA TŐLE LÉVŐBE MÁSOLUNK
    }
    arrayIn[lengIn-1] = first; //A TÖMB UTOLSÓ ELEMÉNEK BEMÁSOLJUK A KIMENTETT ELSŐ ELEMET, ÍGY AZ NEM VESZIK EL A FOLYAMATBAN
  if (arrayIn[0]==1) { break; } //ADDIG FOLYTATJUK, AMÍG A TÖMB ELSŐ ELEME 1 NEM LESZ
    }
  } 
}









