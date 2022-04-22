// ESPNOW Recieving code with SD data code. Unorganized saved data.
// Include required libraries
#include <WiFi.h>
#include <esp_now.h>
#include "FS.h"
#include "SD.h"
#include <SPI.h>

 String dataMessage_1;
 String dataMessage_2;
 String dataMessage_3;
// Define data structure
typedef struct struct_message {
    float a;
    float b;
    float c;

} struct_message;

// Create structured data object
struct_message myData;

// Callback function
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) 
{
  // Get incoming data
  memcpy(&myData, incomingData, sizeof(myData));

  // Print to Serial Monitor
  Serial.print("Palm Sensor 1: ");
  Serial.println(myData.a);

  Serial.print("Palm Sensor 2: ");
  Serial.println(myData.b);

  Serial.print("Finger Sensor 1: ");
  Serial.println(myData.c);

}

// Initialize SD card
void initSDCard(){
   if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }
  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

// Write to the SD card
void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

// Append data to the SD card
void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);
  initSDCard();

   // If the data.txt file doesn't exist
  // Create a file on the SD card and write the data labels
  File file = SD.open("/data.txt");
  if(!file) {
    Serial.println("File doesn't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/data.txt", "Palm sensor 1");
  }
  else {
    Serial.println("File already exists");  
  }
  file.close();

  // Start ESP32 in Station mode
  WiFi.mode(WIFI_STA);

  // Initalize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }


  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
   dataMessage_1 = float(myData.a);
    Serial.print("Sensor 1 data: ");
    Serial.println(dataMessage_1);

     dataMessage_2 = float(myData.b);
    Serial.print("Sensor 2 data: ");
    Serial.println(dataMessage_2);

     dataMessage_3 = float(myData.c);
    Serial.print("Sensor 3 data: ");
    Serial.println(dataMessage_3);

    //Append the data to file
    appendFile(SD, "/data.txt", dataMessage_1.c_str());
     appendFile(SD, "/data.txt", dataMessage_2.c_str());
      appendFile(SD, "/data.txt", dataMessage_3.c_str());
    //delay(3000);
  }
