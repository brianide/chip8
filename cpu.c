#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cpu.h"

CPU* newCPU()
{
	CPU* cpu = malloc(sizeof(CPU));
	memset(&cpu->memory, 0, RAM_SIZE);
	cpu->pcReg = ROM_START;
	return cpu;
}

uint16_t loadROM(CPU* cpu, char* filename)
{
	FILE* rom = fopen(filename, "r");
	uint16_t len = fread(cpu->memory + ROM_START, 1, RAM_SIZE - ROM_START, rom);
	fclose(rom);
	return len;
}

uint8_t evaluate(CPU* cpu)
{
	uint8_t  hi = cpu->memory[cpu->pcReg];
	uint8_t  lo = cpu->memory[cpu->pcReg + 1];
	uint16_t op = hi << 8 | lo;
	
	cpu->pcReg += 2;
	
	uint8_t  h   = (hi >> 4) & 0xF;
	uint8_t  x   = hi & 0xF;
	uint8_t  y   = (lo >> 4) & 0xF;
	uint8_t  n   = lo & 0xF;
	uint16_t kk  = lo;
	uint16_t nnn = op & 0xFFF;
	
	uint8_t*  vx = &cpu->gpRegs[x];
	uint8_t*  vy = &cpu->gpRegs[y];
	uint8_t*  vf = &cpu->gpRegs[0xF];
	uint16_t* pc = &cpu->pcReg;
	uint8_t*  sp = &cpu->spReg;
	uint16_t* i  = &cpu->iReg;
	
	uint16_t acc = 0;
	
	#define c(code, mnem, test, op) if(test) { op; } else
	LIST_OPS(c) fprintf(stderr, "Unknown instruction: %04X\n", op);
	#undef c
	
	return 0;
}

void dumpMemory(CPU* cpu)
{
	for(uint16_t i = 0; i < RAM_SIZE; i += DUMP_BYTES_PER_LINE)
	{
		printf("%04X-%04X: ", i, i + DUMP_BYTES_PER_LINE - 1);
		for(uint8_t j = 0; j < DUMP_BYTES_PER_LINE; j++)
		{
			printf("%02X ", cpu->memory[i + j]);
		}
		printf("\n");
	}
}

void dumpRegisters(CPU* cpu)
{
	printf("I:%04X  PC:%04X  SP:%02X\n\n", cpu->iReg, cpu->pcReg, cpu->spReg);
	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < GP_REGISTERS; j++)
			if(!i)
				printf("v%X ", j);
			else
				printf("%02X ", cpu->gpRegs[j]);
		printf("\n");
	}
}
