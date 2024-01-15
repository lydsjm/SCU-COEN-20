        .syntax     unified
        .cpu        cortex-m4
        .text


//void __attribute__((weak)) OffBy0(void *dst, const void *src); 

        .global     OffBy0
        .align
        .thumb_func
OffBy0:    
        .rept       250         //repeats 250 times
        LDR         R2,[R1],4   //Loads R2 with the content of R1, then increments 4 bytes
        STR         R2,[R0],4   //Stores R2 into R0, then increments 4 bytes
        .endr                   //end 

        BX          LR          //returns
    

//void __attribute__((weak)) OffBy1(void *dst, const void *src);

        .global		OffBy1
        .align
        .thumb_func
OffBy1:
        .rept       3           //repeats 3 times
        LDRB        R2,[R1],1   //loads R2 with contents of R1 in 8-bit form then increments 1 byte
        STR         R2,[R0],1   //stores R2 into R0 then increments 1 byte
        .endr                   //end

        .rept       250         //repeats 250 times
        LDR         R2,[R1],4   //loads R2 with content of R1 then increments 4 bytes
        STR         R2,[R0],4   //stores R2 into R0 then increments 4 bytes
        .endr                   //end
        
        LDRB        R2,[R1],1   //loads R2 with contents of R1 in 8-bit form then increments 1 byte
        STR         R2,[R0],1   //stores R2 into R0 then increments 1 byte

        BX          LR          //returns


//void __attribute__((weak)) OffBy2(void *dst, const void *src);

        .global		OffBy2
        .align
        .thumb_func
OffBy2:
        .rept       2           //repeats 2 times
        LDRB        R2,[R1],1   //loads R2 with contents of R1 in 8-bit form then increments 1 byte
        STR         R2,[R0],1   //stores R2 into R0 then increments 1 byte
        .endr                   //end 

        .rept       250         //repeats 250 times
        LDR         R2,[R1],4   //Loads R2 with content of R1 then increments 4 bytes
        STR         R2,[R0],4   //stores R2 into R0 then increments 4 bytes
        .endr                   //end 
        
        .rept       2           //repeats 2 times
        LDRB        R2,[R1],1   //loads R2 with contents of R1 in 8-bit form then increments 1 byte
        STR         R2,[R0],1   //stores R2 into R0 then increments 1 byte
        .endr                   //end 

        BX          LR          //returns


//void __attribute__((weak)) OffBy3(void *dst, const void *src);

        .global		OffBy3
        .align
        .thumb_func
OffBy3:
        LDRB        R2,[R1],1   //loads R2 with contents of R1 in 8-bit form then increments 1 byte
        STR         R2,[R0],1   //stores R2 into R0 then increments 1 byte

        .rept       250         //repeats 250 times
        LDR         R2,[R1],4   //loads R2 with content of R1 then increments 4 bytes
        STR         R2,[R0],4   //stores R2 into R0 then increments 4 bytes
        .endr                   //end 
        
        .rept       3           //repeats 3 times
        LDRB        R2,[R1],1   //loads R2 with contents of R1 in 8-bit form then increments 1 byte
        STR         R2,[R0],1   //stores R2 into R0 then increments 1 byte
        .endr                   //end 

        BX          LR          //returns
.end