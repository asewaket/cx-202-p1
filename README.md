# K2 Microprocessor Project

This project implements an **Assembler** and **Simulator** for the K2 Microprocessor. The assembler converts K2 assembly code into machine code, and the simulator executes the machine code.

## Project Structure

- `assembler.c`: Converts K2 assembly code into machine code.
- `simulator.c`: Simulates the execution of K2 machine code.
- `include/`: Contains `assembler.h` and `simulator.h` header files.
- `programs/`: Contains assembly program `fibonacci.asm`.
- `bin/`: Contains binary (machine code) output file `fibonacci.bin`.

## Features

- **Assembler**:
  - Converts assembly instructions to 8-bit machine code.
  - Outputs a `.bin` file (e.g., `fibonacci.bin`).

- **Simulator**:
  - Executes machine code in either **step-by-step** or **continuous** mode.
  - Supports basic arithmetic, jump, and conditional instructions.
