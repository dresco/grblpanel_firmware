#include <zephyr.h>

void main(void)
{
    printk("Hello World...! %s\n", CONFIG_BOARD);
}
