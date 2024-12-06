#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include<DHT.h>
DHT  dht(D3,DHT11) ; //dht is variable
const int soilMoisturePin = A0;
const int numReadings = 20;
float moistureReadings[numReadings];
float moisturePercentage=0;
RF24 radio(D1,D2);  // CE and CSN pins
const byte address[6] = "00001";
const uint8_t channel = 100;  // Set the desired RF channel
struct sensor
{
  int soil1;
  float celcius;
  float faheren;
  float humidity;
  int pir;
  byte  sensorNum;

};
sensor soilreading3;
void setup() {
  pinMode(soilMoisturePin,INPUT);
  pinMode(D0,INPUT);//pir
  pinMode(D3,INPUT);////temp and humidity
  dht.begin();
  radio.begin();
  radio.openWritingPipe(address);
  radio.setChannel(channel);   // Set the RF channel
  radio.setPALevel(RF24_PA_LOW); // Adjust power level if needed
  radio.setDataRate(RF24_250KBPS);//RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  Serial.begin(9600);
  soilreading3.sensorNum =3;
  Serial.println("Transmitter3 setup complete");

}
void loop() {
  float totalMoisture = 0;
  // Read and accumulate multiple readings
  for (int i = 0; i < numReadings; i++) {
    moistureReadings[i] = analogRead(soilMoisturePin);
    totalMoisture += moistureReadings[i];
    delay(50); // Add a short delay between readings
  }
  Serial.println(analogRead(soilMoisturePin));
  // Calculate the average
  float averageMoisture = totalMoisture / numReadings;
  // Map the average to a percentage
   moisturePercentage = map(averageMoisture,800,1024, 100, 0);
   soilreading3.soil1=moisturePercentage ;
    radio.write(&soilreading3, sizeof(soilreading3));
 Serial.print("Average Soil Moisture3: ");
  Serial.print(moisturePercentage);
  Serial.println("%");
  //for pir reading 
  int pirreading=digitalRead(D0);
  float t=dht.readTemperature();//celsius
soilreading3.celcius=t;
float tf=dht.readTemperature(true);//fahren
soilreading3.faheren=tf;
float h=dht.readHumidity();
soilreading3.humidity=h;
soilreading3.pir=pirreading;
Serial.print("temp in cel is =");
Serial.println(t);
Serial.print("temp in fahren is =");
Serial.println(tf);
Serial.print("humidity is =");
Serial.println(h);
Serial.print("pir reading  is =");
Serial.println(pirreading);
delay(10); 
}

