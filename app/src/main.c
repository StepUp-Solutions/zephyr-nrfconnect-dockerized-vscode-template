#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "example/adder.h"

LOG_MODULE_REGISTER(blinky, LOG_LEVEL_DBG);

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000
#define RECEIVE_TIMEOUT 100

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE   DT_ALIAS(led0)
#define LED1_NODE   DT_ALIAS(led1)
#define LED2_NODE   DT_ALIAS(led2)
#define SW0_NODE    DT_ALIAS(sw0)
#define UART0_LABEL DT_NODELABEL(uart0)

#define RECEIVE_BUFF_SIZE 10

/* STEP 9.1 - Define the transmission buffer, which is a buffer to hold the data
 * to be sent over UART */
static uint8_t tx_buf[] = {
    "nRF Connect SDK Fundamentals Course\n\r"
    "Press 1-3 on your keyboard to toggle LEDS 1-3 on your development "
    "kit\n\r"
};

/* STEP 10.1.2 - Define the receive buffer */
static uint8_t rx_buf[RECEIVE_BUFF_SIZE] = { 0 };

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
const struct device*             uart = DEVICE_DT_GET(UART0_LABEL);

static void
uart_cb(const struct device* dev, struct uart_event* evt, void* user_data)
{
    switch (evt->type)
    {
        case UART_RX_RDY:
            if ((evt->data.rx.len) == 1)
            {
                if (evt->data.rx.buf[evt->data.rx.offset] == '1')
                    gpio_pin_toggle_dt(&led0);
                else if (evt->data.rx.buf[evt->data.rx.offset] == '2')
                    gpio_pin_toggle_dt(&led1);
                else if (evt->data.rx.buf[evt->data.rx.offset] == '3')
                    gpio_pin_toggle_dt(&led2);
            }
            break;
        case UART_RX_DISABLED:
            uart_rx_enable(dev, rx_buf, sizeof rx_buf, RECEIVE_TIMEOUT);
            break;

        default:
            break;
    }
}

int
main(void)
{
    int ret;

    LOG_DBG("Adder add result: %d", add(2, 5));

    if (!device_is_ready(uart))
    {
        LOG_ERR("UART device not ready");
        return 1;
    }

    ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
    if (ret < 0)
    {
        return 1;
    }
    ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
    if (ret < 0)
    {
        return 1;
    }
    ret = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);
    if (ret < 0)
    {
        return 1;
    }

    ret = uart_callback_set(uart, uart_cb, NULL);
    if (ret)
    {
        return 1;
    }

    ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
    if (ret < 0)
    {
        return 1;
    }
    ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
    if (ret < 0)
    {
        return 1;
    }
    ret = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);
    if (ret < 0)
    {
        return 1;
    }

    ret = uart_tx(uart, tx_buf, sizeof(tx_buf), SYS_FOREVER_US);
    if (ret)
    {
        return 1;
    }

    ret = uart_rx_enable(uart, rx_buf, sizeof rx_buf, RECEIVE_TIMEOUT);
    if (ret)
    {
        return 1;
    }

    while (1)
    {
        k_msleep(SLEEP_TIME_MS);
    }
    return 0;
}
