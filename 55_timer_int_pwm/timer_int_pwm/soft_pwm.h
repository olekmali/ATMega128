//* soft_pwm.h *
#ifndef SOFT_PWM_H_
#define SOFT_PWM_H_
#include <stdint.h>

typedef uint8_t pwm_cnt_t;
                        // this typedef works with PWM_RESOLUTION<=255 and PWM_100_PERCENT<=255

void        PWM_generator_interrupt (void);
void        PWM_generator_setParam  (uint8_t channel, pwm_cnt_t percentage);
pwm_cnt_t   PWM_generator_getParam  (pwm_cnt_t channel);

#define PWM_RESOLUTION  (100)
                        // this is actually 1/resolution. 100 will yield 1% resolution of PWM
#define PWM_CH_MAX      (6)

#define PWM_CH_MASK     (0x3F)      
                        // allows state of bits or LEDs above PWM_CH_MAX to be preserved instead of set to 0

#define PWM_100_PERCENT (100)
                        // if resolution >100 is desired then 100% must be represented by a larger integer value
                        

#endif /* SOFT_PWM_H_ */
