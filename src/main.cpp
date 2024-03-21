#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

#include "ring_buf/ring_buf.hpp"

uint8_t c{'a'};

Ring_buffer buf;

void setup () {
    rcc_periph_clock_enable(RCC_GPIOA);

    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2 | GPIO3);
    gpio_set_af(GPIOA, GPIO_AF7, GPIO2 | GPIO3);

    rcc_periph_clock_enable(RCC_USART2);

    usart_set_baudrate(USART2, 115200);
    usart_set_databits(USART2, 8);
    usart_set_stopbits(USART2, USART_STOPBITS_1);
    usart_set_parity(USART2, USART_PARITY_NONE);

    usart_set_mode(USART2, USART_MODE_TX_RX);
    usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

    usart_enable_rx_interrupt(USART2);
    nvic_enable_irq(NVIC_USART2_EXTI26_IRQ);

    usart_enable(USART2);

    rcc_periph_clock_enable(RCC_GPIOE);

    gpio_mode_setup(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO9 | GPIO11 | GPIO15);


}

void loop () {
    if(!buf.empty()) {
        c = buf.get();
    }
    usart_send_blocking(USART2, c);
    for(volatile uint32_t i=0; i<200000; i++);
    gpio_toggle(GPIOE, GPIO9);
}

int main () {
    setup();
    gpio_set(GPIOE, GPIO15);
    while (true) {
        loop();
    }   
}

void usart2_exti26_isr(void) {
    USART_RQR(USART2) &= ~(USART_RQR_RXFRQ);
    buf.put(static_cast<uint8_t>(usart_recv(USART2)));
    //static uint8_t c = static_cast<uint8_t>(usart_recv(USART2));
    gpio_set(GPIOE, GPIO11);
}