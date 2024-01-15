/*
    This code was written to support the book, "ARM Assembly for Embedded Applications",
    by Daniel W. Lewis. Permission is granted to freely share this software provided
    that this notice is not removed. This software is intended to be used with a run-time
    library adapted by the author from the STM Cube Library for the 32F429IDISCOVERY 
    board and available for download from http://www.engr.scu.edu/~dlewis/book3.
*/
        .syntax         unified
        .cpu            cortex-m4
        .text

// ----------------------------------------------------------
// typedef uint32_t            TIMER ;
// typedef enum { FALSE = 0, TRUE = 1 } BOOL ;
// #define CPU_CLOCK_MHZ       168
// ----------------------------------------------------------

// ----------------------------------------------------------
// TIMER GetTimeout(int msec) ;
// ----------------------------------------------------------

        .global         GetTimeout
        .thumb_func
        .align
GetTimeout:   // R0 = msec
        PUSH            {R4,LR}
        LDR             R4,=168000
        MUL             R4,R0,R4            // R4 (cycles) = 1000 * msec * CPU_CLOCK_MHZ
        BL              GetClockCycleCount
        ADD             R0,R0,R4            // R0 = GetClockCycleCount() + cycles
        POP             {R4,PC}

// ----------------------------------------------------------
// BOOL Timeout(TIMER *timer, unsigned msec) ;
// ----------------------------------------------------------

        .global        Timeout
        .thumb_func
        .align
Timeout:    // R0 = *timer, R1 = msec
        PUSH            {R4,R5,R6,LR}
        MOV             R4,R0               // R4 = timer
        MOV             R5,R1               // R5 = msec
        BL              GetClockCycleCount  // R0 = GetClockCycleCount()
        LDR             R1,[R4]             // R1 = *timer
        SUBS            R0,R0,R1            // R0 = (GetClockCycleCount() - *timer
        ITEE            GT
        LDRGT           R6,=1               // R6 (timeout) = (int) (GetClockCycleCount() - *timer) > 0
        LDRLE           R6,=0
        BLE             False               // if (timeout)
        MOV             R0,R5               //      R0 = msec
        BL              GetTimeout          //      R0 = GetTimeout(msec)
        STR             R0,[R4]             //      *timer = GetTimeout(msec)
False:  MOV             R0,R6               // R0 = timeout
        POP             {R4,R5,R6,PC}       // return

        .end


