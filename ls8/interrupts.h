

//== Interrupts ===============================================================
/*
    This modules implements interrupts for use in the LS8 CPU emulator. All
    three functions should be invoked every emulated clock tick.

    timer_advance generates timer interrupts ever second. It accepts the
    following arguments:
        *cpu: The CPU to generate interrupts on
        *time_current: A struct returned by clock_gettime()
*/

//-- Dependencies ------------------------------------
#include <time.h>
#include "cpu.h"

//-- Generate a time interrupt every second ----------
void timer_advance(struct cpu *cpu, struct timespec *time_current);

//-- Generate keyboard interrupts --------------------
void check_keyboard(struct cpu *cpu);

//-- Handle Interrupts -------------------------------
void handle_interrupts(struct cpu *cpu);
