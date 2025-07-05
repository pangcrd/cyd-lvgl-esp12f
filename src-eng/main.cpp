
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Example CYD  ESP32-2432S028R [ESP NOW DHT22]                            // 
// Design UI on Squareline Studio. LVGL V9.1                               //
// Youtube:https://www.youtube.com/@pangcrd                                //
// Github: https://github.com/pangcrd                                      //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include <SPIFFS.h>
#include "FS.h"
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <SPI.h>
#include "lvgl.h"
#include "ui/ui.h"
#include "espnow-config.h"


relay_state state = {false};
// Function pointer for saving state to flash
void (*save_state_callback)(void) = nullptr;

lv_timer_t* no_connect_timer = nullptr;
bool nofi_connect = false;

unsigned long now = 0;

/*Don't forget to set Sketchbook location in File/Preferences to the path of your UI project (the parent foder of this INO file)*/
/*Change to your screen resolution*/
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;


enum { SCREENBUFFER_SIZE_PIXELS = screenWidth * screenHeight / 10 };
static lv_color_t buf [SCREENBUFFER_SIZE_PIXELS];

TFT_eSPI tft = TFT_eSPI( screenWidth, screenHeight ); /* TFT instance */
/*Touch screen config*/
#define XPT2046_IRQ 36 //GPIO driver cảm ứng 
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33

SPIClass tsSpi = SPIClass(VSPI);
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);

//Run calib_touch files to get value 
uint16_t touchScreenMinimumX = 200, touchScreenMaximumX = 3700, touchScreenMinimumY = 240,touchScreenMaximumY = 3800; //Chạy Calibration để lấy giá trị mỗi màn hình mỗi khác

/* Display flushing */
void my_disp_flush (lv_display_t *disp, const lv_area_t *area, uint8_t *pixelmap)
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    if (LV_COLOR_16_SWAP) {
        size_t len = lv_area_get_size( area );
        lv_draw_sw_rgb565_swap( pixelmap, len );
    }

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( (uint16_t*) pixelmap, w * h, true );
    tft.endWrite();


    lv_disp_flush_ready( disp );
}

/** ========== Read the touch driver ========== **/
void my_touch_read (lv_indev_t *indev_drv, lv_indev_data_t * data)
{
    if(ts.touched())
    {
        TS_Point p = ts.getPoint();
        //Some very basic auto calibration so it doesn't go out of range
        if(p.x < touchScreenMinimumX) touchScreenMinimumX = p.x;
        if(p.x > touchScreenMaximumX) touchScreenMaximumX = p.x;
        if(p.y < touchScreenMinimumY) touchScreenMinimumY = p.y;
        if(p.y > touchScreenMaximumY) touchScreenMaximumY = p.y;
        //Map this to the pixel position
        data->point.x = map(p.x,touchScreenMinimumX,touchScreenMaximumX,1,screenWidth); /* Touchscreen X calibration */
        data->point.y = map(p.y,touchScreenMinimumY,touchScreenMaximumY,1,screenHeight); /* Touchscreen Y calibration */
        data->state = LV_INDEV_STATE_PR;

        // Serial.print( "Touch x " );
        // Serial.print( data->point.x );
        // Serial.print( " y " );
        // Serial.println( data->point.y );
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }
}

/*Set tick routine needed for LVGL internal timings*/
static uint32_t my_tick_get_cb (void) { return millis(); }

