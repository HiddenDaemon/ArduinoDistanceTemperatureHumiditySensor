#include <LiquidCrystal.h> //Library for the 16x2 LCD 1602
#include <DHT11.h> // Library for the DHT11 sensor


DHT11 dht11(A0);


// Buttons configuration
const int places = 3;
const int buttons[places] = {7, 6, 5};
int actualPlace = 0; // Start showing the first screen


// This is used for debounce function, prevents fake reads
unsigned long lastChange = 0;
const unsigned long debounceTime = 50;


// Ultrasound sensor
const int trig = 9;
const int echo = 8;


// LCD pins connection
const int rs = 22, en = 23, d4 = 24, d5 = 25, d6 = 26, d7 = 27; 
//LCD pins init
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


// Temporizador para actualizaciones
unsigned long lastUpdate = 0;
const unsigned long updateTime = 1000;


void setup() {
  lcd.begin(16, 2);
  for (int i = 0; i < places; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }
  //Ultrasonido
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  //Pantalla inicio
  lcd.print("Press a");
  lcd.setCursor(0, 1);
  lcd.print("button");
  delay(2000);
}


void loop() {
  // Button detection
  for (int i = 0; i < places; i++) {
    if (digitalRead(buttons[i]) == LOW && (millis() - lastChange > debounceTime)) {
      actualPlace = i;
      lastChange = millis();
      lcd.clear();
    }
  }
 
  // Updates the zone depends witch button is pressed
  if (millis() - lastUpdate >= updateTime) {
    switch(actualPlace) {
      case 0: showHumidity(); break;
      case 1: showTemperature(); break;
      case 2: showDistance(); break;
    }
    lastUpdate = millis();
  }
}


void showHumidity() { //Function to show humidity data
  int humedad = dht11.readHumidity();
  lcd.setCursor(0, 0);
  lcd.print("Humedad:");
  lcd.setCursor(0, 1);
  lcd.print(humedad);
  lcd.print(" %  ");
}


void showTemperature() { // Function to show temperature data
  float temperature = dht11.readTemperature();
  lcd.setCursor(0, 0);
  lcd.print("temperature:");
  lcd.setCursor(0, 1);
  lcd.print(temperature, 1);
  lcd.print(" C  ");
}


void showDistance() { // Function to show distance data
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  // Receives and calculates data to cm
  long duration = pulseIn(echo, HIGH, 30000);
  int distance = duration * 0.034 / 2;


  lcd.setCursor(0, 0);
  lcd.print("Distancia:  ");
  lcd.setCursor(0, 1);
  // Prints the data
  if (distance >= 2 && distance <= 400) {
    lcd.print(distance);
    lcd.print(" cm ");
  } else {
    lcd.print("Out range");
  }
