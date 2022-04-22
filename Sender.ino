// Include required libraries
#include <WiFi.h>
#include <esp_now.h>


// Define FSR parameters, *ADC2 Pins wont work* 
#define FSRPin_1 36 //VP
#define FSRPin_2 39 //VN
#define FSRPin_3 34 

// Variables for Force
  float cf = 9.75;
  float force_1;
  float vout_1;
  float force_2;
  float vout_2;
  float force_3;
  float vout_3;

// Responder MAC Address (Replace with your responders MAC Address)
uint8_t broadcastAddress[] = {0xAC, 0x0B, 0xFB, 0x6F, 0x70, 0x14};

// Define data structure
typedef struct struct_message {
  float a;
  float b;
  float c;
} struct_message;

// Create structured data object
struct_message myData;

// Register peer
esp_now_peer_info_t peerInfo;

// Sent data callback function
void OnDataSent(const uint8_t *macAddr, esp_now_send_status_t status)
{
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {

  // Setup Serial monitor
  Serial.begin(115200);
  delay(100);


  // Set ESP32 WiFi mode to Station temporarly
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Define callback
  esp_now_register_send_cb(OnDataSent);


  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

}

void loop() {


 force_1 = analogRead(FSRPin_1);
 vout_1 = (force_1*5)/1023.0;
 vout_1 = (vout_1*cf)/4.448;
 Serial.print("Palm Sensor 1 = ");
 Serial.println(vout_1);

 force_2 = analogRead(FSRPin_2);
 vout_2 = (force_2*5)/1023.0;
 vout_2 = (vout_2*cf)/4.448;
 Serial.print("Palm Sensor 2 = ");
 Serial.println(vout_2);

 force_3 = analogRead(FSRPin_3);
 vout_3 = (force_3*5)/1023.0;
 vout_3 = (vout_3*cf)/4.448;
 Serial.print("Palm Sensor 3 = ");
 Serial.println(vout_3);

  // Add to structured data object
  myData.a = vout_1;
  myData.b = vout_2;
  myData.c = vout_3;




  // Send data
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  // Delay for A101 sensor
  delay(500);
}
