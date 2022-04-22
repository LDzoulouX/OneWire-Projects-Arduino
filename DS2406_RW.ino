#include <OneWire.h>

/*
 * This program search for the switch (DS2406)
 * The program will control just one switch
 * Then it Enables or Disables the PIOA (the output) of the switch alternatively for each loop
 */

OneWire net(5);  // on pin 5
bool pioA1 = true; //True = PIOA enabled; False = PIOA disabled

/***************************************************/
/*Function that can print the address of the switch*/
/***************************************************/
void PrintBytes(const uint8_t* addr, uint8_t count, bool newline=false) {
  for (uint8_t i = 0; i < count; i++) {
    Serial.print(addr[i]>>4, HEX);
    Serial.print(addr[i]&0x0f, HEX);
  }
  if (newline)
    Serial.println();
}

void setup(void) {
  Serial.begin(9600);
}

void loop(void) {
  byte addr[8];

  /**************************************************/
  /*Find the address of the switch and check the CRC*/
  /**************************************************/
  if (!net.search(addr)) {  //If there is no more switch
    Serial.print("No more addresses.\n");
    Serial.print("\n");
    net.reset_search(); //Reset the search to begin a new search in the next loop
    delay(3000);
    return;
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {  //Check the CRC
    Serial.print("CRC is not valid!\n");
    return;
  }

  if (addr[0] != 0x12) {  //Check the family of the switch
    PrintBytes(addr, 8);
    Serial.print(" is not a DS2406.\n");
    return;
  }

  Serial.print("  Reading DS2406 ");
  PrintBytes(addr, 8);  //Print the address of the switch
  Serial.println();
  
  /********************************************/
  /*Set Switch State (Enable or disable PIO-A)*/
  /********************************************/
  pioA1 = !pioA1;
  net.reset();      //Bus reset
  net.select(addr); //Choose the DS2406
  net.write(0x55);  //Send the command Write Status
  net.write(0x07);  //Select location 00 07 (2nd byte)
  net.write(0);     //Select location 00 07 (1st byte)
  if(pioA1){ //PIOA1 = 1
    Serial.println("PIO-A enabled");
    net.write(0x1F);  //Command to enable the PIOA
  }
  else{     //PIOA1 = 0
    Serial.println("PIO-A disabled");
    net.write(0x3F);  //Command to disable the PIOA
  }
  for(int i = 0; i < 6; i++){ //Read the CRC
    net.read();
  }
  net.write(0xFF, 1);  //Write the Status back

  /********************/
  /*Read Switch Status*/
  /********************/
  net.reset();         //Bus reset
  net.select(addr);    //Choose the DS2406
  net.write(0xAA, 1);  //Send the command Read Status
  net.write(0, 1);
  net.write(0, 1);
  Serial.print("Read Status : ");
  for(int i = 0; i < 10; i++){
    Serial.print(net.read(), HEX);  //Read and show data
    Serial.print(" ");
  }
  Serial.println();
  net.reset();
}
