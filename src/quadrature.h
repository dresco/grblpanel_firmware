#ifndef __QUADRATURE_H__
#define __QUADRATURE_H__

#include <stm32f4xx_ll_tim.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_bus.h>

//
// Function prototypes
//
void     quadrature_init(void);
void     quadrature_tim2_ll_init(void);
uint32_t quadrature_get_value(void);

#endif
