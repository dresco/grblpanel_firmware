#ifndef __QUADRATURE_H__
#define __QUADRATURE_H__

// Maximum number of encoders supported
#define MAX_ENCODERS 8

#define ENCODER_NODE_1 DT_NODELABEL(encoder1)
#define ENCODER_NODE_2 DT_NODELABEL(encoder2)
#define ENCODER_NODE_3 DT_NODELABEL(encoder3)
#define ENCODER_NODE_4 DT_NODELABEL(encoder4)
#define ENCODER_NODE_5 DT_NODELABEL(encoder5)
#define ENCODER_NODE_6 DT_NODELABEL(encoder6)
#define ENCODER_NODE_7 DT_NODELABEL(encoder7)
#define ENCODER_NODE_8 DT_NODELABEL(encoder8)

#if (DT_NODE_EXISTS(ENCODER_NODE_1) || DT_NODE_EXISTS(ENCODER_NODE_2) || \
     DT_NODE_EXISTS(ENCODER_NODE_3) || DT_NODE_EXISTS(ENCODER_NODE_4) || \
     DT_NODE_EXISTS(ENCODER_NODE_5) || DT_NODE_EXISTS(ENCODER_NODE_6) || \
     DT_NODE_EXISTS(ENCODER_NODE_7) || DT_NODE_EXISTS(ENCODER_NODE_8))   
#define PANEL_ENCODER    1
#endif

//
// Function prototypes
//
void     quadrature_init(void);
uint16_t quadrature_get_value(uint8_t);

#endif
