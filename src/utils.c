#include <main.h>

#include <logging/log.h>
LOG_MODULE_DECLARE(panel, LOG_LEVEL_INF);

// helper functions for testing and setting bits across arrays
uint8_t test_bit( uint8_t A[],  uint8_t k )
{
    return ( (A[k/8] & (1 << (k%8) )) != 0 ) ;
}

void  set_bit( uint8_t A[],  uint8_t k )
{
    A[k/8] |= 1 << (k%8);  // Set the bit at the k-th position in A[i]
}
