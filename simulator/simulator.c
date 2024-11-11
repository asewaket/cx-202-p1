#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MEM_SIZE 9  // maximum number of instructions is 9
#define REG_SIZE 4  // size of each register (4-bit)

// Register arrays
unsigned char registers[3][REG_SIZE] = {0};  // [0]: RA, [1]: RB, [2]: RO
unsigned char programCounter = 0; // initiallize PC
unsigned char memory[MEM_SIZE] = {
    0x50,  // Memory[0] = 0x50 (Binary: 01010000)
    0x61,  // Memory[1] = 0x61 (Binary: 01100001)
    0x40,  // Memory[2] = 0x40 (Binary: 01000000)
    0x11,  // Memory[3] = 0x11 (Binary: 00010001)
    0x70,  // Memory[4] = 0x70 (Binary: 01110000)
    0x01,  // Memory[5] = 0x01 (Binary: 00000001)
    0x23,  // Memory[6] = 0x23 (Binary: 00100011)
    0x32,  // Memory[7] = 0x32 (Binary: 00110010)
    0x82,  // Memory[8] = 0x82 (Binary: 10000010)
};

unsigned char carryFlag = 0;

// Function prototypes for hardware components
int adder4(int a, int b, bool *carryOut);
int subtractor4(int a, int b, bool *borrowOut);
void loadProgram(const char *filename);
void decode_and_execute(unsigned char instruction);
void executeInstructions(int stepMode);

// Utility functions for accessing registers
unsigned char readRegister(unsigned char regIndex);
void writeRegister(unsigned char regIndex, unsigned char value);

int main(int argc, char *argv[]) {
    if (argc != 2) {					//check if command-line arguments is NOT 2 
        printf("Usage: %s <binary file>\n", argv[0]);	//error message
        return 1;
    }
    
    printf("Program Counter = %d\n", programCounter);	//debug print statement

    //loadProgram(argv[1]);

    char mode;
    printf("Select mode: (R)un continuously or (S)tep-by-step: ");
    scanf(" %c", &mode);

    if (mode == 'S') {
        executeInstructions(1);  // Step-by-step mode
    } else if (mode == 'R') {
        executeInstructions(0);  // Continuous mode
    } else {
        printf("Invalid selection. Exiting program.\n");
        return 1;
    }

    return 0;
}

void loadProgram(const char *filename) {	//no value returned
    FILE *file = fopen(filename, "rb");		//open input file
    if (!file) {				
        perror("Error opening file");
        exit(1);
    }

    size_t bytesRead = fread(memory, sizeof(unsigned char), MEM_SIZE, file);	//read MEM_SIZE bytes into memory[], return number of elements stored in bytesRead
    fclose(file);	

    printf("Loaded %zu bytes into memory\n", bytesRead);	//print number of bytes size_t type to confirm how many bytes loaded into memory
    printf("Memory contents:\n");
    for(int i = 0; i < bytesRead; i++) {	//print memory content at each index in hex format
        printf("Memory[%d] = 0x%02X (Binary: ", i, memory[i]);
        for(int j = 7; j >= 0; j--) {	//binary representation of each byte
            printf("%d", (memory[i] >> j) & 1);	//extract j-th bit of memory[i] to visulize content of memory location, debugging step 
        }
        printf(")\n");
    }
}

unsigned char readRegister(unsigned char regIndex) {	//reads and returns values in RA, RB, and RO, unsigned char is 8-bit value , regIndex for which register to read: RA => 0, RB => 1, and RO => 2
    unsigned char value = 0;	//build final 4-bit value from specified register
    for (int i = 0; i < REG_SIZE; i++) {	//goes over each bit position in the 4-bit register 
        value |= (registers[regIndex][i] << i);	//extract i-th bit of register specified by regIndex, and bitwise OR to set i-th bit in value without altering the other bits
    }
    return value & 0x0F;  // Ensure 4-bit output using bitmask, bitwise AND ensures upper 4 bits pf value are set to 0, so output only lower 4 bits
}

void writeRegister(unsigned char regIndex, unsigned char value) {	//writes given 4-bit value to a register by breaking down the vlaue and storing each bit in the apropriate position withhin the register array
    for (int i = 0; i < REG_SIZE; i++) {	
        registers[regIndex][i] = (value >> i) & 0x01;	//access i-th bit of register specified by regIndex, value >>i shift value to the right by i 
        // & 0x01 to ensure only LSB is retained  
    }
}

