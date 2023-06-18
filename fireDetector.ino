#include <DHT.h>
#include <ArduinoJson.h>
#include <CTBot.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp8266.h>
#define mq2 A0
#define pinred D0
#define pingreen D2
// Blyk
#define BLYNK_TEMPLATE_ID "TMPL6VVu9vMjV"
#define BLYNK_TEMPLATE_NAME "fire detection"
#define BLYNK_AUTH_TOKEN "a_gT5kG-QDIDyzv493Z0Tqr19Uq_9jQD"
#define BLYNK_PRINT_Serial

// Telegram
CTBot mybot;
char token[] = "5876345539:AAEOqNumfwBcJa9WrkMuMRuCNKUmqiYkScw";
const int id  = 1506670485;

// WIFI
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "7x - 10y + xy";
char pass[] = "itungaja";

// dht, buzzerpin
int buzzerpin = D7;
int notif;
DHT dht(D1, DHT11);
BlynkTimer timer;

BLYNK_WRITE(V2){
  int pinNotif = param.asInt();
  if (pinNotif==1){
    Serial.println("Notifikasi Aktif");
    notif=1;
  }else{
    Serial.println("Notifikasi Nonaktif");
    notif=0;
  }
}

void sendSensor() {
  int nilai = analogRead(mq2);
  float temp = dht.readTemperature();
  Blynk.virtualWrite(V0, nilai);
  Blynk.virtualWrite(V1, temp);
  Serial.print("Ketebalan asap: ");
  Serial.println(nilai);
  Serial.print("Suhu: ");
  Serial.print(temp);
  Serial.println(" Celcius");
  
  if (notif==1){
    if (nilai > 25 && temp > 30) {
      digitalWrite(pinred, HIGH);
      mybot.sendMessage(id,"LARII !!!!!!!!");
      tone(buzzerpin, 20, 1000);
      digitalWrite(pingreen, LOW);
      Blynk.virtualWrite(V3,"LARII !!!!!!!!");
    } else {
      digitalWrite(pingreen, HIGH);
      noTone(buzzerpin);
      mybot.sendMessage(id,"SANTAI DULU GA SIE :V");
      digitalWrite(pinred, LOW);
      Blynk.virtualWrite(V3,"SANTAI DULU GA SIE :V");
    }
  }else {
    Blynk.virtualWrite(V3," ");
    if (nilai > 25 && temp > 30) {
      digitalWrite(pinred, HIGH);
      digitalWrite(pingreen, LOW);
    } else {
      digitalWrite(pingreen, HIGH);
      digitalWrite(pinred, LOW);
    }
  }
}

void setup(){
  Serial.begin(9600);
  pinMode(mq2, INPUT);
  pinMode(pinred, OUTPUT);
  pinMode(pingreen, OUTPUT);
  pinMode(buzzerpin, OUTPUT);
  dht.begin();
  Blynk.begin(auth, ssid, pass);
  mybot.wifiConnect(ssid,pass);
  mybot.setTelegramToken(token);
  timer.setInterval(2500L, sendSensor);
}

void loop(){
  delay(10); // this speeds up the simulation
  sendSensor();
  Blynk.run();
  timer.run();
}

