int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by


int dispenseTime = 700;

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
  
if (handDetected == 1)  
{
 Serial.println("YOLO");
 handDetected = 0;
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
     if (i==4)  
      if ( dispenseCandy(candy) )
       Serial.println('Fin');
      else
        Serial.println('OOB');
     i = 0; 
    }
    

    
  }
  
}

int dispenseCandy(int howMuchCandy)
{
 if (howMuchCandy > 0 && howMuchCandy <=5)
 {   
    digitalWrite(motor,HIGH);
    delay(howMuchCandy * dispenseTime);
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