int adder4(int a, int b, bool *carryOut) {	//simulates a 4-bit adder, which performs the addition of two 4-bit integers and handles overflow by using a carry-out flag
    int result = a + b;
    *carryOut = (result > 15);  // Check for overflow in 4-bit addition
    return result & 0x0F;       // Ensure 4-bit output
}

int subtractor4(int a, int b, bool *borrowOut) {	//simulates a 4-bit subtractor, performing the subtraction of two 4-bit integers and managing underflow using a borrow-out flag
    int result = a - b;
    *borrowOut = (result < 0);  // he pointer *borrowOut is set to true (1) if an underflow occurred, otherwise false (0) 
    return result & 0x0F;       // Ensure 4-bit output
}

void decode_and_execute(unsigned char instruction) {	//responsible for decoding and executing a single instruction for a 4-bit microprocessor simulator. This function takes an instruction from memory, decodes it into an opcode and an operand, and executes it accordingly
    unsigned char opcode = (instruction >> 4) & 0x0F;
    unsigned char operand = instruction & 0x0F;
    int jumped = 0;
    
    //jumped is used to indicate if the program counter was modified by a jump instruction (conditional or unconditional). This helps control whether programCounter should be incremented after the execution
    printf("Executing instruction at PC = %d: 0x%02X\n", programCounter, instruction);
    printf("Decoded - Opcode: 0x%X, Operand: 0x%X\n", opcode, operand);

switch (opcode) {
    case 0x0:  // RA = 0
        {
            unsigned char RA = readRegister(0);
            printf("Initial RA = %d\n", RA);
            writeRegister(0, 0);
            printf("RA set to 0\n");
        }
        break;
    case 0x1:  // RB = 1
        {
            unsigned char RB = readRegister(1);
            printf("Initial RB = %d\n", RB);
            writeRegister(1, 1);
            printf("RB set to 1\n");
        }
        break;
    case 0x2:  // RO = RA
        {
            unsigned char RA = readRegister(0);
            printf("Initial RA = %d\n", RA);
            writeRegister(2, RA);
            printf("RO set to RA -> RO = %d\n", RA);
        }
        break;
    case 0x3:  // RB = RA + RB
        {
            unsigned char RA = readRegister(0);
            unsigned char RB = readRegister(1);
            printf("Initial RA = %d, RB = %d\n", RA, RB);
            bool carryOut;
            RB = adder4(RA, RB, &carryOut);
            carryFlag = carryOut;
            writeRegister(1, RB);
            printf("RB = RA + RB -> RB = %d\n", RB);
        }
        break;
    case 0x4:  // JC = 0 (No Jump)
        {
            printf("JC = 0 (No Jump)\n");
        }
        break;
    case 0x5:  // RA = operand
        {
            writeRegister(0, operand);
            printf("LOAD RA: RA = %d\n", operand);
        }
        break;
    case 0x6:  // RB = operand
        {
            writeRegister(1, operand);
            printf("LOAD RB: RB = %d\n", operand);
        }
        break;
    case 0x7:  // JC: conditional jump
        {
            printf("JC: target = %d %s\n", operand, carryFlag ? "(Jump)" : "(No Jump)");
            if (carryFlag) {
                programCounter = operand;
                jumped = 1;
            }
        }
        break;
    case 0x8:  // JUMP: unconditional jump
        {
            printf("JUMP: target = %d\n", operand);
            programCounter = operand;
            jumped = 1;
        }
        break;
    default:
        printf("Unknown opcode: 0x%X\n", opcode);
        break;
	}

	if (!jumped) {		//If no jump occurred (jumped is 0), increment programCounter to point to the next instruction
		programCounter++;
	}

}


void executeInstructions(int stepMode) {	//responsible for executing instructions from the microprocessor's memory until all instructions are completed or a termination condition is met
    while (programCounter < MEM_SIZE) {
        unsigned char instruction = memory[programCounter];
        decode_and_execute(instruction); //alls the decode_and_execute function, which decodes the current instruction and performs the corresponding operation

        if (stepMode) {
            printf(" [Press Enter to continue]\n");
            getchar();  // Wait for user input in step mode
	//waits for the user to press Enter, allowing them to observe the program's state between each step. This mode is useful for debugging and closely analyzing the program's behavior.
        }
    }
}

