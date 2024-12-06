#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
RF24 radio(D1,D2);  // CE and CSN pins
const byte address[6] = "00001";
const uint8_t channel = 100;  // Set the desired RF channel
float soilreading1=0;
int motor=D0;
int buzzer=D3;
float temp;
float humidity;
float temp_feh;
int pirsensor;
//blynk
#define BLYNK_TEMPLATE_ID "TMPL3hgsyQ0ju"
#define BLYNK_TEMPLATE_NAME "SMS"
#define BLYNK_AUTH_TOKEN "oRGrZQLvIenI0Gea8q-cq40RJ4xbRvp7"
#include<ESP8266WiFi.h>
#include<BlynkSimpleEsp8266.h>
char auth[]=BLYNK_AUTH_TOKEN;
char wifi[]="One Plus 11R";
char wifipass[]="shivam12345"; 
BlynkTimer timer;
BLYNK_WRITE (V1){
soilreading1 = param.asInt( ) ;
}
BLYNK_WRITE (V2){
temp = param.asInt( ) ;
}
BLYNK_WRITE (V3){
humidity = param.asInt( ) ;
}

BLYNK_WRITE (V4){
  pirsensor = param.asInt( ) ;
}
BLYNK_WRITE (V5){
temp_feh = param.asInt( ) ;
}
BLYNK_WRITE (V0){
 int value = param.asInt( ) ;
 if(value==1)
 {
 digitalWrite(motor,HIGH);
 }
 else{
  digitalWrite(motor,LOW);
 }
}
void mytimer(){
  Blynk.virtualWrite(V1,soilreading1);
  Blynk.virtualWrite(V2,temp);
   Blynk.virtualWrite(V3,humidity);
   Blynk.virtualWrite(V4,pirsensor);
   Blynk.virtualWrite(V5,temp_feh);
}
struct sensor
{
  int soil;
  float celcius;
  float faheren;
  float humidity;
  int pir;
  byte  sensorNum;

};
sensor sensorData;
void setup() {
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setChannel(channel);   // Set the RF channel
  radio.setPALevel(RF24_PA_LOW);  // Adjust power level if needed
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();
  Serial.begin(9600);
  Blynk.begin(auth,wifi,wifipass);
  timer.setInterval(100L, mytimer);
  pinMode(motor,OUTPUT);
  pinMode(D3,OUTPUT);
   digitalWrite(buzzer,0);
  Serial.println("Receiver setup complete");
}

void loop() {
  digitalWrite(buzzer,0);
  if(radio.available())
  {
    radio.read(&sensorData, sizeof(sensorData));
    disp_sensor_data();
  }

          //for automation for tommato crop
    // if(sensorData.soil<60){
    //   digitalWrite(D0,1);
    // }
    // if(sensorData.soil >75){
    //    digitalWrite(D0,0);
    // }
   Blynk.run();
    timer.run();
    delay(100);
}
void disp_sensor_data()
{
   if(sensorData.sensorNum == 3)
  {
   soilreading1=sensorData.soil;
   temp=sensorData.celcius;
   humidity=sensorData.faheren;
   temp_feh=sensorData.humidity;
   pirsensor=sensorData.pir;
    if(pirsensor==1){
      digitalWrite(buzzer,1); 
    }
    else{
      digitalWrite(buzzer,0); 
    }
   Serial.print("1 soil data=  ");
   Serial.println(sensorData.soil);
Serial.print("temp in cel is =");
Serial.println(temp);
Serial.print("temp in fahren is =");
Serial.println(temp_feh);
Serial.print("humidity is =");
Serial.println(humidity);
Serial.print("pir sensor data =");
Serial.println( pirsensor);
  }
  
}