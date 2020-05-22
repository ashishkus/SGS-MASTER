#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1015 ads;     /* Use thi for the 12-bit version */

double offsetI;
double filteredI;
double sqI,sumI;
int16_t sampleI;
double Irms;

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


const int more_posi=8;
const int more_nega=9 ;
const int shut=10 ;


void setup() {
  
Serial.begin(9600);
  ads.setGain(GAIN_TWO);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  ads.begin();
  
 
  // Setup GPIO
// relay pin is set as output
  pinMode(more_posi,OUTPUT);
  pinMode(more_nega,OUTPUT);
  pinMode(shut,OUTPUT);
   digitalWrite(more_posi,LOW);
     digitalWrite(more_nega,LOW);
     digitalWrite(shut,LOW); 
  
 

}   

// loop() runs continuously, it's our infinite loop.
void loop(){
   
    
  double current = calcIrms(1024);
  delay(500);
   double power =(250*current); // w
  Serial.println(power);
  if(power > 50.00)
  {
    digitalWrite(more_posi,HIGH);
     digitalWrite(more_nega,HIGH);
     digitalWrite(shut,LOW); 
  }
  else if(power < 1.00)
  {
   digitalWrite(shut,HIGH); 
   digitalWrite(more_posi,LOW);
     digitalWrite(more_nega,LOW);
  }
  else{
    digitalWrite(shut,LOW); 
   digitalWrite(more_posi,LOW);
     digitalWrite(more_nega,LOW);
  }
}
