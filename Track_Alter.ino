#include <AltSoftSerial.h> 
#include <TinyGPS++.h> 
#include <GSM.h>
#define PINNUMBER "1234" // PIN Number for the SIM - leave blank unless your SIM has a pin, this is inserted between ""


static const uint32_t GPSBaud = 9600; //Baud rate for communication with the GPS, Adafruit GPS = 9600, your GPS may well be 4800, check the spec

TinyGPSPlus gps; // The TinyGPS++ object for interfacing with the GPS

AltSoftSerial ss; // The serial connection object to the GPS device

String yourPassword = "alpacho";
String password;

GSM gsmAccess; 
GSM_SMS sms;

char senderNumber[20]; // Array to hold the number a SMS is retreived from

void setup() 
{
  ss.begin(GPSBaud); // begin the GPS serial connection
  
  Serial.begin(9600); 

  Serial.println("Where's my wheels?"); 
  Serial.println("Initialising..."); 
  
  boolean notConnected = true; // connection state

  while(notConnected) 
  {
    
    if(gsmAccess.begin(PINNUMBER)==GSM_READY)// if it succeeds connecting
       notConnected = false; // connected
     
    else
    {
      Serial.println("Not connected"); 
      delay(1000); 
    }
  }

  Serial.println("GSM initialized");   
  Serial.println("Waiting for messages");
}
  

void loop() 
{
  Serial.println("chec");
  while (sms.available() > 0) //while there is stuff in the buffer
  Serial.println("check");
    if (gps.encode(sms.read())) //if it can successfully decode it, do it. Else try again when more charachters are in the buffer
 
  if (sms.available()) // if a text has been recieved
  {
    Serial.println("Message received from:"); 

    sms.remoteNumber(+917740939329, 20); // assign the sender number to the "senderNumber" variable 
    Serial.println(senderNumber);
    
    password = ""; 

    char c;
    
    while(c=sms.read())
    {
      password += c; // append the sms to the "password" variable
    }

    Serial.println(password); 
    Serial.println("\nEND OF MESSAGE"); 

    sms.flush(); // delete message from modem buffer
    Serial.println("MESSAGE DELETED");

    //if (password == yourPassword) // if the sms contains the correct password
    {
      Serial.println("\nPASSWORD VALID");
      sms.beginSMS(senderNumber); // begin an sms to the sender number
      sms.print(gps.location.lat(), 6); // append the lat to the sms
      sms.print(","); // append a comma
      sms.print(gps.location.lng(), 6); // append the lon to the sms
      sms.endSMS(); //send the sms
    }
//    else Serial.println("\nPASSWORD NOT VALID"); // print to the computer
  }

  delay(1000);
}
//-----+
