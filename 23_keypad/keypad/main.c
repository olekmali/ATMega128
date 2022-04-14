//* keypad test - main.c *
#include <stdint.h>
#include "bootmenu.h"
#include "lib_kpad_test.h"



int main(void)
{
    const uint8_t mode = boottime_menu();
    if (mode!=0)
    {           // diagnostic behavior
        if (9<=mode)        test7_getkey_autorepeat();
        else if (8==mode)   test5_getkey_value();
        else if (4<=mode)   test4_keypressed_status();
        else if (3==mode)   test2_keypad_status_low();
        else if (2==mode)   test3_keypad_status_high();
        else if (1==mode)   test1_count_up();
    } else {    // default functionality    
        test6_getkey_debounce();
    }
    return(0);
}
