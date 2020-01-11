#include "flags.h"
#include "flags_static.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

/*
Mutes para prevenir data races 
*/
static SemaphoreHandle_t flags_mutex;
static TickType_t flags_max_block_time;

flags_t CONTROL_FLAGS;

void flags_init(){
    flags_max_block_time = pdMS_TO_TICKS(500); //La lectura/escritura se bloqueará max por 500 ms
    CONTROL_FLAGS.raw_flags = 0;
    flags_mutex = xSemaphoreCreateMutex();
    if(flags_mutex != NULL){
        _flags_give_mutex();
    }
}

bool flags_is_nvs_ok(){
    bool ret = false;
    if(_flags_take_mutex()){
        ret = CONTROL_FLAGS.nvs ? true : false;
    }
    _flags_give_mutex();
    return ret;
}

void flags_set_nvs_ok(){
    if(_flags_take_mutex()){
        CONTROL_FLAGS.nvs = 1;
    }
    _flags_give_mutex();
}

bool flags_is_wifi_connected(){
    bool ret = false;
    if(_flags_take_mutex()){
        ret = CONTROL_FLAGS.wifi_connected ? true : false;
    }
    _flags_give_mutex();
    return ret;
}

void flags_set_wifi_connected(){
    if(_flags_take_mutex()){
        CONTROL_FLAGS.wifi_connected = 1;
    }
    _flags_give_mutex();
}

void flags_reset_wifi_connected(){
    if(_flags_take_mutex()){
        CONTROL_FLAGS.wifi_connected = 0;
    }
    _flags_give_mutex();
}

void flags_reset_mqtt_connected(){
    if(_flags_take_mutex()){
        CONTROL_FLAGS.mqtt_connected = 0;
    }
    _flags_give_mutex();
}

bool flags_is_mqtt_connected(){
    bool ret = false;
    if(_flags_take_mutex()){
        ret = CONTROL_FLAGS.mqtt_connected ? true : false;
    }
    _flags_give_mutex();
    return ret;
}

void flags_set_mqtt_connected(){
    if(_flags_take_mutex()){
        CONTROL_FLAGS.mqtt_connected = 1;
    }
    _flags_give_mutex();
}


static bool _flags_take_mutex(){
    if(xSemaphoreTake(flags_mutex, flags_max_block_time) == pdPASS){
        return true;
    }
    else{
        return false;
    }
}

static void _flags_give_mutex(){
    xSemaphoreGive(flags_mutex);
}