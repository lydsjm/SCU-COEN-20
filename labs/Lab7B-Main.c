/*
    This code was written to support the book, "ARM Assembly for Embedded Applications",
    by Daniel W. Lewis. Permission is granted to freely share this software provided
    that this notice is not removed. This software is intended to be used with a run-time
    library adapted by the author from the STM Cube Library for the 32F429IDISCOVERY 
    board and available for download from http://www.engr.scu.edu/~dlewis/book3.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "library.h"
#include "graphics.h"

#pragma GCC push_options
#pragma GCC optimize ("O0")

void __attribute__((weak)) PutBit(void *bits, uint32_t index, uint32_t bit)
    {
#ifdef  BITWISE
    uint32_t  which = index >> 5 ;
    uint32_t  shift = index & 0x1F ;
    uint32_t *pword = ((uint32_t *) bits) + which ;
    *pword &= ~(  1 << shift) ;
    *pword |=  (bit << shift) ;
#else   // BITBANDING
    uint32_t  which  = index >> 5 ;
    uint32_t  bitpos = index & 0x1F ;
    uint32_t *bbadrs = (uint32_t *) (0x22000000 + (((uint32_t) bits + 4*which - 0x20000000) << 5) + (bitpos << 2)) ;
    *bbadrs = bit ;
#endif
    }

// Functions to be implemented in ARM assembly
uint32_t __attribute__((weak)) GetBit(void *bits, uint32_t index)
    {
#ifdef  BITWISE
    uint32_t word = index >> 5 ;
    uint32_t shft = index & 0x1F ;
    return (((uint32_t *) bits)[word] >> shft) & 1 ;
#else   // BITBANDING
    uint32_t word = index >> 5 ;
    uint32_t bitpos = index & 0x1F ;
    uint32_t *bbadrs = (uint32_t *) (0x22000000 + 32*((uint32_t) bits + 4*word - 0x20000000) + 4*bitpos) ;
    return *bbadrs ;
#endif
    }

#pragma GCC pop_options

typedef enum {FALSE = 0, TRUE = 1} BOOL ;

typedef struct
    {
    unsigned            ttl_cycles ;
    unsigned            ttl_births ;
    unsigned            ttl_deaths ;
    unsigned            ttl_alive ;
    unsigned            getCalls ;
    unsigned            putCalls ;
    unsigned            getCycles ;
    unsigned            putCycles ;
    } STATISTICS ;

typedef struct _tFont
    {
    const uint8_t *     table ;
    const uint16_t      Width ;
    const uint16_t      Height ;
    } sFONT;

// Functions private to the main program
static void             Delay(unsigned clocks) ;
static void             DisplayReport(STATISTICS *stats) ;
static BOOL             Evolve(void) ;
static int              FailureReport(int row, char *fmt, unsigned index, unsigned value) ;
static void             InitializeGame(unsigned getCycles, unsigned putCycles) ;
static void             LEDs(int grn_on, int red_on) ;
static int              ReportLine(int row, sFONT *font, char *format, ...) ;
static int              ReportHeader(int row, sFONT *font, char *title, int lines) ;
static int              SanityChecksOK(void) ;
static void             SetFontSize(sFONT *font) ;

#define SIZE            8

#define X_MIN           0
#define Y_MIN           48

#define XSTAT           5
#define YSTAT           (YPIXELS-30)

#define COLS            (XPIXELS/SIZE)
#define ROWS            ((YPIXELS-Y_MIN-30)/SIZE)

#define CELLS           (ROWS*COLS)
#define WORDS           (CELLS + 31)/32

#define TXT_HEIGHT      15
#define TXT_WIDTH       7

#define INDEX(row, col) ((row)*COLS+(col))

#define REPORT_XPOS     20
#define REPORT_YPOS     65
#define REPORT_WIDTH    18

static uint32_t         storage[2][WORDS] ;
static uint32_t *       bits = storage[0] ;
static uint32_t *       next = storage[1] ;

static STATISTICS   stats ;

extern sFONT Font8 ;
extern sFONT Font12 ;   // Smaller font used in footer
extern sFONT Font16 ;   // Larger font used in header
extern sFONT Font20 ;
extern sFONT Font24 ;   // Largest font used for game

int main()
    {
    unsigned strt, stop, ovhd, getCycles, putCycles  ;

    InitializeHardware(HEADER, "Lab 7B: Conway's Game of Life") ;

    if (!SanityChecksOK()) return 255 ;

    SetFontSize(&Font12) ;
    memset(&stats, 0, sizeof(stats)) ;

    strt = GetClockCycleCount() ;
    stop = GetClockCycleCount() ;
    ovhd = stop - strt ;

    strt = GetClockCycleCount() ;
    for (int k = 0; k < 1000; k++)
        {
        GetBit(storage, 0) ;
        }
    stop = GetClockCycleCount() ;
    getCycles = (stop - strt)/1000 - ovhd ;

    strt = GetClockCycleCount() ;
    for (int k = 0; k < 1000; k++)
        {
        PutBit(storage, 0, 0) ;
        }
    stop = GetClockCycleCount() ;
    putCycles = (stop - strt)/1000 - ovhd ;

    while (1)
        {
        BOOL stabilized = FALSE ;

        InitializeGame(getCycles, putCycles) ;
        while (!stabilized)
            {
            uint32_t *temp ;

            stabilized = Evolve();

            temp = bits ;
            bits = next ;
            next = temp ;
            }

        DisplayReport(&stats) ;
        WaitForPushButton() ;
        }

    return 0 ;
    }

static BOOL Evolve(void)
    {
    static unsigned prev_pop = 0 ;
    static unsigned cycles = 0 ;
    unsigned born, died  ;
    char text[100] ;
    BOOL stabilized ;

    cycles++ ;
    born = died = 0 ;
    for (int row = 0; row < ROWS; row++)
        {
        for (int col = 0; col < COLS; col++)
            {
            int n, alive, me ;

            n = me = 0 ;
            for (int r = row - 1; r <= row + 1; r++)
                {
                if (r < 0 || r == ROWS) continue ;
                for (int c = col - 1; c <= col + 1; c++)
                    {
                    int bit ;

                    if (c < 0 || c == COLS) continue ;
                    stats.getCalls++ ;
                    bit = GetBit(bits, INDEX(r, c)) ;
                    if (r == row && c == col) me = bit ;
                    else if (bit != 0) n++ ;
                    }
                }

            if (me == 1)
                {
                alive = (n==2) || (n==3) ;
                if (!alive) died++ ;
                SetColor(alive ? COLOR_BLACK : COLOR_RED) ;
                }
            else
                {
                alive = (n == 3) ;
                if (alive) born++ ;
                SetColor(alive ? COLOR_GREEN : COLOR_WHITE) ;
                }

            FillRect(SIZE*col + X_MIN, SIZE*row + Y_MIN, SIZE, SIZE) ;
            PutBit(next, INDEX(row, col), alive) ;
            stats.putCalls++ ;
            }
        }

    stats.ttl_cycles++ ;
    stats.ttl_births += born ;
    stats.ttl_deaths += died ;
    stats.ttl_alive  += born - died ;

    if (stats.ttl_alive != prev_pop)
        {
        prev_pop = stats.ttl_alive ;
        cycles = 0 ;
        stabilized = FALSE ;
        }
    else stabilized = cycles == 10 ;

    sprintf(text, "Births:%-4d Deaths:%-4d Alive:%-4d", born, died, stats.ttl_alive) ;
    SetColor(COLOR_BLACK) ;
    DisplayStringAt(XSTAT, YSTAT, text) ;

    for (int delay = 0; delay < 1000; delay++)
        {
        // Check for user abort
        if (PushButtonPressed())
            {
            WaitForPushButton() ;
            return TRUE ;
            }
        Delay(8000) ;
        }

    return stabilized ;
    }

static void InitializeGame(unsigned getCycles, unsigned putCycles)
    {
    stats.getCycles = getCycles ;
    stats.putCycles = putCycles ;

    for (int row = 0; row < ROWS; row++)
        {
        for (int col = 0; col < COLS; col++)
            {
            int alive = GetRandomNumber() < RAND_MAX / 2 ;
            if (alive) stats.ttl_alive++ ;
            PutBit(bits, INDEX(row, col), alive) ;
            }
        }
    }

static void Delay(unsigned clocks)
    {
    uint32_t timeout = GetClockCycleCount() + clocks ;
    while ((int) (timeout - GetClockCycleCount()) > 0) ;
    }

static int FailureReport(int row, char *fmt, unsigned index, unsigned value)
    {
    unsigned byte, word ;
    char text[100] ;

    SetForeground(COLOR_WHITE) ;
    SetBackground(COLOR_RED) ;

    sprintf(text, fmt, index, value) ;
    DisplayStringAt(30, row, text) ;
    row += TXT_HEIGHT ;

    SetForeground(COLOR_BLACK) ;
    SetBackground(COLOR_WHITE) ;

    DisplayStringAt(30, row, "      Index     In Memory") ;
    row += TXT_HEIGHT ;

    byte = ((uint8_t *) storage)[index/8] ;
    index /= 8 ;
    sprintf(text, "%-6s%5d          0x%02X", "Byte", index, byte) ;
    DisplayStringAt(30, row, text) ;
    row += TXT_HEIGHT ;

    index /= 4 ;
    word = ((uint32_t *) storage)[index] ;
    sprintf(text, "%-6s%5d    0x%08X", "Word", index, word) ;
    DisplayStringAt(30, row, text) ;

    return row + TXT_HEIGHT + 4 ;
    }

static int SanityChecksOK(void)
    {
    uint32_t index, word, shift , bugs, bit, row ;

    for (int i = 0; i < WORDS; i++) bits[i] = 0 ;

    bugs = 0 ;
    row = 50 ;

    do index = GetRandomNumber() % CELLS ; while (index < 32) ;
    word = index / 32 ;
    shift  = index % 32 ;
    PutBit(bits, index, 1) ;
    if (bits[word] != (1 << shift))
        {
        row = FailureReport(row, "Bad PutBit(&, %d, %d)    ", index, 1) ;
        bugs++ ;
        }
    bits[word] = 0 ;

    do index = GetRandomNumber() % CELLS ; while (index < 32) ;
    word = index / 32 ;
    shift  = index % 32 ;
    bits[word] = 0xFFFFFFFF ;
    PutBit(bits, index, 0) ;
    if (bits[word] != ~(1 << shift))
        {
        row = FailureReport(row, "Bad PutBit(&, %d, %d)    ", index, 0) ;
        bugs++ ;
        }
    bits[word] = 0 ;

    do index = GetRandomNumber() % CELLS ; while (index < 32) ;
    word = index / 32 ;
    shift  = index % 32 ;
    bits[word] = 1 << shift ;
    bit = GetBit(bits, index) ;
    if (bit != 1)
        {
        row = FailureReport(row, "Bad GetBit(*, %d)=>%d    ", index, bit) ;
        bugs++ ;
        }
    bits[word] = 0 ;

    do index = GetRandomNumber() % CELLS ; while (index < 32) ;
    word = index / 32 ;
    shift  = index % 32 ;
    bits[word] = ~(1 << shift) ;
    bit = GetBit(bits, index) ;
    if (bit != 0)
        {
        FailureReport(row, "Bad GetBit(*, %d)=>%d    ", index, bit) ;
        bugs++ ;
        }
    bits[word] = 0 ;

    LEDs(!bugs, bugs) ;
    return bugs == 0 ;
    }

static void LEDs(int grn_on, int red_on)
    {
    static uint32_t * const pGPIOG_MODER    = (uint32_t *) 0x40021800 ;
    static uint32_t * const pGPIOG_ODR      = (uint32_t *) 0x40021814 ;
    
    *pGPIOG_MODER |= (1 << 28) | (1 << 26) ;    // output mode
    *pGPIOG_ODR &= ~(3 << 13) ;         // both off
    *pGPIOG_ODR |= (grn_on ? 1 : 0) << 13 ;
    *pGPIOG_ODR |= (red_on ? 1 : 0) << 14 ;
    }

static int ReportHeader(int row, sFONT *font, char *title, int lines)
    {
    int height = 1 + (++lines * font->Height) + font->Height/4 ;
    int width  = 1 + REPORT_WIDTH*font->Width ;
    DrawRect(REPORT_XPOS - 1, row - 1, width, height) ;
    FillRect(REPORT_XPOS, row, REPORT_WIDTH*font->Width, font->Height) ;
    SetForeground(COLOR_WHITE) ;
    SetBackground(COLOR_BLACK) ;
    DisplayStringAt(REPORT_XPOS + font->Width, row, title) ;
    return row + font->Height + font->Height/4 ;
    }

static int ReportLine(int row, sFONT *font, char *format, ...)
    {
    char text[100] ;
    va_list args ;

    SetForeground(COLOR_BLACK) ;
    SetBackground(COLOR_WHITE) ;
    va_start(args, format) ;
    vsnprintf(text, sizeof(text), format, args);
    DisplayStringAt(REPORT_XPOS + font->Width, row, text) ;
    va_end(args) ;
    return row + font->Height ;
    }

static void DisplayReport(STATISTICS *stats)
    {
    sFONT *font = &Font16 ;
    int row ;

    ClearDisplay() ;
    SetFontSize(font) ;

    row = REPORT_YPOS ;

    row = ReportHeader(row, font, "GAME STATISTICS", 4) ;
    row = ReportLine(row, font, "Cycles:%u", stats->ttl_cycles) ;
    row = ReportLine(row, font, "Births:%u", stats->ttl_births) ;
    row = ReportLine(row, font, "Deaths:%u", stats->ttl_deaths) ;
    row = ReportLine(row, font, " Alive:%u", stats->ttl_alive) ;

    row += 15 ;

    row = ReportHeader(row, font, "FUNCTION CALLS", 2) ;
    row = ReportLine(row, font, " GetBit:%u", stats->getCalls) ;
    row = ReportLine(row, font, " PutBit:%u", stats->putCalls) ;

    row += 15 ;

    row = ReportHeader(row, font, "CLOCK CYCLES", 2) ;
    row = ReportLine(row, font, " GetBit:%u", stats->getCycles) ;
    row = ReportLine(row, font, " PutBit:%u", stats->putCycles) ;
    }

static void SetFontSize(sFONT *font)
    {
    extern void BSP_LCD_SetFont(sFONT *) ;
    BSP_LCD_SetFont(font) ;
    }

