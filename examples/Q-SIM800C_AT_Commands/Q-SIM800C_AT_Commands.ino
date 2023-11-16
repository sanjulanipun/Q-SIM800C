/*
   SIM800C  |  Arduino  |  Power Supply(5-20V 1A)
   _______________________________________________
    KEY          8              *
    TX           7              *
    RX           6              *
    DTR          5              *
    RIN          9              *
    GND         GND             -
    VCC          *              +
    _______________________________________________
*/

#include <SoftwareSerial.h>

#define MODEM_BAUD          9600
#define MODEM_PWRKEY        8
#define MODEM_TX            7
#define MODEM_RX            6
#define MODEM_DTR           5
#define MODEM_RI            9

//Create software serial object to communicate with SIM800C
SoftwareSerial QSIM800C(MODEM_TX, MODEM_RX);


bool reply = false;

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(115200);
  Serial.println();

  Serial.println("System Starting...");
  delay(100);

  //
  pinMode(MODEM_PWRKEY, OUTPUT);
  pinMode(MODEM_RI, INPUT);

  //Begin serial communication with Arduino and SIM800C
  QSIM800C.begin(MODEM_BAUD);

  //  QSIM800C.println("AT"); //Once the handshake test is successful, it will back to OK
  //  QSIM800C.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  //  QSIM800C.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  //  QSIM800C.println("AT+CREG?"); //Check whether it has registered in the network
  //  QSIM800C.println("AT+CMGF=1"); // Configuring TEXT mode
  //  QSIM800C.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled


  Modem_POWER_ON();
  if (reply) {
    Serial.println(F("***********************************************************"));
    Serial.println(F(" You can now send AT commands"));
    Serial.println(F(" Enter \"AT\" (without quotes), and you should see \"OK\""));
    Serial.println(F(" If it doesn't work, select \"Both NL & CR\" in Serial Monitor"));
    Serial.println(F(" DISCLAIMER: Entering AT commands without knowing what they do"));
    Serial.println(F(" can have undesired consiquinces..."));
    Serial.println(F("***********************************************************\n"));

    // Uncomment to read received SMS
    //SerialAT.println("AT+CMGL=\"ALL\"");
  } else {
    Serial.println(F("*************************************************************************************"));
    Serial.println(F(" Failed to connect to the Modem! Check the Baud OR Modem Power Supply and try Again! "));
    Serial.println(F("*************************************************************************************\n"));
  }
}

void loop() {

  if (QSIM800C.available()) {
    Serial.write(QSIM800C.read());
  }
  if (Serial.available()) {
    QSIM800C.write(Serial.read());
  }
  delay(1);

}

void Modem_POWER_ON() {

  int i = 40;

  Serial.println("# Initializing Modem #");
  Serial.println("# Sending \"AT\" to Modem. Waiting for Response # ");

  while (i) {

    QSIM800C.println(F("AT"));

    // Show the User: we are doing something.
    Serial.print(F("."));
    delay(500);

    // Did the Modem send something?
    if (QSIM800C.available()) {

      String r = QSIM800C.readString();
      Serial.print("\r\n# Response:\r\n" + r);

      if (r.indexOf("OK") >= 0) {
        reply = true;
        break;;
      } else {
        Serial.print(F("\r\n# "));
      }
    }

    // On the 5th try: Inform the User what is happening
    if (i == 35) {
      Serial.println();
      Serial.println();
      Serial.println("# Modem is not yet online / Modem did not yet answer. #");
      Serial.println("# Probably Power loss? OR Modem is turn OFF ?");
      Serial.println("# Tring to turn ON Modem... Wait for 2.5 Seconds to turn ON Modem! #");
      digitalWrite(MODEM_PWRKEY, HIGH);
      delay(2500);
      digitalWrite(MODEM_PWRKEY, LOW);
      delay(1000);
      Serial.println();
      Serial.println("# Tring ON Pulse Send Done! Now Sending \"AT\" to Modem. Waiting for Response! #");
    }

    delay(500);

    i--;

  }

  Serial.println(F("#\r\n"));

}
