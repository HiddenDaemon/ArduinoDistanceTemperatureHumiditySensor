/*
  ARDUINO MULTI-SENSOR DASHBOARD

  Created by: HiddenDaemon
  Last update: february (2026)

  Used hardware:
    Arduino Mega 2560
    DHT11 -> Temperature and humidity connected to pin A0
    Ultrasonic sensor -> Distance connected to pins 8, 9
    LCD 1602 connected to pins 22, 23, 24, 25, 26, 27
    Buttons (3) connected to pins 5, 6, 7
*/

#include <LiquidCrystal.h> //Library for the 16x2 LCD 1602
#include <DHT11.h> // Library for the DHT11 sensor

// MODIFY THE PINS FOR YOUR OWN PINS

const int DHT11pin = A0;
const int DistanceButton = 5;
const int TemperatureButton = 6;
const int HumidityButton = 7;

// MODIFY: Ultrasound sensor pins
const int trig = 9;
const int echo = 8;

// MODIFY: LCD pins connection
const int rs = 22, en = 23, d4 = 24, d5 = 25, d6 = 26, d7 = 27; 
// DHT11 starts at port assigned to DHT11pin variable
DHT11 dht11(DHT11pin);


// Buttons configuration
const int places = 3;
const int buttons[places] = {HumidityButton, TemperatureButton, DistanceButton};
int actualPlace = 0; // Used to define which screen show


// This is used for debounce function, prevents fake reads from buttons
unsigned long lastChange = 0;
const unsigned long debounceTime = 50; // Time in milliseconds



//LCD pins init
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


// Temporizador para actualizaciones
unsigned long lastUpdate = 0;
const unsigned long updateTime = 1000;


void setup() {
  lcd.begin(16, 2);
  // Starts buttons assigned to the array
  for (int i = 0; i < places; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }
  // Defines ultrasound sensor
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // First screen init
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
 
  // Updates the zone depends witch button is pressed with debounce function
  if (millis() - lastUpdate >= updateTime) {
    switch(actualPlace) {
      case 0: showHumidity(); break;
      case 1: showTemperature(); break;
      case 2: showDistance(); break;
    }
    lastUpdate = millis();
  }
}

//Function to show humidity data
void showHumidity() { 
  int humedad = dht11.readHumidity();
  lcd.setCursor(0, 0);
  lcd.print("Humedad:");
  lcd.setCursor(0, 1);
  lcd.print(humedad);
  lcd.print(" %  ");
}

// Function to show temperature data
void showTemperature() { 
  float temperature = dht11.readTemperature();
  lcd.setCursor(0, 0);
  lcd.print("temperature:");
  lcd.setCursor(0, 1);
  lcd.print(temperature, 1);
  lcd.print(" C  ");
}

// Function to show distance data
void showDistance() { 
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  // Receives and calculates data to cm
  long duration = pulseIn(echo, HIGH, 30000);
  int distance = duration * 0.034 / 2;


  lcd.setCursor(0, 0);
  lcd.print("Distance:  ");
  lcd.setCursor(0, 1);
  // Prints the data
  if (distance >= 2 && distance <= 400) {
    lcd.print(distance);
    lcd.print(" cm ");
  } else {
    lcd.print("No range");
  }
}
