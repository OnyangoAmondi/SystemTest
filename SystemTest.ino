#include <ESP8266WiFi.h>
#define LED_pin D2
#define LED_WiFi D7
#define noiseRemoval 1000

const byte interruptPin = D5;
volatile boolean interrupt_occurred = false;

int pCount = 0; 
int wiFiState = 0; //shows if Wi-Fi is connected
long int tNow = 0;
long int tLast = 0;
float energy = 0.00;


//Wi-Fi Connection
const char* ssid = "Armondri";
const char* password = "Jillo@2023";




void ISR_Counter();
void WiFiState();


void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }

   
  // put your setup code here, to run once:
  
  pinMode(LED_pin, OUTPUT);
  digitalWrite(LED_pin, LOW);

  pinMode(LED_WiFi, OUTPUT);
  

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), ISR_Counter, FALLING); //responds to falling edges of pin
  
}

void loop() {
  // put your main code here, to run repeatedly:
   if (WiFi.status() != WL_CONNECTED){
  wiFiState = 1;
  digitalWrite(LED_WiFi, wiFiState); //indicate wifi connected
} else {
  Serial.println("Disconnected from WiFi access point");
  wiFiState = 0;
  digitalWrite(LED_WiFi,wiFiState); //indicate wifi not connected -  BLUE LED
   Serial.println("Trying to Reconnect"); 
   delay(10000);
    WiFi.begin(ssid, password);
}
    energy = pCount/1600.00;
    Serial.print("Count : ");  
    Serial.print(pCount);
    Serial.print(" : Energy : ");  
    Serial.print(energy, 2);
    Serial.println(" Kwh");
  
    
      
  }



void IRAM_ATTR ISR_Counter() {
  // The IRAM_ATTR attribute places compiled code in the (faster) Internal RAM (IRAM) of the ESP8266.
    digitalWrite(LED_pin, HIGH);
    tNow = millis();
  if(interrupt_occurred = true && tNow - tLast > noiseRemoval){
    pCount++; //increment count
  }
     tLast = tNow;
}

