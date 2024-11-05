// assembler.c
#include "assembler.h"

// Function to convert an integer (0-15) to a binary string of fixed length
void int_to_binary(int value, char *output, int bits) {
    for (int i = bits - 1; i >= 0; --i) {
        output[i] = (value % 2) ? '1' : '0';
        value /= 2;
    }
    output[bits] = '\0';  // Null-terminate the string
}

// Function to convert assembly instruction to machine code
int convert_to_machine_code(const char *instruction, char *machine_code) {
    memset(machine_code, '0', 8);
    machine_code[8] = '\0';

    if (strcmp(instruction, "RA=RA+RB") == 0) strncpy(machine_code, "00000000", 8);
    else if (strcmp(instruction, "RB=RA+RB") == 0) strncpy(machine_code, "00010000", 8);
    else if (strcmp(instruction, "RA=RA-RB") == 0) strncpy(machine_code, "00000100", 8);
    else if (strcmp(instruction, "RB=RA-RB") == 0) strncpy(machine_code, "00010100", 8);
    else if (strcmp(instruction, "RO=RA") == 0) strncpy(machine_code, "00100000", 8);

    // Instructions with immediate values
    else if (strncmp(instruction, "RA=", 3) == 0) {
        int imm = atoi(&instruction[3]);
        if (imm < 0 || imm > 15) return -1;
        char imm_bin[5];
        int_to_binary(imm, imm_bin, 4);
        snprintf(machine_code, 9, "0000%s", imm_bin);
    } else if (strncmp(instruction, "RB=", 3) == 0) {
        int imm = atoi(&instruction[3]);
        if (imm < 0 || imm > 15) return -1;
        char imm_bin[5];
        int_to_binary(imm, imm_bin, 4);
        snprintf(machine_code, 9, "0011%s", imm_bin);
    } else if (strncmp(instruction, "JC=", 3) == 0) {
        int imm = atoi(&instruction[3]);
        if (imm < 0 || imm > 15) return -1;
        char imm_bin[5];
        int_to_binary(imm, imm_bin, 4);
        snprintf(machine_code, 9, "0111%s", imm_bin);
    } else if (strncmp(instruction, "J=", 2) == 0) {
        int imm = atoi(&instruction[2]);
        if (imm < 0 || imm > 15) return -1;
        char imm_bin[5];
        int_to_binary(imm, imm_bin, 4);
        snprintf(machine_code, 9, "1011%s", imm_bin);
    } else {
        return -1;
    }

    return 0;
}

// Function to handle errors
void handle_error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(1);
}

// Helper function to trim leading and trailing whitespace
char *trim_whitespace(char *str) {
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = 0;
    return str;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        handle_error("Please provide the assembly file name as a command line argument.");
    }

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        handle_error("Failed to open input file.");
    }

    char output_filename[100];
    snprintf(output_filename, sizeof(output_filename), "%s%s", strtok(argv[1], "."), OUTPUT_FILE_EXTENSION);

   
    // Open the output binary file
    FILE *output_file = fopen(output_filename, "w");
    if (!output_file) {
        fclose(input_file);
        handle_error("Failed to create output file.");
    }

    char line[MAX_LINE_LENGTH];
    char machine_code[9];  // For an 8-bit machine code with a null terminator

    // Read each line of the assembly file
    while (fgets(line, sizeof(line), input_file)) {
        // Strip newline and trim whitespace
        line[strcspn(line, "\n")] = '\0';
        char *instruction = trim_whitespace(line);

        // Convert the assembly instruction to machine code
        if (convert_to_machine_code(instruction, machine_code) == -1) {
            fprintf(stderr, "Warning: Invalid instruction '%s'\n", instruction);
            continue;
        }

        // Write the machine code to the binary file
        fprintf(output_file, "%s\n", machine_code);
    }

    // Close files
    fclose(input_file);
    fclose(output_file);

    printf("Assembler finished successfully. Output written to %s\n", output_filename);
    return 0;
}

