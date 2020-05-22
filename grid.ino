#include <ESP8266WebServer.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>

#include <ThingSpeak.h>



#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1015 ads;     /* Use thi for the 12-bit version */

double offsetI;
double filteredI;
double sqI,sumI;
int16_t sampleI;
double Irms;
char ssid[] = "ssid";
char pass[] = "pass";
WiFiClient client;
unsigned long myChannelNumber = 7****; //Your Channel Number (Without Brackets)
 
const char* myWriteAPIKey = "*********"; //Your Write API Key
unsigned long ChannelNumber = ******; //Your Channel Number (Without Brackets)
 
const char* WriteAPIKey = "***********"; //Your Write API Key
double calcIrms(unsigned int Number_of_Samples)
{
   
  /* Be sure to update this value based on the IC and the gain settings! */
  double multiplier =0.05F;    /* ADS1115 @ +/- 4.096V gain (16-bit results) */
  for (unsigned int n = 0; n < Number_of_Samples; n++)
  {
    sampleI = ads.readADC_Differential_0_1();
    //Serial.println(sampleI);
   // delay(100);
    

    // Digital low pass filter extracts the 2.5 V or 1.65 V dc offset, 
  //  then subtract this - signal is now centered on 0 counts.
    offsetI = (offsetI + (sampleI-offsetI)/1024);
    
    filteredI = sampleI - offsetI;
    //filteredI = sampleI * multiplier;

    // Root-mean-square method current
    // 1) square current values
    sqI = filteredI * filteredI;
    // 2) sum 
    sumI += sqI;
  }
  
  
  Irms = sqrt(sumI / Number_of_Samples)*multiplier; 

  //Reset accumulators
  sumI = 0;
//--------------------------------------------------------------------------------------       
 
  return Irms;
}


const int sub=D3;
const int dom=D4 ;
const int street=D5 ;
const int motor1=D6 ;
const int motor2=D7 ;

void setup() {
  
Serial.begin(115200);
  ads.setGain(GAIN_TWO);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  ads.begin();
  
  // Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid,pass);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
   Serial.print("connected: ");
  Serial.println(WiFi.localIP());
 
 
 
ThingSpeak.begin(client);
 
  // Setup GPIO
// relay pin is set as output
  pinMode(sub,INPUT);
  pinMode(dom,INPUT);
  pinMode(street,INPUT);
  pinMode(motor1,INPUT);
  pinMode(motor2,INPUT);
  
 

}   

  double p1=10;  
  double p2=10;
  double p3=10;
  double p4=8;
  double p5=50;
  
// loop() runs continuously, it's our infinite loop.
void loop(){
   
    
  double current = calcIrms(1024);
  delay(500);
   double power =(250*current); // w
   double p=p1-p2 ;
   double ps_dom=p2-p3 ;
   ThingSpeak.setField( 1, (long)p);
   ThingSpeak.setField( 3, (long)ps_dom);
   ThingSpeak.writeFields(ChannelNumber,WriteAPIKey); //Update in ThingSpeaK
   delay(5000);
   
   if(digitalRead(sub) == LOW){
  ThingSpeak.setField( 1, (long)power);
ThingSpeak.setField( 2, (long)p2);
ThingSpeak.setField( 3, (long)p3);
ThingSpeak.setField( 6, (long)p4);
ThingSpeak.setField( 7, (long)p5);
ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
Serial.println("make it any");
delay(5000); // ThingSpeak will only accept updates every 15 seconds.
  
  
  p1=power ;
  
   }
   else if(digitalRead(dom) == LOW){
    ThingSpeak.setField( 1, (long)p1);
ThingSpeak.setField( 2, (long)power);
ThingSpeak.setField( 3, (long)p3);
ThingSpeak.setField( 6, (long)p4);
ThingSpeak.setField( 7, (long)p5);
ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
Serial.println("make it anyD");
delay(5000); // ThingSpeak will only accept updates every 15 seconds.
     p2=power ;
   }
   else if(digitalRead(street) == LOW){
    ThingSpeak.setField( 1, (long)p1);
ThingSpeak.setField( 2, (long)p2);
ThingSpeak.setField( 3, (long)power);
ThingSpeak.setField( 6, (long)p4);
ThingSpeak.setField( 7, (long)p5);
ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
Serial.println("make it anyS");

delay(5000); // ThingSpeak will only accept updates every 15 seconds.
     p3=power ;
   }
  else if(digitalRead(motor1) == LOW){
    ThingSpeak.setField( 1, (long)p1);
ThingSpeak.setField( 2, (long)p2);
ThingSpeak.setField( 3, (long)p3);
ThingSpeak.setField( 6, (long)power);
ThingSpeak.setField( 7, (long)p5);
ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
Serial.println("make it anyM1");

delay(5000); // ThingSpeak will only accept updates every 15 seconds.
     p4=power ;
   }
   else if(digitalRead(motor2) == LOW){
    ThingSpeak.setField( 1, (long)p1);
ThingSpeak.setField( 2, (long)p2);
ThingSpeak.setField( 3, (long)p3);
ThingSpeak.setField( 6, (long)p4);
ThingSpeak.setField( 7, (long)power);
ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
Serial.println("make it anyM2");

delay(20000); // ThingSpeak will only accept updates every 15 seconds.
     p5=power ;
   }
   else{
    Serial.println("no measurement");
   }
  }
