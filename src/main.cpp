#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>


constexpr uint16_t PERIOD_MS(1000);

void blink_LED () {
    if (timer_get_counter(TIM1) < PERIOD_MS / 2) gpio_set(GPIOE, GPIO9);
    else gpio_clear(GPIOE, GPIO9);
}

void LED_gpio_setup () {
    rcc_periph_clock_enable(RCC_GPIOE);
    gpio_mode_setup(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO9 | GPIO11);
    gpio_mode_setup(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO14);
    gpio_set_af(GPIOE, GPIO_AF2, GPIO14);
}

void timer_setup () {
    rcc_periph_clock_enable(RCC_TIM1);
    timer_set_prescaler(TIM1, rcc_get_timer_clk_freq(TIM1) / PERIOD_MS - 1);
    timer_set_period(TIM1, PERIOD_MS - 1);

    timer_enable_irq(TIM1, TIM_DIER_UIE);
    nvic_enable_irq(NVIC_TIM1_UP_TIM16_IRQ);

    timer_enable_counter(TIM1);
}

int main () {
    rcc_clock_setup_pll(
        &rcc_hsi_configs[RCC_CLOCK_HSI_64MHZ]
    );

    LED_gpio_setup();
    timer_setup();

    while (true) {
        blink_LED();
    }
}

void tim1_up_tim16_isr(void) {
    timer_clear_flag(TIM1, TIM_SR_UIF);
    gpio_toggle(GPIOE, GPIO11);
}