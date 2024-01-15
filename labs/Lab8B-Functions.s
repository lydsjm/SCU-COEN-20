.syntax unified
.cpu cortex-m4
.text

.global Bills
.thumb_func
Bills:
    LDR     R2, =3435973837     // (2^32)/20
    UMULL   R3, R2, R2, R0     // Calculate quotient for bills
    LSRS    R3, R2, 4          // Shift right to get the quotient

    STR     R3, [R1], 4        // Store the calculated quotient in memory
    LSL     R3, R3, 2          // Multiply the quotient by 4 (R3 * 4)
    ADD     R3, R3, R3, LSL 2  // Multiply the quotient by 5 (R3 * 20)
    SUB     R0, R0, R3         // Find the remaining dollars/coins

    B       Common               // Branch to the Common function

.global Coins
.thumb_func
Coins:
    LDR     R2, =1374389535     // (2^32)/25
    UMULL   R3, R2, R2, R0     // Calculate quotient for coins
    LSRS    R3, R2, 3          // Shift right to get the quotient

    STR     R3, [R1], 4        // Store the calculated quotient in memory
    ADD     R3, R3, R3, LSL 2  // Multiply the quotient by 5 (R3 * 5)
    ADD     R3, R3, R3, LSL 2  // Multiply the quotient by 5 (R3 * 25)
    SUB     R0, R0, R3         // Find the remaining dollars/coins

    B       Common               // Branch to the Common function

Common:
    LDR     R3, =3435973837     // (2^32)/10
    UMULL   R3, R2, R2, R0     // Calculate quotient for 10 units
    LSRS    R3, R2, 3          // Shift right to get the quotient

    STR     R3, [R1], 4        // Store the calculated quotient in memory
    LSL     R3, R3, 1          // Multiply the quotient by 2 (R3 * 2)
    ADD     R3, R3, R3, LSL 2  // Multiply the quotient by 5 (R3 * 10)
    SUB     R0, R0, R3         // Find the remaining dollars/coins

    LDR     R3, =3435973837     // (2^32)/5
    UMULL   R3, R2, R2, R0     // Calculate quotient for 5 units
    LSRS    R3, R2, 2          // Shift right to get the quotient

    STR     R3, [R1], 4        // Store the calculated quotient in memory
    

    SUB     R0, R0, R3         // Find the remaining dollars

    STR     R0, [R1]           // Store the remaining amount in memory
    BX LR                      // Return from the function

.end



