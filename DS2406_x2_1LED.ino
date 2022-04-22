/*
 * Program that light up 1 LED alternatively with the use of 2 DS2406 and 1 PMOS.
 * One DS2406 is used to control the LED, and the second one is a backup.
 * If the first switch is disconnected, then the second does the work.
 * The first switch does the work again if reconnected.
 * The LEDs is ON when the PIOA associated is disabled.
 */

#include <OneWire.h>

OneWire net(5);  // on pin 5
//Theses addresses can be found with the use of the program DS2406_RW
//With one switch at a time
byte addr1[8] = {0x12,0x1a,0x1b,0xd5,0,0,0,0x99}; //Address 1 : 121A1BD500000099
byte addr2[8] = {0x12,0x3e,0x30,0xd5,0,0,0,0x7b}; //Address 2 : 123E30D50000007B
bool ena = 0; //0 enables the PIOA and set the LED OFF, 1 disables the PIOA and set the LED ON

void setup(void) {
  Serial.begin(9600);
}

void loop(void) {
  byte addr[8];

  /*******************************************************/
  /* Check if device 1 is ok and use it, or use device 2 */
  /*******************************************************/
  if(net.search(addr)){      //Scan all addresses to find the good one
    for(int i = 1; i < 8; i++){ //Compare each byte of the address to the selected one
      if(addr[i] != addr1[i]){  //If the byte don't correspond -> switch 1 disconnected
        if(ena){
          Serial.println("Enable PIOA2\n");
          net.reset();
          net.select(addr2);//Select device 2
          net.write(0x55);  //Send the command Write Status
          net.write(0x07);  //Select location 00 07 (2nd byte)
          net.write(0);     //Select location 00 07 (1st byte)
          net.write(0x1F);  //Enable PIOA
          for(int i = 0; i < 6; i++){ //Read the CRC
            net.read();
          }
          net.write(0xFF, 1);  //Write the Status back
          ena = 0;
        }else{
          Serial.println("Disable PIOA2\n");
          net.reset();
          net.select(addr2);//Select device 1
          net.write(0x55);  //Send the command Write Status
          net.write(0x07);  //Select location 00 07 (2nd byte)
          net.write(0);     //Select location 00 07 (1st byte)
          net.write(0x3E);  //Disable PIOA
          for(int i = 0; i < 6; i++){ //Read the CRC
            net.read();
          }
          net.write(0xFF, 1);  //Write the Status back
          ena = 1;
        }
        delay(1000);
        return;
      }
    }
    //When here, device 1 is here and active
    //First, disable device 2
    net.reset();
    net.select(addr2);//Select device 1
    net.write(0x55);  //Send the command Write Status
    net.write(0x07);  //Select location 00 07 (2nd byte)
    net.write(0);     //Select location 00 07 (1st byte)
    net.write(0x3E);  //Disable PIOA
    for(int i = 0; i < 6; i++){ //Read the CRC
      net.read();
    }
    net.write(0xFF, 1);  //Write the Status back
    if(ena){
      Serial.println("Enable PIOA1\n");
      net.reset();
      net.select(addr1);//Select device 1
      net.write(0x55);  //Send the command Write Status
      net.write(0x07);  //Select location 00 07 (2nd byte)
      net.write(0);     //Select location 00 07 (1st byte)
      net.write(0x1F);  //Enable PIOA
      for(int i = 0; i < 6; i++){ //Read the CRC
        net.read();
      }
      net.write(0xFF, 1);  //Write the Status back
      ena = 0;
    }else{
      Serial.println("Disable PIOA1\n");
      net.reset();
      net.select(addr1);//Select device 1
      net.write(0x55);  //Send the command Write Status
      net.write(0x07);  //Select location 00 07 (2nd byte)
      net.write(0);     //Select location 00 07 (1st byte)
      net.write(0x3E);  //Disable PIOA
      for(int i = 0; i < 6; i++){ //Read the CRC
        net.read();
      }
      net.write(0xFF, 1);  //Write the Status back
      ena = 1;
    }
    delay(1000);
  }
  net.reset_search(); //Reset the search to begin a new search in the next loop

  delay(200);
}
