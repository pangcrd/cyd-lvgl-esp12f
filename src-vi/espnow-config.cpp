#include "espnow-config.h"



/** This is all the data about the peer **/

/** RECEIVER MAC ADDRESS: 60:55:f9:af:83:ec **/
/** REPLACE WITH YOUR RECEIVER MAC Address 48:3F:DA:5C:F5:FC**/

uint8_t MACAddress[] = {0x48, 0x3F, 0xDA, 0x5C, 0xF5, 0xFC};

esp_now_peer_info_t slave;

/** Structure example to receive data **/ 

bool btn_states[4];
data dataRecv;

volatile bool dataReceived = false; /** Check data **/
unsigned long lastRecvTime = 0;
volatile bool update_info = false;
bool isConnected = false;

void (*check_timeout_callback)(void) = nullptr;
void (*on_slave_connected_callback)(void) = nullptr;

  /** The all important data! **/
  /** callback when data is sent from Master to Slave **/
  void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {


    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
    
    // esp_err_t result = esp_now_send(slave.peer_addr, (uint8_t *) &ledData, sizeof(ledData));
    // if (result == ESP_OK) {
    //     Serial.println("Sent successfully");
    // } else {
    //     Serial.println("Send failed");
    // }
    // if (status == ESP_NOW_SEND_SUCCESS) {
    //     dataReceived = true;

    //     if (on_slave_connected_callback != NULL) {
    //         on_slave_connected_callback();
    //     }

    // } else {
    //     Serial.println("SEND FAILED");
    // }
  }

/** callback function that will be executed when data is received **/ 
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) { 
  /** Copies the sender mac address to a string **/ 
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&dataRecv, incomingData, sizeof(dataRecv));
  memcpy(dataRecv.mac, mac_addr, 6); 
  Serial.printf("Board ID %d", dataRecv.id);
  Serial.printf("Uptime %d", dataRecv.uptime);
  
  lastRecvTime = millis();

    if (!isConnected && check_timeout_callback != NULL) {
        isConnected = true;
        check_timeout_callback();
    }

    if (on_slave_connected_callback != NULL) {
            on_slave_connected_callback();
        }
      update_info = true;   
}   


void Espnow_init(void* pvParams){
    //WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        vTaskDelete(NULL);
        return;
    }
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);

    memcpy(slave.peer_addr, MACAddress, 6);
    slave.channel = 0;  
    slave.encrypt = false;

    bool peerAdded = false;
    int retry = 0;
    while (!peerAdded && retry < 10) {
        if (esp_now_add_peer(&slave) == ESP_OK){
            Serial.println("Peer added successfully");
            peerAdded = true;
        } else {
            Serial.println("Failed to add peer. Retrying...");
            vTaskDelay(pdMS_TO_TICKS(500)); 
            retry++;
        }
    }

    if (!peerAdded) {
        Serial.println("Failed to add peer after retries");
    }

    vTaskDelete(NULL);  
}

/** Send data to Slave **/
void dataSend(){

esp_err_t result = esp_now_send(MACAddress, (uint8_t *) &state, sizeof(state));
    
    if (result == ESP_OK) {
        Serial.println("Sent with success");
    } else {
        Serial.println("Error sending the data");
    }
}
/** UPTIME PRASE */
TimeBreakdown parseUptime(uint32_t uptime_sec) {
  TimeBreakdown t;
  t.hours = uptime_sec / 3600;
  t.minutes = (uptime_sec % 3600) / 60;
  t.seconds = uptime_sec % 60;
  return t;
}
/********************************** WIFI CONFIG BEGIN (THIS CODE IS TEST FOR ESPNOW RUN WITH WIFI AT SAME TIME) ****************************/
  const char* ssid = "SHome";
  const char* password = "015975321";
  
  const char* ntpServer = "pool.ntp.org";

  const long gmtOffset_sec = 7 * 3600; // GMT+7
  const int daylightOffset_sec = 0;

  bool connectFlag = false; // Flag to check wifi connected or not

  String currentTime = "Unknown";
  TaskHandle_t timeTaskHandle = NULL;

  /** Let check wifi connected or not */
void startWiFiTask(void* pvParams) {

    vTaskDelay(pdMS_TO_TICKS(2000));
    //WiFi.mode(WIFI_STA);
    Serial.println("Starting WiFi connection...");
    WiFi.begin(ssid, password);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        Serial.print(".");
        vTaskDelay(pdMS_TO_TICKS(500)); 
        attempts++;
    }
    int ch = WiFi.channel();
    //esp_wifi_set_channel(6, WIFI_SECOND_CHAN_NONE);
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected.");
        connectFlag = true;
    } else {
        Serial.println("\nWiFi connection failed.");
        connectFlag  = false;
    }
    
    vTaskDelete(NULL);
}

/** Let check NTP status */ 
void startNTPTask(void* pvParams) {

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    struct tm timeinfo;
      while (!getLocalTime(&timeinfo)) {
      Serial.println("Waiting for NTP...");
      vTaskDelay(pdMS_TO_TICKS(1000)); 
      }
    Serial.println("NTP synced.");
    connectFlag = true;
    vTaskDelete(NULL);
}

void WiFiAndTimeInit() {
    xTaskCreatePinnedToCore(startWiFiTask, "WiFi Task", 4080, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(startNTPTask, "NTP Task", 4080, NULL, 1, NULL, 0);
}
void updateTimeTask(void* pvParams) {
      while (1) {
        struct tm timeinfo;
        if (getLocalTime(&timeinfo)) {
          char buffer[64];
          strftime(buffer, sizeof(buffer), "%H:%M %d/%m/%Y", &timeinfo);//%H:%M:%S
          currentTime = String(buffer);
          Serial.print("Time now: ");
          Serial.println(currentTime);
        } else {
          Serial.println("Failed to get time in update task");
        }
  
        vTaskDelay(pdMS_TO_TICKS(60000)); 
      }
    }
  
    void GetTimeTask() {
      xTaskCreatePinnedToCore(updateTimeTask, "Time Task", 3072, NULL, 0, &timeTaskHandle, 0);
    }
/********************************** WIFI CONFIG END****************************/
