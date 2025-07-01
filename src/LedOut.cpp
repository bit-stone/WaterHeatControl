#include <LedOut.h>

void set_white_led()
{
    PORTF_OUTSET = PIN5_bm;
}

void clear_white_led()
{
    PORTF_OUTCLR = PIN5_bm;
}

void set_green_led()
{
    PORTC_OUTSET = PIN6_bm;
}

void clear_green_led()
{
    PORTC_OUTCLR = PIN6_bm;
}

void set_yellow_led()
{
    PORTB_OUTSET = PIN2_bm;
}

void clear_yellow_led()
{
    PORTB_OUTCLR = PIN2_bm;
}

void set_red_led()
{
    PORTF_OUTSET = PIN4_bm;
}

void clear_red_led()
{
    PORTF_OUTCLR = PIN4_bm;
}

void set_all_led()
{
    PORTF_OUTSET = PIN4_bm | PIN5_bm;
    PORTB_OUTSET = PIN2_bm;
    PORTC_OUTSET = PIN6_bm;
}

void clear_all_led()
{
    PORTF_OUTCLR = PIN4_bm | PIN5_bm;
    PORTB_OUTCLR = PIN2_bm;
    PORTC_OUTCLR = PIN6_bm;
}

void init_led_output()
{
    // LED configuration
    // red - PF4
    // yellow - PB2
    // green - PC6
    // white - PF5

    PORTF_DIRSET = PIN4_bm | PIN5_bm;
    PORTB_DIRSET = PIN2_bm;
    PORTC_DIRSET = PIN6_bm;

    set_all_led();
    _delay_ms(50);

    clear_all_led();
    _delay_ms(100);

    set_white_led();
    _delay_ms(100);
    clear_white_led();

    set_green_led();
    _delay_ms(100);
    clear_green_led();

    set_yellow_led();
    _delay_ms(100);
    clear_yellow_led();

    set_red_led();
    _delay_ms(100);
    clear_red_led();
}

void applyDeltaT(float delta_T)
{
    if (delta_T <= 2.0)
    {
        clear_all_led();
        set_white_led();
    }
    else if (delta_T <= 7.0)
    {
        clear_all_led();
        set_white_led();
        set_green_led();
    }
    else if (delta_T <= 12.0)
    {
        clear_all_led();
        set_white_led();
        set_green_led();
        set_yellow_led();
    }
    else if (delta_T >= 15.0)
    {
        set_all_led();
    }
}