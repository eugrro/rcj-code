/* The range readings are in units of mm. */
#define GPIO_PIN1 4  //laser number: 1 pin #3
#define GPIO_PIN2 5  //laser number: 3 
#define GPIO_PINL1 6 //laser number: 2
#define GPIO_PINL2 7 //laser number: 4*/
#define SERVO 9

#include <Wire.h>
#include <Servo.h>
#include <VL6180X.h> //tof short range polulu library
#include <VL53L0X.h> //tof long range polulu library

VL6180X laser1; //init laser var1 SHORT
VL6180X laser2; //init laser var2 SHORT
VL53L0X laser3; //init laser var3 LONG
VL53L0X laser4; //init laser var4 LONG

Servo servo;  
 
int angle = 0;   // servo position in degrees 

unsigned long curr_time;

void setup()
{

  /*RESET PINS AND WRITE*/
  pinMode(LED_PIN, OUTPUT);
  pinMode(L_ENCODER_A, INPUT); //set encoderA as INPUT
  pinMode(L_ENCODER_B, INPUT); //set encoderB as INPUT
  pinMode(R_ENCODER_A, INPUT); //set encoderA as INPUT
  pinMode(R_ENCODER_B, INPUT); //set encoderB as INPUT
  pinMode(GPIO_PIN1, INPUT); //set encoderA as INPUT
  pinMode(GPIO_PIN2, INPUT); //set encoderB as INPUT
  pinMode(GPIO_PINL1, INPUT); //set encoderA as INPUT
  pinMode(GPIO_PINL2, INPUT); //set encoderB as INPUT
  delay(100);

  digitalWrite(LED_PIN, LOW); //turn off LED
  digitalWrite(GPIO_PIN1, LOW); //reset XSHUT of first short laser
  digitalWrite(GPIO_PINL1, LOW); //reset XSHUT of first long laser
  digitalWrite(GPIO_PIN2, LOW); //reset XSHUT of second short laser
  digitalWrite(GPIO_PINL2, LOW); //reset XSHUT of second long laser
  delay(100);

  // initialize hardware interrupts
  attachInterrupt(digitalPinToInterrupt(2), leftEncoderEvent, CHANGE);    //Pins 2 and 3 are the only Interrupt Pins 2-3
  attachInterrupt(digitalPinToInterrupt(3), rightEncoderEvent, CHANGE);
  delay(100);

  digitalWrite(GPIO_PIN2, HIGH); //begin writing to XSHUT of first laser
  delay(50); //delay
  laser2.init(); //init laser object, look for it
  laser2.configureDefault(); //laser config
  laser2.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 35);
  laser2.setTimeout(500); //in case you can't find the laser object, timeout for this long
  //laser2.stopContinuous();
  delay(300);
  laser2.startRangeContinuous(30);
  laser2.setAddress(0x26);
  delay(100); //delay

  digitalWrite(GPIO_PINL1, HIGH); //begin writing to XSHUT of first laser
  delay(50); //delay
  laser3.init(); //init laser object, look for it
  laser3.setTimeout(500); //in case you can't find the laser object, timeout for this long
  laser3.stopContinuous();
  delay(300);
  laser3.startContinuous(30);
  laser3.setAddress(0x27);
  delay(100); //delay

  digitalWrite(GPIO_PINL2, HIGH); //begin writing to XSHUT of first laser
  delay(50); //delay
  laser4.init(); //init laser object, look for it
  laser4.setTimeout(500); //in case you can't find the laser object, timeout for this long
  laser4.stopContinuous();
  delay(300);
  laser4.startContinuous(30);
  laser4.setAddress(0x28);
  delay(100); //delay

  digitalWrite(GPIO_PIN1, HIGH); //begin writing to XSHUT of first laser
  delay(50); //delay
  laser1.init(); //init laser object, look for it
  laser1.configureDefault(); //laser config
  laser1.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 35);
  laser1.setTimeout(500); //in case you can't find the laser object, timeout for this long
  laser1.setAddress(0x25);
  laser1.stopContinuous();
  delay(300);
  laser1.startRangeContinuous(30);
  delay(100); //delay
}

void loop()
{
  // scan from 0 to 180 degrees
  for(angle = 0; angle < 180; angle++)  
  {         
    servo.write(angle);     
    if(angle % 3 == 0)
    {       
      Serial.print(millis());    
      Serial.print(" l "); Serial.print(angle); 
      Serial.print(" "); Serial.print(laser1.readRangeContinuousMillimeters()); //label with l1 and get the laser reading (mm)

      Serial.print(" ");  Serial.print(laser2.readRangeContinuousMillimeters()); //label with l2 and get the laser reading (mm)

      Serial.print(" ");  Serial.print(laser3.readRangeContinuousMillimeters()); //label with l3 and get the laser reading (mm)

      Serial.print(" ");  Serial.println(laser4.readRangeContinuousMillimeters()); //label with l4 and get the laser reading (mm)                 
    }          
    delay(8);                   
  } 

  delay(500);
  // now scan back from 180 to 0 degrees
  for(angle = 180; angle > 0; angle--)    
  {                                
    servo.write(angle);  
    if(angle % 3 == 0)
    { 
      Serial.print(millis());    
      Serial.print(" l "); Serial.print(angle); 
      Serial.print(" "); Serial.print(laser1.readRangeContinuousMillimeters()); //label with l1 and get the laser reading (mm)

      Serial.print(" ");  Serial.print(laser2.readRangeContinuousMillimeters()); //label with l2 and get the laser reading (mm)

      Serial.print(" ");  Serial.print(laser3.readRangeContinuousMillimeters()); //label with l3 and get the laser reading (mm)

      Serial.print(" ");  Serial.println(laser4.readRangeContinuousMillimeters()); //label with l4 and get the laser reading (mm)   
                   
    }               
    delay(8);       
  } 
  delay(500);

}
