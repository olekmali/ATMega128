// board.h
/***************************************************
			D E F I N I T I O N S
****************************************************/

/* LCD Data: Port A: D0-3
* Port A6: RS (0 = command, 1=character)
* Port A5: Read/Write bar (keep low after startup)
* Port A4: Enable (data strobed in on high to low transition)
*/

#define LCD_DATA_PORT 	PORT(LCD_DATA)
#define LCD_E_PORT 		PORT(LCD_E)
#define LCD_RS_PORT 	PORT(LCD_RS)
#define LCD_RW_PORT		PORT(LCD_RW)
#define LCD_DATA_DDR 	DDR(LCD_DATA)
#define LCD_E_DDR 		DDR(LCD_E)
#define LCD_RS_DDR 		DDR(LCD_RS)
#define LCD_DATA_PIN	PIN(LCD_DATA)
#define SET_E() (LCD_E_PORT|=(1<<LCD_E_POS))
#define SET_RS() (LCD_RS_PORT|=(1<<LCD_RS_POS))
#define CLEAR_E() (LCD_E_PORT&=(~(1<<LCD_E_POS)))
#define CLEAR_RS() (LCD_RS_PORT&=(~(1<<LCD_RS_POS)))
#define CLEAR_RW() (LCD_RW_PORT&=(~(1<<LCD_RW_POS)))

/* counts for gen_cnt, decremented by Timer 0 */
#define ms_250	255					// 250ms delay
#define ms_100	100					// 100ms delay
#define ms_50	50					// 50ms delay
#define ms_25	25					// 25ms delay
#define ms_5	5					// 5ms delay
#define ms_2	2					// 2 ms delay

/* counts for delay_us_ctr, a delay loop (0.655us per count average) */
#define us_50	75					// 50 us delay
#define us_10	16					// 10 us delay
#define us_2	1					// 2 us delay

/* counts for delay_ms_ctr, a delay loop (0.98ms per count average) */
#define ms_1	1				// 1 ms delay

/***************************************************
			F U N C T I O N S
****************************************************/
void keypad(void);
void key_init(void);
void delay(void);
void init_timer_ports(void);
void delay_us(void);
void delay_ms(void);
void prog_init(void);
void delay_1sec(void);
void delay_2sec(void);
void lcd_test(void);
void keypad_test(void);
void led_test(void);
void switch_test(void);
void display_init(void);