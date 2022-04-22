/*
 * Program that light up 2 LEDs alternatively with the use of 2 DS2406 and 2 PMOS.
 * Each switch is connected to one PMOS and one LED.
 * The LED is ON when the PIOA associated is disabled.
 */

#include <OneWire.h>

OneWire net(5);  // on pin 5
//Theses addresses can be found with the use of the program DS2406_RW
//With one switch at a time
byte addr1[8] = {0x12,0x1a,0x1b,0xd5,0,0,0,0x99}; //Address 1 : 121A1BD500000099
byte addr2[8] = {0x12,0x3e,0x30,0xd5,0,0,0,0x7b}; //Address 2 : 123E30D50000007B
bool numAddr = 0; //0 for address 1; 1 for address 2

void setup(void) {
  Serial.begin(9600);
}

void loop(void) {

  /***********************************************************/
  /* Set Switches States (Enable or disable PIOA1 and PIOA2) */
  /***********************************************************/
  if(numAddr){  //numAddr = 1 -> 2nd switch enabled
    Serial.println("\nPIOA1 disabled and PIO2 enabled");

    /*For device 2*/
    net.reset();
    net.select(addr2);//Select device 2
    net.write(0x55);  //Send the command Write Status
    net.write(0x07);  //Select location 00 07 (2nd byte)
    net.write(0);     //Select location 00 07 (1st byte)
    net.write(0x1F);  //Enable PIOA2
    for(int i = 0; i < 6; i++){ //Read the CRC
      net.read();
    }
    net.write(0xFF, 1);  //Write the Status back
    delay(50);

    /*For device 1*/
    net.reset();
    net.select(addr1);//Select device 1
    net.write(0x55);  //Send the command Write Status
    net.write(0x07);  //Select location 00 07 (2nd byte)
    net.write(0);     //Select location 00 07 (1st byte)
    net.write(0x3E);  //Disable PIOA1
    for(int i = 0; i < 6; i++){ //Read the CRC
      net.read();
    }
    net.write(0xFF, 1);  //Write the Status back
    numAddr = 0;         //Choose to enable PIOA1 and disable PIOA2 for the next loop
  }
  else{         //numAddr = 0 -> 1st switch enabled
    Serial.println("\nPIOA1 enabled and PIO2 disabled");

    /*For device 2*/
    net.reset();
    net.select(addr2);//Select device 2
    net.write(0x55);  //Send the command Write Status
    net.write(0x07);  //Select location 00 07 (2nd byte)
    net.write(0);     //Select location 00 07 (1st byte)
    net.write(0x3E);  //Disable PIOA2
    for(int i = 0; i < 6; i++){ //Read the CRC
      net.read();
    }
    net.write(0xFF, 1);  //Write the Status back
    delay(50);

    /*For device 1*/
    net.reset();
    net.select(addr1);//Select device 1
    net.write(0x55);  //Send the command Write Status
    net.write(0x07);  //Select location 00 07 (2nd byte)
    net.write(0);     //Select location 00 07 (1st byte)
    net.write(0x1F);  //Enable PIOA1
    for(int i = 0; i < 6; i++){ //Read the CRC
      net.read();
    }
    net.write(0xFF, 1);  //Write the Status back
    numAddr = 1;         //Choose to enable PIOA2 and disable PIOA1 for the next loop
  }

  /**********************/
  /* Read Switch Status */
  /**********************/
  net.reset();         //Bus reset
  net.select(addr1);   //Select the device 1
  net.write(0xAA, 1);  //Send the command Read Status
  net.write(0, 1);
  net.write(0, 1);
  Serial.print("Read Status 1 : ");
  for(int i = 0; i < 10; i++){
    Serial.print(net.read(), HEX);  //Read and show data
    Serial.print(" ");
  }
  Serial.println();
  net.reset();
  net.select(addr2);   //Select the device 2
  net.write(0xAA, 1);  //Send the command Read Status
  net.write(0, 1);
  net.write(0, 1);
  Serial.print("Read Status 2 : ");
  for(int i = 0; i < 10; i++){
    Serial.print(net.read(), HEX);  //Read and show data
    Serial.print(" ");
  }
  Serial.println();
  net.reset();

  delay(200);
}
