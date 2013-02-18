int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by


int dispenseTime = 700;
unsigned long previousTime = millis();
int candyDispensed = 0;

int motor = 14;
int handSensor = 5;
int tableDetect = 3;
int rLED = 9;
int gLED = 10;
int bLED = 11;

int buffer = 0;
int candy = 0;
int i = 0;
int handDetected = 0;


void setup() {
  // put your setup code here, to run once:
pinMode(motor,OUTPUT);
pinMode(handSensor,INPUT);
pinMode(tableDetect,INPUT);
digitalWrite(tableDetect, HIGH); //Holds line high

pinMode(rLED,OUTPUT);
pinMode(gLED,OUTPUT);
pinMode(bLED,OUTPUT);

//digitalWrite(gLED,HIGH);

Serial.begin(9600);

attachInterrupt(0, handDetect_func, RISING);
}

void loop() {
  // put your main code here, to run repeatedly: 

while (digitalRead(tableDetect) ==   LOW)
{
digitalWrite(bLED, 0);    
digitalWrite(gLED, 0);    

digitalWrite(rLED, 1);    
delay(250);
analogWrite(rLED, 0);
delay(250);  
  
}  
 
  
  
if (handDetected == 1)  
{
 Serial.println("YOLO");
 handDetected = 0;
 
analogWrite(bLED, 0);    
analogWrite(gLED, 0);    


digitalWrite(rLED, HIGH);    
delay(150);
digitalWrite(rLED, LOW);
delay(150);
digitalWrite(rLED, HIGH);
delay(150);
digitalWrite(rLED, LOW);
delay(150);
digitalWrite(rLED, HIGH);
delay(150);
digitalWrite(rLED, LOW);
delay(150);
digitalWrite(rLED, HIGH);
delay(150);
digitalWrite(rLED, LOW);
delay(150);
 
 
} 


analogWrite(rLED, brightness);    
analogWrite(gLED, brightness);   
analogWrite(bLED, brightness);   
brightness = brightness + fadeAmount;

if (brightness == 0 || brightness == 255) {
 fadeAmount = -fadeAmount ; 
  }     
delay(30);        

    
  
  
  if (Serial.available() > 0)
  {
    
   buffer = Serial.read();

   if (buffer == 'G') 
     i = 1;
     
   else if (buffer == 'U') {
     if (i == 1)  i =2;
     else  i = 0; }
     
   else if (buffer == 'M') {
     if (i == 2) i =3;
      else i = 0;  }
      
   else if (i == 3)
    {      
     if (buffer >= 48 && buffer <= 57)     
      candy = buffer - 48;
     else 
      candy = 0;
     i = 4;
    }
 
   else if (buffer == '\n') {
     
     if (i==4 && candy <=5)  
     {        
     digitalWrite(bLED, 0);        
     digitalWrite(rLED, 0);    
     
     previousTime = millis();
     candyDispensed = 0;
     Serial.println('Waiting for hand');     
     
     while( ((millis() - previousTime) < 10000) && candyDispensed == 0)
     {
      analogWrite(gLED, brightness);   
      brightness = brightness + fadeAmount;
      if (brightness == 0 || brightness == 255) {
       fadeAmount = -fadeAmount ; }
      delay(30);  
     
     if (handDetected == 1)  
      {
      if ( dispenseCandy(candy) )
       Serial.println('Fin');
      handDetected = 0; 
      candyDispensed = 1;
      }
        
     }
        
     }
  else
   Serial.println("Out of bounds");
        
     i = 0; 
    }
    

    
  }
  
}

int dispenseCandy(int howMuchCandy)
{
int calculatedDispenseTime = howMuchCandy * dispenseTime;  
unsigned long startTime = millis();
  
 if (howMuchCandy > 0 && howMuchCandy <=5)
 {   
    digitalWrite(motor,HIGH);
    digitalWrite(rLED,LOW);
    digitalWrite(gLED,LOW);
    
    
    while ( (millis() - startTime) < calculatedDispenseTime )
    {
     digitalWrite(bLED,HIGH);
     delay(150);
     digitalWrite(bLED,LOW);
     delay(150);
    }  
    
    
    digitalWrite(motor,LOW); 
    return 1;
 }
 else
  return 0;
}

void handDetect_func()
{
handDetected = 1;  
  
}


