#ifndef __QUADRATURE_H__
#define __QUADRATURE_H__

#include <stm32f4xx_ll_tim.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_bus.h>

#define IC_PRESCALER    LL_TIM_ICPSC_DIV1
#define IC_FILTER       LL_TIM_IC_FILTER_FDIV1_N2

//
// Function prototypes
//
void     quadrature_init(void);
void     quadrature_tim2_ll_init(void);
void     quadrature_tim3_ll_init(void);
void     quadrature_tim4_ll_init(void);
void     quadrature_tim5_ll_init(void);
uint16_t quadrature_get_value(uint8_t);

#endif
