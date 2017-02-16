//* soft_pwm.h *
#ifndef SOFT_PWM_ALT_H_
#define SOFT_PWM_ALT_H_
#include <stdint.h>

void        PWM_generator_interrupt (void);
void        PWM_generator_setFreq   (uint32_t int_frq, uint16_t pwm_frq);
void        PWM_generator_setCoef   (uint8_t channel, uint8_t percentage);
uint8_t     PWM_generator_getCoef   (uint8_t channel);

#define PWM_CH_MAX      (6)

#define PWM_CH_MASK     (0x3F)      
                        // allows state of bits or LEDs above PWM_CH_MAX to be preserved instead of set to 0

#define PWM_100_PERCENT (100)
                        // if resolution >100 is desired then 100% must be represented by a larger integer value
                        

#endif /* SOFT_PWM_ALT_H_ */
