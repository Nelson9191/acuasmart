#ifndef _AM_WIFI_H_
#define _AM_WIFI_H_

#include <stdbool.h>

bool am_wifi_init();

void am_wifi_start_task();

void wifi_config_task(void * pvParameters);



#endif
