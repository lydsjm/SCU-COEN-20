#include <stdint.h>


static const char digits[] = "0123456789ABCDEF";


void Bits2HexString(uint8_t bits, char string[]) { // radix 16
	unsigned int nibble1 = bits >> 4; // shift 4 bits to the right to get the first half of the hex number
	unsigned int nibble2 = bits & 0b00001111; //bitwise & to get second half of the hex number
	//store in string
	string[0] = digits[nibble1];
	string[1] = digits[nibble2];
	string[2] = '\0';
}


void Bits2OctalString(uint8_t bits, char string[]) { // radix 8
	unsigned int nibble1 = bits >> 6;  //shift 6 to the right to get first two bits for first part of octal grouping
	unsigned int nibble2 = (bits & 0b00111000) >> 3; // shift 3 to the right to get the second octal grouping
	unsigned int nibble3 = bits & 0b00000111; //use bitwise & to shift third octal grouping
	//store in string
	string[0] = digits[nibble1];
	string[1] = digits[nibble2];
	string[2] = digits[nibble3];
	string[3] = '\0';
}




void Bits2UnsignedString(uint8_t bits, char string[]) { // radix 10
	unsigned int dc100 = bits / 100; //100 place digit
	bits = bits - (dc100 * 100); //erase 100 place digit
	unsigned int dc10 = bits / 10; //10 place digit
	bits = bits - (dc10 * 10); //erase 10 place digit
	unsigned int dc1 = bits; //1 place digit


	//store in string
	string[0] = digits[dc100];
	string[1] = digits[dc10];
	string[2] = digits[dc1];
	string[3] = '\0';
}




void Bits2SignMagString(uint8_t bits, char string[]) {
	int sign_bit = bits & 0b10000000;
	if (sign_bit == 0) {
		string[0] = '+';
		Bits2UnsignedString(bits, string + 1); // convert the unsinged bits to a string and store after positive sign
	}
	else {
		string[0] = '-';
		Bits2UnsignedString(bits & 0b01111111, string + 1); // convert the unsigned bits without the sign bit to a string and store
	}
}


void Bits2TwosCompString(uint8_t bits, char string[]) {
	int sign_bit = bits & 0b10000000;
	if (sign_bit == 0) {
		Bits2UnsignedString(bits, string); //convert unsinged to a string and store in array
	}
	else {
		uint8_t magnitude = ~bits + 1;
		string[0] = '-';
		Bits2UnsignedString(magnitude, string + 1); // convert the unsigned magnituted to a string and store it after the negative sign
	}
}

