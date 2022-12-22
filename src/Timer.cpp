#include "Timer.h"

Timer::Timer() {

}

void Timer::initTimer() {
    // configure pins
    PORTB_DIR |= (1 << PIN0_bp) | (1 << PIN1_bp);

    // configure timer
    TCA0.SINGLE.PER = 640;
    TCA0.SINGLE.CMP0 = 320;
    TCA0.SINGLE.CMP1 = 480;

    TCA0.SINGLE.CTRLA &= 0b11110001 | (TCA_SINGLE_CLKSEL_DIV1_gc << 1);

    TCA0.SINGLE.CTRLB |= (1 << TCA_SINGLE_CMP0EN_bp) | (1 << TCA_SINGLE_CMP1_bp);
    TCA0.SINGLE.CTRLB |= TCA_SINGLE_WGMODE_SINGLESLOPE_gc;

    TCA0.SINGLE.CTRLA |= (1 << TCA_SINGLE_ENABLE_bp);
}