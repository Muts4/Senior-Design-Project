// ESPNOW Recieving code with SD data code. Unorganized saved data.
// Include required libraries
#include <WiFi.h>
#include <esp_now.h>
#include "FS.h"
#include "SD.h"
#include <SPI.h>

//OLED Libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//OLED -- this is the Width and Height of Display which is 128 X 32 pixels
#define OLED_WIDTH  128
#define OLED_HEIGHT 32
//OLED -- More OLED stuff
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

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

 display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize OLED with the I2C addr 0x3C (for the 128x32)
 display.clearDisplay(); // cleans the screen
 display.drawRect(0, 0, 127,32, WHITE); //draws a margin around the screen
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.setCursor(22,10);
 display.println("Welcome");

 display.display();
 delay(5000);
 
 display.clearDisplay();
 display.drawRect(0, 0, 127,32, WHITE); //draws a margin around the screen
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.setCursor(40,3); //x,y position of the text
 display.println("Team 41");
 display.setCursor(10,12); //x,y position of the text
 display.println("Grip Force Monitor");
 display.setCursor(40,21); //x,y position of the text
 display.println("System");

 display.display();
 delay(5000);
 
 //Display main menu
 display.clearDisplay();
 display.drawRect(100, 2, 24, 10, WHITE); //Draw battery icon on the top right corner
 display.fillRect(102, 5, 6, 4, WHITE); //Full battery or less
 display.fillRect(109, 5, 6, 4, WHITE); //66.6 % of battery or less
 display.fillRect(116, 5, 6, 4, WHITE); //33.3 % of battery or less
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.setCursor(10,10);
 display.println("Recording data");
 display.setCursor(10,18);
 display.println("Starts in...");
 
 display.drawRect(0, 0, 127,32, WHITE);
  
 display.display();
 delay(1000);

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

 //OLED display code
 //define string variables for printing on OLED
 String forceData1 =  String(myData.a, 2);
 String forceData2 =  String(myData.b, 2);
 String forceData3 =  String(myData.c, 2);
 display.clearDisplay(); 
 
 // Prints the sensor's data on the OLED
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.setCursor(3,3); //x,y position of the text
 display.println("Palm Sensor 1:");
 display.setCursor(90,3); //x,y position of the incoming data
 display.println(forceData1);
 
 display.setCursor(3,12); //x,y position of the text
 display.println("Palm Sensor 2:");
 display.setCursor(90,12); //x,y position of the incoming data
 display.println(forceData2);
 
 display.setCursor(3,21); //x,y position of the text
 display.println("Finger Sensor:");
 display.setCursor(90,21); //x,y position of the incoming data
 display.println(forceData3);
 display.drawRect(0, 0, 127,32, WHITE); //draws a margin around the screen

 display.display();

//SD stuff starts
  
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
