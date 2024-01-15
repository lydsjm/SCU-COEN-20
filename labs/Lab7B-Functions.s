.syntax unified
.cpu cortex-m4
.text


//.set BitBanding, 1    // Enable Bit-banding mode


.global PutBit
.thumb_func
.align


PutBit:
.ifdef BitBanding
 // Bit-banding implementation
	  SUB      R0, R0, 0x20000000      // Subtract base address to calculate the bit-band region
	  LSL      R0, R0, 5               // Shift left by 5 to obtain the address within the bit-band region
	  ADD      R0, R0, R1, LSL 2       // Calculate the final bit-band address using the bit number
	  ADD      R0, R0, 0x22000000      // Add back the base address of the alias region
	  STR      R2, [R0]                // Store the value of R2 at the calculated address
	  BX       LR                      // Return


.else


	  LSR     R3, R1, 3                // Divide the bit number by 8 to get the byte offset
	  ADD     R0, R0, R3               // Add the byte offset to the base address to get the target byte address
	  AND     R1, R1, (1<<3)-1         // Mask the bit number to extract the bit position within the byte
	  LSL     R2, R2, R1               // Shift the value in R2 to the correct bit position
	  MOV     R3, 1                    // Load the value 1 into R3
	  LSL     R1, R3, R1               // Calculate the bitmask with a single set bit at the correct position


	  LDRB    R3, [R0]                 // Load the byte value at the target address
	  BIC     R3, R3, R1               // Clear the target bit in the byte
	  ORR     R3, R3, R2               // Set the target bit to the desired value
	  STRB    R3, [R0]                 // Store the modified byte back to memory
	  BX      LR                       // Return


.endif


.global GetBit
.thumb_func
.align


GetBit:
.ifdef BitBanding
 // Bit-banding implementation
	  SUB      R0, R0, 0x20000000      // Subtract base address to calculate the bit-band region
	  LSL      R0, R0, 5               // Shift left by 5 to obtain the address within the bit-band region
	  ADD      R0, R0, R1, LSL 2       // Calculate the final bit-band address using the bit number
	  ADD      R0, R0, 0x22000000      // Add back the base address of the alias region
	  LDR      R0, [R0]                // Load the value at the calculated address into R0
	  BX       LR                      // Return


.else


	  LSR     R3, R1, 3                 // Divide the bit number by 8 to get the byte offset
	  ADD     R0, R0, R3               // Add the byte offset to the base address to get the target byte address
	  AND     R1, R1, 7                // Mask the bit number to extract the bit position within the byte


	  LDRB    R3, [R0]                 // Load the byte value at the target address
	  LSR     R3, R3, R1               // Shift the target bit to the least significant bit position
	  AND     R0, R3, 1                // Mask the result to obtain the desired bit value
	  BX LR                            // Return
.endif

.end












