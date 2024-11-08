#include "my_mqtt.h"

#include "mqtt_client.h"

#include <inttypes.h>

#include "esp_event.h"
#include "esp_log.h"

#include "my_led.h"

#include "esp_timer.h"

#include <esp_wifi.h>

#include "my_getip.h"

static const char *TAG = "my_mqtt";
static const char *VERSION = "p1.0";

int64_t from;

void start_clock(void)
{
    from = esp_timer_get_time();
}

void stop_clock(void)
{
    int64_t to = esp_timer_get_time();
    ESP_LOGI(TAG, "MQTT elapsed time: %lld us", to - from);
}

static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
esp_mqtt_client_handle_t client = NULL;

char espID[7];
char topic_out[20];
char topic_in[20];
char topic_id[20];
char topic_inid[20] = "";
char payload[255];

// void myprintstr(char *str) {
//     printf("str= %s\n", str);
//     printf("size str= %d\n", sizeof(str));
//     printf("len str= %d\n", strlen(str));
// }

char espIP[20];

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");

        snprintf(payload, sizeof(payload),
            "{\"id\":\"%s\",\"ip\":\"%s\",\"ver\":\"%s\"}", espID, espIP, VERSION);
        msg_id = esp_mqtt_client_publish(client, "decel/power/on", payload, 0, 1, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, topic_in, 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        msg_id = esp_mqtt_client_subscribe(client, topic_id, 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s (len=%d)\r\n", event->topic_len, event->topic, event->topic_len);
        printf("DATA=%.*s (len=%d)\r\n", event->data_len, event->data, event->data_len);
        if ((strncmp(event->topic, topic_in, event->topic_len) == 0) ||
                (strncmp(event->topic, topic_inid, event->topic_len) == 0)) {
            event->data[event->data_len] = '\0';
            printf("LED Topic, data = %s\n", event->data);
            // stop_clock();
            set_leds(event->data);
        }
        if (strncmp(event->topic, topic_id, event->topic_len) == 0) {
            printf("new ID = %s\n", event->data);
            if (strlen(topic_inid) != 0)
                msg_id = esp_mqtt_client_unsubscribe(client, topic_inid);
            snprintf(topic_inid, 20, "decel/%.*s/out", event->data_len, event->data);
            msg_id = esp_mqtt_client_subscribe(client, topic_inid, 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        }        
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

extern const uint8_t client_cert_pem_start[] asm("_binary_client_crt_start");
extern const uint8_t client_cert_pem_end[] asm("_binary_client_crt_end");
extern const uint8_t client_key_pem_start[] asm("_binary_client_key_start");
extern const uint8_t client_key_pem_end[] asm("_binary_client_key_end");
extern const uint8_t server_cert_pem_start[] asm("_binary_broker_crt_start");
extern const uint8_t server_cert_pem_end[] asm("_binary_broker_crt_end");

void mqtt_app_start(void)
{
    int err;
    err = my_get_ip(espIP);
    ESP_LOGI(TAG, "my_get_ip err=%d!", err);
    printf("IP: %s\r\n", espIP);

    uint8_t eth_mac[6];
    esp_wifi_get_mac(WIFI_IF_STA, eth_mac);
    snprintf(espID, sizeof(espID), "%02x%02x%02x", eth_mac[3], eth_mac[4], eth_mac[5]);

    snprintf(topic_out, sizeof(topic_out), "decel/%s/out", espID);
    snprintf(topic_in, sizeof(topic_in), "decel/%s/in", espID);
    snprintf(topic_id, sizeof(topic_id), "decel/%s/id", espID);

    const esp_mqtt_client_config_t mqtt_cfg = {
        // .broker.address.uri = "mqtt://decel:decel2023@nl.h3lio.org:21883",
        // .broker.address.uri = "mqtts://decel:decel2023@oracle23nl.me:28883",
        .broker.address.uri = "mqtts://decel:decel2023@nl.h3lio.org:28883",
        .broker.verification.certificate = (const char *)server_cert_pem_start,
        .credentials = {
            .authentication = {
                .certificate = (const char *)client_cert_pem_start,
                .key = (const char *)client_key_pem_start,
            },
        },
        .session = {
            .last_will = {
                .topic = (const char *)"decel/power/off",
                .msg = espID,
                .msg_len = 6,
                .qos = 0,
                .retain = 0
            },
            .keepalive = 10
        }
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

void publish_msg(char *msg)
{
    ESP_LOGI(TAG, "PUBLISH_MSG");
    if (client != NULL)
        esp_mqtt_client_publish(client, topic_out, msg, 0, 1, 0); // 4600us
        // esp_mqtt_client_enqueue(client, "myproj/button", msg, 0, 0, 0, true); // 900us
}
