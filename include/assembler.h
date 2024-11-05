// assembler.h
#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 100
#define OUTPUT_FILE_EXTENSION ".bin"

// Function to convert an integer (0-15) to a binary string of fixed length
void int_to_binary(int value, char *output, int bits);

// Function to convert assembly instruction to machine code
int convert_to_machine_code(const char *instruction, char *machine_code);

// Function to handle errors
void handle_error(const char *message);

// Helper function to trim leading and trailing whitespace
char *trim_whitespace(char *str);

#endif // ASSEMBLER_H

