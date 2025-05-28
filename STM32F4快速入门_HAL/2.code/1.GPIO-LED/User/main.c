#include "bsp_init.h"

int main(void)
{
    bsp_init();
    while(1)
    {
        LED_ON(LED0_GPIO_Pin);
        delay_ms(500);
        LED_OFF(LED0_GPIO_Pin);
        delay_ms(500);
        LED_ON(LED1_GPIO_Pin);
        delay_ms(500);
        LED_OFF(LED1_GPIO_Pin);
        delay_ms(500);
    }
}
