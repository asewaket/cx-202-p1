#include "simulator.h"

// Define registers and program counter
int RA = 0;
int RB = 0;
int RO = 0;
int PC = 0;

// Flags
int carry_flag = 0;
int zero_flag = 0;

// Define instruction memory
char instructions[MAX_INSTRUCTIONS][9];

// Function to convert a binary string to an integer
int binary_to_int(const char *binary) {
    return strtol(binary, NULL, 2);
}

// Function to load binary file
int load_binary_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return -1;
    }

    int count = 0;
    while (fgets(instructions[count], 9, file) != NULL && count < MAX_INSTRUCTIONS) {
        instructions[count][strcspn(instructions[count], "\n")] = '\0';
        count++;
    }

    fclose(file);
    return count;
}

// Function to initialize registers and flags
void initialize() {
    RA = 0;
    RB = 0;
    RO = 0;
    PC = 0;
    carry_flag = 0;
    zero_flag = 0;
}

// ALU Operations
void alu_add(int *reg1, int reg2) {
    int result = *reg1 + reg2;
    set_flags(result);
    *reg1 = result & 0xF;  // Mask to 4 bits
}

void alu_subtract(int *reg1, int reg2) {
    int result = *reg1 - reg2;
    set_flags(result);
    *reg1 = result & 0xF;  // Mask to 4 bits
}

// Function to set flags based on the result
void set_flags(int result) {
    carry_flag = (result > 15 || result < 0);
    zero_flag = (result == 0);
}

// Function to decode and execute each instruction
void decode_and_execute(char *instruction) {
    char opcode[6];
    strncpy(opcode, instruction, 5);
    opcode[5] = '\0';

    if (strcmp(opcode, "00000") == 0) {
        alu_add(&RA, RB);
    } else if (strcmp(opcode, "00001") == 0) {
        RA = binary_to_int(&instruction[5]);
    } else if (strcmp(opcode, "00010") == 0) {
        RB = binary_to_int(&instruction[5]);
    } else if (strcmp(opcode, "00011") == 0) {
        alu_add(&RB, RA);
    } else if (strcmp(opcode, "00100") == 0) {
        RO = RA;
    } else if (strcmp(opcode, "01110") == 0) {
        int target = binary_to_int(&instruction[4]);
        if (carry_flag) {
            PC = target - 1;
        }
    } else if (strcmp(opcode, "10110") == 0) {
        int target = binary_to_int(&instruction[4]);
        PC = target - 1;
    } else if (strcmp(opcode, "000101") == 0) {
        alu_subtract(&RA, RB);
    } else if (strcmp(opcode, "000110") == 0) {
        alu_subtract(&RB, RA);
    } else {
        fprintf(stderr, "Unknown instruction: %s\n", instruction);
    }
}

// Main Execution Loop
void execute_instructions(int instruction_count) {
    while (PC < instruction_count) {
        char *current_instruction = instructions[PC];
        printf("PC=%d | Executing: %s | RA=%d, RB=%d, RO=%d, CF=%d, ZF=%d\n",
               PC, current_instruction, RA, RB, RO, carry_flag, zero_flag);
        decode_and_execute(current_instruction);
        PC++;
    }
}

// Main function
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <binary file>\n", argv[0]);
        return 1;
    }

    int instruction_count = load_binary_file(argv[1]);
    if (instruction_count == -1) {
        return 1;
    }

    initialize();
    execute_instructions(instruction_count);

    return 0;
}

