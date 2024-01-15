.syntax unified
.cpu cortex-m4
.text

.global FloodFill
.thumb_func
.align

// R0 = x, R1 = y, R2 = old_clr, R3 = new_clr
FloodFill:
			PUSH		{R4-R8, LR}     // Save registers R4-R8 and LR onto the stack

			MOV			R5, R0          // Move x (R0) to R5 
			MOV 		R6, R1          // Move y (R1) to R6 
			MOV 		R7, R2          // Move old_clr (R2) to R7 
			MOV 		R8, R3          // Move new_clr (R3) to R8 

			BL 			OutOfBounds		// Call OutOfBounds to check if (x, y) is out of bounds
			CBNZ 		R0, FloodExit   // If out of bounds, branch to FloodExit

			MOV 		R0, R5          // Move x (R5) back to R0 
			MOV 		R1, R6          // Move y (R6) back to R1 
			BL 			PixelAdrs       // Call PixelAdrs to get the address of the pixel
			LDR 		R4, [R0]        // Load the value of the pixel into R4
			CMP			R4, R7          // Compare the value with old_clr (R7)
			BNE 		FloodExit       // If not equal, branch to FloodExit

			STR 		R8, [R4]        // Store new_clr (R8) into the pixel address

			SUB 		R0, R5, 1       // Calculate x - 1
			MOV 		R1, R6          // Move y (R6) to R1 
			MOV 		R2, R7          // Move old_clr (R7) to R2 
			MOV 		R3, R8          // Move new_clr (R8) to R3 
			BL 			FloodFill       // Recursive call to FloodFill with (x - 1, y, old_clr, new_clr)


			ADD 		R0, R5, 1       // Calculate x + 1
			MOV 		R1, R6          // Move y (R6) to R1 
			MOV 		R2, R7          // Move old_clr (R7) to R2 
			MOV 		R3, R8          // Move new_clr (R8) to R3 
			BL 			FloodFill       // Recursive call to FloodFill with (x + 1, y, old_clr, new_clr)


			MOV 		R0, R5          // Move x (R5) to R0 
			SUB 		R1, R6, 1       // Calculate y - 1
			MOV 		R2, R7          // Move old_clr (R7) to R2 
			MOV 		R3, R8          // Move new_clr (R8) to R3 
			BL 			FloodFill       // Recursive call to FloodFill with (x, y - 1, old_clr, new_clr)


			MOV 		R0, R5          // Move x (R5) to R0 
			ADD 		R1, R6, 1       // Calculate y + 1
			MOV 		R2, R7          // Move old_clr (R7) to R2 
			MOV 		R3, R8			// Move new_clr (R8) to R3 
			BL 			FloodFill		// Recursive call to FloodFill with (x, y + 1, old_clr, new_clr)

FloodExit:
			POP		{R4-R8, PC}			// Pop registers R4-R8









