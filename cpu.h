#include <stdint.h>

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

#define SPRITE_HEIGHT 5

#define RAM_SIZE 0x1000
#define STACK_SIZE 0x10
#define GP_REGISTERS 0x10
#define ROM_START 0x200

#define DUMP_BYTES_PER_LINE 16

// HI LO
// -n nn
// -x --
// -- y-
// -- -n
// -- kk
#define LIST_OPS(c) \
	c("00E0", "CLS",         op == 0x00E0,         memset(cpu->display, 0, DISPLAY_WIDTH * DISPLAY_HEIGHT)) \
	c("00EE", "RET",         op == 0x00EE,         cpu->pcReg = cpu->stack[cpu->spReg--]) \
	c("1nnn", "JP nnn",      h == 0x1,             cpu->pcReg = nnn) \
	c("2nnn", "CALL nnn",    h == 0x2,             cpu->stack[++cpu->spReg] = cpu->pcReg; cpu->pcReg = nnn) \
	c("3xkk", "SE Vx, kk",   h == 0x3,             if(cpu->gpRegs[x] == kk) cpu->pcReg += 2) \
	c("4xkk", "SNE Vx, kk",  h == 0x4,             if(cpu->gpRegs[x] != kk) cpu->pcReg += 2) \
	c("5xy0", "SE Vx, Vy",   h == 0x5,             if(cpu->gpRegs[x] == cpu->gpRegs[y]) cpu->pcReg += 2) \
	c("6xkk", "LD Vx, kk",   h == 0x6,             cpu->gpRegs[x] = kk) \
	c("7xkk", "ADD Vx, kk",  h == 0x7,             cpu->gpRegs[x] += kk) \
	c("8xy0", "LD Vx, Vy",   h == 0x8 && n == 0x0, cpu->gpRegs[x] = cpu->gpRegs[y]) \
	c("8xy1", "OR Vx, Vy",   h == 0x8 && n == 0x1, cpu->gpRegs[x] |= cpu->gpRegs[y]) \
	c("8xy2", "AND Vx, Vy",  h == 0x8 && n == 0x2, cpu->gpRegs[x] &= cpu->gpRegs[y]) \
	c("8xy3", "XOR Vx, Vy",  h == 0x8 && n == 0x3, cpu->gpRegs[x] ^= cpu->gpRegs[y]) \
	c("8xy4", "ADD Vx, Vy",  h == 0x8 && n == 0x4, acc = cpu->gpRegs[x] + cpu->gpRegs[y]; cpu->gpRegs[0xF] = acc >> 8; cpu->gpRegs[x] = acc) \
	c("8xy5", "SUB Vx, Vy",  h == 0x8 && n == 0x5, cpu->iReg = nnn) \
	c("8xy6", "SHR Vx",      h == 0x8 && n == 0x6, cpu->iReg = nnn) \
	c("8xy7", "SUBN Vx, Vy", h == 0x8 && n == 0x7, cpu->iReg = nnn) \
	c("8xyE", "SHL Vx",      h == 0x8 && n == 0xE, cpu->iReg = nnn)

typedef union
{
	uint8_t memory[RAM_SIZE];
	struct
	{
		uint8_t sprites[SPRITE_HEIGHT * 0xF];
		uint16_t pcReg;
		uint8_t spReg;
		uint8_t gpRegs[GP_REGISTERS];
		uint16_t iReg;
		uint8_t display[DISPLAY_WIDTH * DISPLAY_HEIGHT];
		uint16_t stack[STACK_SIZE];
	};
} CPU;

CPU* newCPU();
uint16_t loadROM(CPU*, char*);
void evaluate(CPU*);
void dumpMemory(CPU*);
void dumpRegisters(CPU*);
