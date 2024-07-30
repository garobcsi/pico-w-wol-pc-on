#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include <string.h>

#define STRINGIFY(x) #x
#define STRINGIFY_VALUE(x) STRINGIFY(x)

#define WOL_PORT 9  // Standard WoL port
#define GPIO_PIN 14
#define LED_PIN CYW43_WL_GPIO_LED_PIN
#define SSID your_ssid
#define PASSWORD your_password

static const uint8_t WOL_MAGIC_HEADER[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void wol_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port) {
    if (p->len == 102) {
        uint8_t *payload = (uint8_t *)p->payload;
        if (memcmp(payload, WOL_MAGIC_HEADER, sizeof(WOL_MAGIC_HEADER)) == 0) {
            cyw43_arch_gpio_put(LED_PIN, true);
            gpio_put(GPIO_PIN, false);
            sleep_ms(200);
            cyw43_arch_gpio_put(LED_PIN, false);
            gpio_put(GPIO_PIN, true);
            sleep_ms(200);
        }
    }
    pbuf_free(p);
}

int main() {
    stdio_init_all();
    if (cyw43_arch_init()) {
        printf("WiFi init failed\n");
        return 1;
    }

    gpio_init(GPIO_PIN);
    gpio_set_dir(GPIO_PIN, GPIO_OUT);
    gpio_put(GPIO_PIN, true);

    cyw43_arch_enable_sta_mode();

    if (cyw43_arch_wifi_connect_timeout_ms(STRINGIFY_VALUE(SSID), STRINGIFY_VALUE(PASSWORD), CYW43_AUTH_WPA2_AES_PSK, 30000) != 0) {
        printf("WiFi connection failed\n");
        cyw43_arch_gpio_put(LED_PIN, true);
        return 1;
    }

    printf("Connected to WiFi\n");

    struct udp_pcb *pcb;
    pcb = udp_new_ip_type(IPADDR_TYPE_ANY);
    if (!pcb) {
        printf("Failed to create PCB\n");
        cyw43_arch_gpio_put(LED_PIN, true);
        return 1;
    }

    if (udp_bind(pcb, IP_ADDR_ANY, WOL_PORT) != ERR_OK) {
        printf("UDP bind failed\n");
        udp_remove(pcb);
        cyw43_arch_gpio_put(LED_PIN, true);
        return 1;
    }

    udp_recv(pcb, wol_recv, NULL);

    while (true) {
        cyw43_arch_poll();
    }

    cyw43_arch_deinit();
    return 0;
}
