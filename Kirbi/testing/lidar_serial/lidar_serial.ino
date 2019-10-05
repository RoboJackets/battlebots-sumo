// set this to the hardware serial port you wish to use
#define HWSERIAL Serial1
byte byteArray [9];
// Byte0 Byte 1 Byte2 Byte3  Byte4      Byte5      Byte6  Byte7  Byte8
// 0x89  0x89 Dist_L  Dist_H Strength_L Strength_H Temp_L Temp_H Checksum
// byte2 is distance, overflows into byte3

byte configOutput [5] = {0x5A, 0x05, 0x07, 0x01, 0x11}; // write this array to HWSERIAL to enable output
byte configUART [5] = {0x5A, 0x05, 0x0A, 0x00, 0x11}; // write this array to HWSERIAL to set sensor to UART mode
// more config commands on datasheet https://acroname.com/sites/default/files/assets/sj-gu-tfmini_plus-01-a04-datasheet_en.pdf


void setup() {
  Serial.begin(115200); 
  HWSERIAL.begin(115200);// default baud rate
  HWSERIAL.write(configUART, 5); // set sensor to UART mode
  HWSERIAL.write(configOutput, 5); // enable output
}

void loop() {
  if (HWSERIAL.available() > 0) {
    HWSERIAL.readBytes(byteArray, 9); // write output of read to an array of length 9
    for (int i =0;i<9;i++){
      Serial.println(byteArray[i]);  
    }  
  }
}
