#include <ESP8266WiFi.h>
#include <espnow.h>

#define NUM_RLY  4   
int rly_pins[NUM_RLY] = {16,14,12,13};  /** LED OR RELAY GPIO **/ 

/** Set your Board ID (ESP32 Sender #1 = BOARD_ID 1, ESP32 Sender #2 = BOARD_ID 2, etc) **/ 
#define BOARD_ID 3
//#define BOARD_ID 2
/** MAC Address of the receiver => Master**/
uint8_t MasterAddress[] = {0xD4, 0x8A, 0xFC, 0xC8, 0xE7, 0x64};

/** Struct recv data**/
typedef struct data {
    uint8_t id;
    uint32_t uptime;
    uint8_t mac[6];
  } data;

typedef struct {
    bool btn_state[4]; 
} relay_state;

relay_state state;
data dataRecv;

unsigned int run = 0; 
unsigned long previousMillis = 0;

/** callback when data is recv from Master **/
void onDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
    memcpy(&state, incomingData, sizeof(state));
    Serial.println("Data received!");
    
    for (int i = 0; i < NUM_RLY; i++) {
        digitalWrite(rly_pins[i], state.btn_state[i] ? HIGH : LOW);
    }

    dataRecv.id = BOARD_ID;
    static uint32_t lastUptime = 0;
    if (millis() - lastUptime >= 1000) {
      lastUptime += 1000;
      dataRecv.uptime++; 
  }
    esp_now_send(MasterAddress, (uint8_t *)&dataRecv, sizeof(dataRecv));
}

/** Callback when data is sent to master **/ 
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
    
    if (sendStatus == 0) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
}
void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    /** Set OUTPUT pin **/
    for (int i = 0; i < NUM_RLY; i++) pinMode(rly_pins[i], OUTPUT);
    
    if (esp_now_init() != 0) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    // Once ESPNow is successfully Init, we will register for Send CB to
    // get the status of Trasnmitted packet
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(onDataRecv);
    
    // Add peer
    esp_now_add_peer(MasterAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
void loop() {}
