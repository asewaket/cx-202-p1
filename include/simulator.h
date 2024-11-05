// simulator.h
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define registers and program counter
extern int RA;
extern int RB;
extern int RO;
extern int PC;

// Flags
extern int carry_flag;
extern int zero_flag;

// Define instruction memory
#define MAX_INSTRUCTIONS 100
extern char instructions[MAX_INSTRUCTIONS][9];  // Array to store 8-bit instructions

// Function prototypes
int load_binary_file(const char *filename);
void initialize();
void execute_instructions(int instruction_count);
void decode_and_execute(char *instruction);
void alu_add(int *reg1, int reg2);
void alu_subtract(int *reg1, int reg2);
void set_flags(int result);
int binary_to_int(const char *binary);

#endif // SIMULATOR_H

