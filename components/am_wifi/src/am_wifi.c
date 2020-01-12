#include "am_wifi.h"

#include "esp_wifi.h"
#include "iot_wifi.h"

void wifi_config_task(void * pvParameters){
    WIFINetworkParams_t xNetworkParams;
    WIFIReturnCode_t xWifiStatus;
    int ctr_connect = 0;
    bool connected = false;
    bool connecting = false;
    struct WIFIMsg wifi_msg;
    
    ( void ) pvParameters;

    xNetworkParams.pcSSID = WIFI_SSID;
    xNetworkParams.ucSSIDLength = sizeof( WIFI_SSID );
    xNetworkParams.pcPassword = WIFI_PASSWORD;
    xNetworkParams.ucPasswordLength = sizeof( WIFI_PASSWORD );
    xNetworkParams.xSecurity = WIFI_SECURITY;

    for(;;){   

        if(!connected && !connecting){
            ctr_connect = 0;
            printf("connecting to: %s - %s\n", WIFI_SSID, WIFI_PASSWORD);
            xWifiStatus = WIFI_ConnectAP( &( xNetworkParams ) );
            if( xWifiStatus == eWiFiSuccess ){ 
                configPRINTF( ( "WiFi connected!\r\n") );
            }
            else{
                configPRINTF( ( "Unable to CONNECT...\r\n" ) );
                connected = false;
                connecting = true;
            }
        }

        if(connecting && !connected && ctr_connect++>10){
            connecting = false;
            connected = false;
        }

        while(xQueueReceive(wifi_queue, &wifi_msg, NULL) != errQUEUE_EMPTY){
            switch(wifi_msg.status){
                case SYSTEM_EVENT_STA_START:
                    printf("SYSTEM_EVENT_STA_START\n");                        
                    break;
                case SYSTEM_EVENT_STA_CONNECTED:
                    printf("SYSTEM_EVENT_STA_CONNECTED\n");               
                    connected = true;
                    connecting = false;                       
                    break;
                case SYSTEM_EVENT_STA_GOT_IP:
                    printf("SYSTEM_EVENT_STA_GOT_IP\n");
                    connected = true;
                    connecting = false;                  
                    flags_set_wifi_connected();
                    break;
                case SYSTEM_EVENT_STA_DISCONNECTED:
                    printf("SYSTEM_EVENT_STA_DISCONNECTED\n");                
                    connected = false;
                    connecting = false;
                    flags_reset_wifi_connected();
                    break;
                default:
                    break;
                }
        }


        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}