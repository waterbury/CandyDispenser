#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>

int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by


int dispenseTime = 700;
unsigned long previousTime = millis();
int candyDispensed = 0;

int motor = 14;
int handSensor = 5;
int NFC_IRQ = 3;
int tableDetect = 8;
int rLED = 9;
int gLED = 10;
int bLED = 11;

int buffer = 0;
int candy = 0;
int i = 0;
int handDetected = 0;

  //Authorized UIDs
  uint8_t auth_uids[][7] = {
    {0xFE, 0xE2, 0x70, 0x5A, 0x00, 0x00, 0x00}, // David Hand
    {0xCE, 0xE5, 0x70, 0x5A, 0x00, 0x00, 0x00}, // Lance Hand
    {0x43, 0x7C, 0x15, 0x34, 0x00, 0x00, 0x00}, // Lance Phone
    {0x2E, 0x1D, 0x73, 0xBE, 0x00, 0x00, 0x00}, // iShortBus Phone Sticker
    {0x04, 0xC4, 0x5C, 0x1A, 0xD9, 0x26, 0x80}, // bentruck Phone Sticker
    {0xEE, 0xCF, 0x17, 0xDB, 0x00, 0x00, 0x00}, // Door Open Chip
    {0xCE, 0x2D, 0x74, 0xBE, 0x00, 0x00, 0x00}, //Conner Brooks Sticker
    {0x1E, 0x83, 0x74, 0xBE, 0x00, 0x00, 0x00} //Tony Door Sticker
};
Adafruit_NFCShield_I2C nfc(NFC_IRQ, 7);

void setup() {
  // put your setup code here, to run once:
  

  
pinMode(NFC_IRQ,INPUT);

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



  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0x01);
  
  // configure board to read RFID tags
  nfc.SAMConfig();
    
  Serial.println("Waiting for an ISO14443A card");

}

void loop() {
  // put your main code here, to run repeatedly: 

/*while (digitalRead(tableDetect) ==   HIGH)
{
digitalWrite(bLED, 0);    
digitalWrite(gLED, 0);    

digitalWrite(rLED, 1);    
delay(250);
analogWrite(rLED, 0);
delay(250);  
  
} */ 

  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;

  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength))
  {

    Serial.print("Entry Request for UID: ");
    nfc.PrintHex(uid, uidLength);

    uint32_t i;
    uint32_t b;
    int num_uids = sizeof(auth_uids)/sizeof(auth_uids[0]);
    Serial.println(num_uids);
    for (i=0; i < num_uids; i++){
        Serial.print("Comparing UID to: ");
        nfc.PrintHex(auth_uids[i], 7);
        for (b=0; b < 7; b++)
        {
            if (uid[b] == auth_uids[i][b])
            {
                if (b == 6)
                {
                   // digitalWrite(13, HIGH);
                   dispenseCandy(5);
                   
                    Serial.println("Unlocked!");
                    delay(10000);
                   // digitalWrite(13, LOW);
                    Serial.println("Locked!");
                    i=num_uids;
                }
            } 
            else 
            {
                Serial.println("Failed!");
                //b=7;
            }
        }
    }

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
     Serial.println("Waiting for hand");     
     
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
       Serial.println("Fin");
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