/********************************** SPIFFS Begin ******************************* */
void spiffs_init() {
    if (!SPIFFS.begin(true)) {
        //Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    File file = SPIFFS.open("/spiffs.txt", "r");
    if (file) {
        String savedState = file.readString();
        //Serial.printf("Raw saved state: %s\n", savedState.c_str());
        
        for (int i = 0; i < 4; ++i) {
            if (i < savedState.length()) {
                state.btn_state[i] = (savedState[i] == '1');
            } else {
                state.btn_state[i] = false;
            }
        }
        file.close();
        //Serial.printf("Loaded state: btn1=%d, btn2=%d, btn3=%d, btn4=%d\n", 
                     //state.btn_state[0], state.btn_state[1], 
                    // state.btn_state[2], state.btn_state[3]);
    } else {
        //Serial.println("No previous state found, using default.");
        for (int i = 0; i < 4; ++i) {
            state.btn_state[i] = false;
        }
    }
    
    update_all_buttons_ui();
}

void saveStateToFlash() {
    File file = SPIFFS.open("/spiffs.txt", "w");
    if (file) {
        String stateStr = "";
        for (int i = 0; i < 4; ++i) {
            stateStr += (state.btn_state[i] ? '1' : '0');
        }
        file.print(stateStr);
        file.close();
        //Serial.printf("State saved: %s\n", stateStr.c_str());
    }
}
/************************************ SPIFFS END ********************************* */

/************************************ NOTIFICATION POPUP BEGIN ********************************* */
void no_connect_panel(lv_timer_t * timer) {
    if (lv_obj_has_flag(ui_Panel2, LV_OBJ_FLAG_HIDDEN)) {
        lv_obj_clear_flag(ui_Panel2, LV_OBJ_FLAG_HIDDEN);  
    } else {
        lv_obj_add_flag(ui_Panel2, LV_OBJ_FLAG_HIDDEN);    
    }
}

void on_slave_connected() {
    if (no_connect_timer != NULL) {
        lv_timer_del(no_connect_timer);
        no_connect_timer = NULL;
    }
    lv_obj_add_flag(ui_Panel2, LV_OBJ_FLAG_HIDDEN); 
}

void hide_panel3_cb(lv_timer_t * timer) {
    lv_obj_add_flag(ui_Panel3, LV_OBJ_FLAG_HIDDEN);
    lv_timer_del(timer);
}

void show_connected_popup() {
    //lv_obj_add_flag(ui_Panel2, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text_fmt(ui_Label3, "%s Found board ID: %d", LV_SYMBOL_OK, dataRecv.id);
    //Serial.printf("Board ID %d", dataRecv.id);
    lv_obj_clear_flag(ui_Panel3, LV_OBJ_FLAG_HIDDEN);
    lv_timer_create(hide_panel3_cb, 2000, NULL);  
}
void show_info_tab() {
    data temp;
    memcpy(&temp, &dataRecv, sizeof(temp)); 

    TimeBreakdown t = parseUptime(temp.uptime);
    // Format string 
    char time_str[16];
    snprintf(time_str, sizeof(time_str), "%02d:%02d:%02d", t.hours, t.minutes, t.seconds);

    lv_label_set_text_fmt(ui_Label5, "%s BOARD ID: %d", LV_SYMBOL_OK, temp.id);
    lv_label_set_text_fmt(ui_Label6, "%s UPTIME: %s", LV_SYMBOL_OK, time_str);
    lv_label_set_text_fmt(ui_Label7, "%s MAC: %02X:%02X:%02X:%02X:%02X:%02X", LV_SYMBOL_OK,
                            temp.mac[0], temp.mac[1], temp.mac[2],
                            temp.mac[3], temp.mac[4], temp.mac[5]);
}
/************************************ NOTIFICATION POPUP END ********************************* */

void sendDataTask(void* pvParams) {
    while (1) {
        dataSend();
        vTaskDelay(pdMS_TO_TICKS(200)); 
    }  
}

void setup (){

    Serial.begin( 115200 );
    
    save_state_callback = saveStateToFlash;
    check_timeout_callback = show_connected_popup;
    on_slave_connected_callback = on_slave_connected;

    WiFi.mode(WIFI_AP_STA);

    lv_init();

    //Initialise the touchscreen
    tsSpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS); /* Start second SPI bus for touchscreen */
    ts.begin(tsSpi);      /* Touchscreen init */
    ts.setRotation(3);   /* Inverted landscape orientation to match screen */

    tft.begin();         /* TFT init */
    tft.setRotation(3); /* Landscape orientation, flipped */
                                            
    static lv_disp_t* disp;
    disp = lv_display_create( screenWidth, screenHeight );
    lv_display_set_buffers( disp, buf, NULL, SCREENBUFFER_SIZE_PIXELS * sizeof(lv_color_t), LV_DISPLAY_RENDER_MODE_PARTIAL );
    lv_display_set_flush_cb( disp, my_disp_flush );

    //Initialize the input device. For LVGL version 9+ only
    lv_indev_t *touch_indev = lv_indev_create();
    lv_indev_set_type(touch_indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(touch_indev, my_touch_read);


    lv_tick_set_cb( my_tick_get_cb );

    ui_init();
    spiffs_init(); // Initialize SPIFFS

    no_connect_timer = lv_timer_create(no_connect_panel, 2000, NULL);

    // WiFiAndTimeInit();
    // GetTimeTask();
    // delay(1000);
    xTaskCreatePinnedToCore(Espnow_init, "Create espnow", 8192, NULL, 6, NULL, 0);
    xTaskCreatePinnedToCore(sendDataTask, "Send Data", 5120, NULL, 5, NULL, 0);
    //Serial.println( "Setup done" );
}

void loop ()
{   

    lv_timer_handler(); /* let the GUI do its work */
    delay(5);  
    now = millis();
    /** if no data received for 8 seconds, show the no connect panel dataReceived */
    if (now - lastRecvTime > 8000) {
        if (isConnected) {
            isConnected = false;
            if (no_connect_timer != NULL) {
                lv_timer_del(no_connect_timer);
                no_connect_timer = NULL;
            }
            no_connect_timer = lv_timer_create(no_connect_panel, 2000, NULL);
        }
    } 

    if (update_info) {
        show_info_tab();  
        update_info = false;
    }
}