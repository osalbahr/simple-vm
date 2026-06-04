#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define DEFAULT_MEMORY_SIZE 4096

typedef struct CPU {
    uint32_t ip;
    uint32_t ir;
    uint8_t r0;
    uint8_t r1;
    uint8_t r2;
    uint8_t r3;
} CPU;

typedef struct Instruction {
    uint32_t opcode:3;
    uint32_t reg0:2;
    uint32_t reg1:2;
    uint32_t out:2;
} Instruction;

typedef struct Program {
    uint32_t instructionCount;
    Instruction *instructions;
} Program;

typedef struct Memory {
    uint8_t memory[DEFAULT_MEMORY_SIZE];
} Memory;

typedef struct VM {
    CPU cpu;
    Memory memory;
} VM;

uint8_t get_reg_val(CPU *cpu, uint32_t reg)
{
    switch (reg) {
        case 0b00:
            return cpu->r0;
        case 0b01:
            return cpu->r1;
        case 0b10:
            return cpu->r2;
        case 0b11:
            return cpu->r3;
    }

    printf("Invalid reg: %d\n", reg);
    exit(1);
}

bool opcode_add(VM *vm, Instruction *instruction)
{
    uint8_t a = get_reg_val(&vm->cpu, instruction->reg0);
    uint8_t b = get_reg_val(&vm->cpu, instruction->reg1);

    switch (instruction->out) {
        case 0b00:
            vm->cpu.r0 = a + b;
            return true;
        case 0b01:
            vm->cpu.r1 = a + b;
            return true;
        case 0b10:
            vm->cpu.r2 = a + b;
            return true;
        case 0b11:
            vm->cpu.r3 = a + b;
            return true;
    }

    return false;
}

bool execute(VM *vm, Program *program)
{
    while (vm->cpu.ip < program->instructionCount) {
        Instruction instruction = program->instructions[vm->cpu.ip++];
        opcode_add(vm, &instruction);
        vm->cpu.ir++;
    }

    return true;
}

int main()
{
    VM vm;
    vm.cpu.ip = 0x0;
    vm.cpu.ir = 0x0;
    vm.cpu.r0 = 2;
    vm.cpu.r1 = 3;
    vm.cpu.r2 = 0;
    vm.cpu.r3 = 0;

    Instruction instruction;
    instruction.opcode = 0;
    instruction.reg0 = 0b00;
    instruction.reg1 = 0b01;
    instruction.out = 0b10;

    Program program;
    program.instructionCount = 1;
    program.instructions = &instruction;

    if (execute(&vm, &program)) {
        printf("r0 = %d\n", vm.cpu.r0);
        printf("r1 = %d\n", vm.cpu.r1);
        printf("r2 = %d\n", vm.cpu.r2);
        printf("r3 = %d\n", vm.cpu.r3);
    }
}
