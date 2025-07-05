#ifndef ESPNOWCONFIG_H
#define ESPNOWCONFIG_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp_now.h>
#include <WiFi.h>
#include "ui/ui.h"

/** Struct recv data**/
typedef struct data {
    uint8_t id;
    uint32_t uptime;
    uint8_t mac[6];
  } data;
/** Convert Uptime to hh:mm:ss */
typedef struct {
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
} TimeBreakdown;

TimeBreakdown parseUptime(uint32_t uptime_sec);
extern data dataRecv;

extern volatile bool dataReceived ;
extern unsigned long lastRecvTime;
extern volatile bool update_info;
extern bool isConnected;
extern String currentTime;

extern bool btn_states[];
extern bool connectFlag;


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len);
void Espnow_init(void* pvParams);
void dataSend();
void WiFiAndTimeInit();
void GetTimeTask();
//void processDataCallback();


extern void (*check_timeout_callback)(void);
extern void (*on_slave_connected_callback)(void);

#endif