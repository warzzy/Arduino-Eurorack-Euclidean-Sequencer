#define BUTTON_PIN 11
#define OUT_PIN 12
#define CLOCK_PIN 2
#define LED LED_BUILTIN

int const leng = 8;
int volatile steps = 0;
int i = 0;
int volatile clki = 0;
int clk = 0;
int arrayOut[leng];
int remainder = 0;
int tempint = 0;
int volatile outBuffer = 0;
 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  Serial.println("I le S  Ai");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(OUT_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT); 
  pinMode(CLOCK_PIN, INPUT); // set pin to input
  attachInterrupt(digitalPinToInterrupt(CLOCK_PIN), onClock, RISING);
  
}

void onClock() {
  outBuffer = arrayOut[clki];
  clki++;
  if (clki==leng) { clki = 0; }
  Serial.println(outBuffer);
}

void fillArray(int arrayIn[leng], int stepsIn) {
  for (int i=0; i<leng; i++) {
    tempint = tempint + steps;
    if (tempint < leng) {
      arrayIn[i] = 0;
    } else {
      arrayIn[i] = 1;
      tempint = tempint % leng;
    }
    }
    delay(10);
    tidyArray(arrayIn);
  }

void tidyArray(int arrayIn[leng]) {
  int first = arrayIn[0];
  if ((steps>0) && (arrayIn[0]!=1)) {
  for (int f=0; f<leng; f++) {
    for (int i=0; i<leng; i++) {
      arrayIn[i] = arrayIn[i+1];
    }
    arrayIn[leng-1] = first;
  if (arrayIn[0]==1) { break; }
    }
  }
}

int printArr(int arrayIn[leng]) {
    i = 0;
    while (i<leng) {
    Serial.print(arrayIn[i]);
    i++;
  }
  Serial.println("");
}

void loop() {
    //printArr(arrayOut);
    //delay(1000);
    fillArray(arrayOut, steps);
    //printArr(arrayOut);
    
  /*fillArray(tempArr,steps);
  delay(10);
  for (int i=0; i<leng; i++) {
    Serial.print(i);
    Serial.print(" ");
    Serial.print(leng);
    Serial.print("  ");
    Serial.print(steps);
    Serial.print("  ");
    Serial.println(arrayOut[i]);
    delay(1000);
  }*/
  

}











/*void loop() {
  // put your main code here, to run repeatedly:
/*  bool BUTTON = digitalRead(BUTTON_PIN);
  if (BUTTON) {
    digitalWrite(LED, HIGH);
    digitalWrite(OUT_PIN, HIGH);
  } else {
    digitalWrite(LED, LOW);
    digitalWrite(OUT_PIN, LOW);
  }
  bool  onC = digitalRead(CLOCK_PIN); 
  if ( onC ) {
     digitalWrite(LED, HIGH);
     Serial.print("1");
    digitalWrite(OUT_PIN, HIGH);
  } else {
    digitalWrite(LED, LOW);
    Serial.print("0");
    digitalWrite(OUT_PIN, LOW);   
  }

}
*/
