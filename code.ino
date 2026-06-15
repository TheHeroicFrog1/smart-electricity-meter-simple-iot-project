// src/main.ino

#define BLYNK_TEMPLATE_ID "TMPL3jYiv8GF5"
#define BLYNK_TEMPLATE_NAME "smart electric meter"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN_HERE" 

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <PZEM004Tv30.h>

// Initialize PZEM on hardware serial
PZEM004Tv30 pzem(&Serial);

char auth[] = BLYNK_AUTH_TOKEN;

// WiFi Credentials
char ssid[] = "YOUR_WIFI_SSID";         
char pass[] = "YOUR_WIFI_PASSWORD";     

// Sensor Values
float voltage = 0;
float currentA = 0;
float powerW = 0;
float energyKwh = 0;
float energyWh = 0;
float frequency = 0;
float pf = 0;

// Billing Configuration
float rate = 6.62; // ₹ per kWh
float bill = 0;

unsigned long lastMillis = 0;

void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.println("Connecting to Blynk...");
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
}

void loop() {
  Blynk.run();

  // Read Voltage
  voltage = pzem.voltage();
  if (!isnan(voltage)) {
    Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
  } else {
    voltage = 0;
  }

  // Read Current
  currentA = pzem.current();
  if (!isnan(currentA)) {
    Serial.print("Current: "); Serial.print(currentA); Serial.println("A");
  } else {
    currentA = 0;
  }

  // Read Power
  powerW = pzem.power();
  if (!isnan(powerW)) {
    Serial.print("Power: "); Serial.print(powerW); Serial.println("W");
  } else {
    powerW = 0;
  }

  // Read Energy
  energyKwh = pzem.energy();
  if (!isnan(energyKwh)) {
    energyWh = energyKwh * 1000.0;   // convert kWh → Wh
    Serial.print("Energy: "); Serial.print(energyWh); Serial.println(" Wh");
  } else {
    energyKwh = 0;
    energyWh = 0;
  }

  // Read Frequency
  frequency = pzem.frequency();
  if (!isnan(frequency)) {
    Serial.print("Frequency: "); Serial.print(frequency); Serial.println(" Hz");
  } else {
    frequency = 0;
  }

  // Read Power Factor
  pf = pzem.pf();
  if (!isnan(pf)) {
    Serial.print("PF: "); Serial.println(pf);
  } else {
    pf = 0;
  }

  // Billing calculation
  bill = energyKwh * rate;
  Serial.print("Bill: ₹"); Serial.println(bill);

  Serial.println();
  delay(2000);

  // Non-blocking upload to Blynk every 5 seconds
  if (millis() - lastMillis > 5000) {
    lastMillis = millis();

    Blynk.virtualWrite(V1, voltage);
    Blynk.virtualWrite(V2, currentA);
    Blynk.virtualWrite(V3, powerW);
    Blynk.virtualWrite(V4, energyWh);  // WH
    Blynk.virtualWrite(V5, frequency);
    Blynk.virtualWrite(V6, pf);
    Blynk.virtualWrite(V8, energyKwh); // KWH
    Blynk.virtualWrite(V7, bill);      // Total Bill in INR
  }
}