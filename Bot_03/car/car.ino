/**
 * An Mirf example which copies back the data it recives.
 *
 * Pins:
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 *
 * Configurable:
 * CE -> 8
 * CSN -> 7
 *
 */
struct PultCommand{
  char LM;
  byte LZ;
  char RM;
  byte RZ;
};

struct PultCommand UpData;

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

void setup(){
  Serial.begin(9600);
  
  /*
   * Set the SPI Driver.
   */
  Mirf.cePin = 9;
  Mirf.csnPin = 10;

  Mirf.spi = &MirfHardwareSpi;
  
  /*
   * Setup pins / SPI.
   */
   
  Mirf.init();
  
  /*
   * Configure reciving address.
   */
   
  Mirf.setRADDR((byte *)"serv1");
  
  /*
   * Set the payload length to sizeof(unsigned long) the
   * return type of millis().
   *
   * NB: payload on client and server must be the same.
   */
   
  //Mirf.payload = sizeof(unsigned long);
  Mirf.payload = sizeof(UpData);
  
  /*
   * Write channel and payload config then power up reciver.
   */
  Mirf.channel = 5;
  
  Mirf.config();
  
  Serial.println("Listening..."); 
}

void loop(){
  /*
   * A buffer to store the data.
   */

  //byte data[Mirf.payload];
  
  /*
   * If a packet has been recived.
   *
   * isSending also restores listening mode when it 
   * transitions from true to false.
   */
   
  if(!Mirf.isSending() && Mirf.dataReady()){
    Serial.println("Got packet");
    
    /*
     * Get load the packet into the buffer.
     */
     
    //Mirf.getData(data);
    Mirf.getData((byte *) &UpData);
    /*
    UpData.LM = data[0];
    UpData.LZ = data[1];
    UpData.RM = data[2];
    UpData.RZ = data[3];
    */
    /*
     * Set the send address.
     */
    
    Mirf.setTADDR((byte *)"clie1");
    
    /*
     * Send the data back to the client.
     */
    Serial.println(UpData.LM);
    Serial.println(UpData.LZ, DEC);
    Serial.println(UpData.RM);
    Serial.println(UpData.RZ, DEC);
    //Serial.println("************");
     
    Mirf.send((byte *)&UpData);
    
    /*
     * Wait untill sending has finished
     *
     * NB: isSending returns the chip to receving after returning true.
     */
      
    Serial.println("Reply sent.");
  }
}
