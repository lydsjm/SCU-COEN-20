    .syntax     unified
    .cpu        cortex-m4
    .text

/* void Integrate(void) */
    .global     Integrate
    .thumb_func
    .align
//S4 = prev, S16 = dx, S17 = r, S18 = v, S19 = a, S20 = x, R4 = n
Integrate:
    PUSH        {R4, LR}           // Save R4 and LR on the stack
    VPUSH       {S16, S17, S18, S19, S20, S21}   // Save floating-point registers S16-S21 on the stack
    BL          DeltaX              // Call DeltaX() function to calculate dx
    VMOV        S16, S0             // Store dx value in S16
    VMOV        S0, 1.0             // Initialize r (S0) to 1.0
    VMOV        S17, S0             // Store r value in S17
    VSUB.F32    S1, S1, S1          // Initialize v (S1) to 0.0
    VMOV        S18, S1             // Store v value in S18
    VSUB.F32    S2, S2, S2          // Initialize a (S2) to 0.0
    VMOV        S19, S2             // Store a value in S19
    VMOV        S3, 1.0             // Initialize x (S3) to 1.0
    VMOV        S20, S3             // Store x value in S20
    LDR         R0,= 0              // Initialize n (R0) to 0
    MOV         R4, R0              // Store n value in R4

While:
    MOV         R0, R4              // Move n value to R0
    VMOV        S0, S17             // Move r value to S0
    VMOV        S1, S18             // Move v value to S1
    VMOV        S2, S19             // Move a value to S2
    BL          UpdateDisplay       // Call UpdateDisplay(n, r, v, a) function
    VMOV        S4, S18             // Store current v value in S4 as prev
    VMOV        S5, 1.0             // Initialize S5 to 1.0
    VDIV.F32    S6, S5, S20         // Calculate 1/x and store in S6
    VADD.F32    S7, S20, S16        // Calculate x + dx and store in S7
    VDIV.F32    S8, S5, S7          // Calculate 1/(x + dx) and store in S8
    VADD.F32    S9, S6, S8          // Calculate (1/x + 1/(x + dx)) and store in S9
    VMOV        S10, 2.0            // Initialize S10 to 2.0
    VDIV.F32    S17, S9, S10        // Calculate (1/x + 1/(x + dx))/2 and store in S17
    VMLA.F32    S18, S17, S17       // Calculate v + r*r and store in S18
    VADD.F32    S19, S19, S17       // Calculate a + r and store in S19
    ADD         R4, R4, 1           // Increment n by 1
    VADD.F32    S20, S20, S16       // Calculate x + dx and store in S20
    VCMP.F32    S18, S4             // Compare v and prev
    VMRS        APSR_nzcv, FPSCR    // Move FPSCR register to APSR
    BEQ         Done                // Exit loop if v == prev
    B           While               // Continue looping

Done:
    VPOP        {S16, S17, S18, S19, S20, S21}   // Restore floating-point registers S16-S21
    POP         {R4, LR}           // Restore R4 and LR from the stack
    BX          LR                 // Return

/* End of file */
    .end
