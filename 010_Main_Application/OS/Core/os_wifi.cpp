/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file os_wifi.c
        @details This module contains all required functions to manage
                 the WiFi network connection related functionality.

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#include "os_wifi.h"
#include "../Config/os_wifi_cfg.h"

#include <string.h>

#include "esp_eth.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "tcpip_adapter.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "nvs_flash.h"

/*********************************************************************/
/*      GLOBAL VARIABLES                                             */
/*********************************************************************/

/*********************************************************************/
/*      PRIVATE FUNCTION DECLARATIONS                                */
/*********************************************************************/
PRIVATE void os_wifi_event_on_disconnect( void*            arg,
                                          esp_event_base_t event_base,
                                          int32_t          event_id,
                                          void*            event_data );
PRIVATE void os_wifi_event_on_got_ip( void*            arg,
                                      esp_event_base_t event_base,
                                      int32_t          event_id,
                                      void*            event_data );
/*********************************************************************/
/*   FUNCTION DEFINITIONS                                            */
/*********************************************************************/

#define GOT_IPV4_BIT BIT( 0 )
#define GOT_IPV6_BIT BIT( 1 )
#define CONNECTED_BITS ( GOT_IPV4_BIT )

static EventGroupHandle_t s_connect_event_group;
static ip4_addr_t         s_ip_addr;
static const char*        s_connection_name;

PRIVATE void os_wifi_event_on_got_ip( void*            arg,
                                      esp_event_base_t event_base,
                                      int32_t          event_id,
                                      void*            event_data )
{
    ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
    memcpy( &s_ip_addr, &event->ip_info.ip, sizeof( s_ip_addr ) );
    xEventGroupSetBits( s_connect_event_group, GOT_IPV4_BIT );
}

esp_err_t os_wifi_connect( void )
{
    if ( s_connect_event_group != NULL )
    {
        return ESP_ERR_INVALID_STATE;
    }

    s_connect_event_group = xEventGroupCreate();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init( &cfg ) );
    ESP_ERROR_CHECK( esp_event_handler_register( WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &os_wifi_event_on_disconnect, NULL ) );
    ESP_ERROR_CHECK( esp_event_handler_register( IP_EVENT, IP_EVENT_STA_GOT_IP, &os_wifi_event_on_got_ip, NULL ) );
    ESP_ERROR_CHECK( esp_wifi_set_storage( WIFI_STORAGE_RAM ) );

    wifi_config_t wifi_config = {
        .sta = {
            .ssid     = OS_WIFI_DEFAULT_SSID,
            .password = OS_WIFI_DEFAULT_PASSWORD,
        },
    };

    ESP_LOGI( OS_WIFI_LOG_MSG_TAG, "Connecting to %s...", wifi_config.sta.ssid );
    ESP_ERROR_CHECK( esp_wifi_set_mode( WIFI_MODE_STA ) );
    ESP_ERROR_CHECK( esp_wifi_set_config( ESP_IF_WIFI_STA, &wifi_config ) );
    ESP_ERROR_CHECK( esp_wifi_start() );
    ESP_ERROR_CHECK( esp_wifi_connect() );
    s_connection_name = OS_WIFI_DEFAULT_SSID;

    xEventGroupWaitBits( s_connect_event_group, CONNECTED_BITS, true, true, portMAX_DELAY );
    ESP_LOGI( OS_WIFI_LOG_MSG_TAG, "Connected to %s", s_connection_name );
    ESP_LOGI( OS_WIFI_LOG_MSG_TAG, "IPv4 address: " IPSTR, IP2STR( &s_ip_addr ) );
    return ESP_OK;
}

esp_err_t os_wifi_disconnect( void )
{
    if ( s_connect_event_group == NULL )
    {
        return ESP_ERR_INVALID_STATE;
    }
    vEventGroupDelete( s_connect_event_group );
    s_connect_event_group = NULL;

    ESP_ERROR_CHECK( esp_event_handler_unregister( WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &os_wifi_event_on_disconnect ) );
    ESP_ERROR_CHECK( esp_event_handler_unregister( IP_EVENT, IP_EVENT_STA_GOT_IP, &os_wifi_event_on_got_ip ) );
    ESP_ERROR_CHECK( esp_wifi_stop() );
    ESP_ERROR_CHECK( esp_wifi_deinit() );

    ESP_LOGI( OS_WIFI_LOG_MSG_TAG, "Disconnected from %s", s_connection_name );
    s_connection_name = NULL;
    return ESP_OK;
}

PRIVATE void os_wifi_event_on_disconnect( void*            arg,
                                          esp_event_base_t event_base,
                                          int32_t          event_id,
                                          void*            event_data )
{
    ESP_LOGI( OS_WIFI_LOG_MSG_TAG, "Wi-Fi disconnected, trying to reconnect..." );
    ESP_ERROR_CHECK( esp_wifi_connect() );
}

void os_wifi_init( void )
{
    ESP_ERROR_CHECK( nvs_flash_init() );
    tcpip_adapter_init();
    ESP_ERROR_CHECK( esp_event_loop_create_default() );

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK( os_wifi_connect() );
}

void os_wifi_main( void )
{
}
