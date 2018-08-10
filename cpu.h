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
	c("00EE", "RET",         op == 0x00EE,         *pc = cpu->stack[(*sp)--]) \
	c("1nnn", "JP nnn",      h == 0x1,             *pc = nnn) \
	c("2nnn", "CALL nnn",    h == 0x2,             cpu->stack[++(*sp)] = *pc; *pc = nnn) \
	c("3xkk", "SE Vx, kk",   h == 0x3,             if(*vx == kk)  *pc += 2) \
	c("4xkk", "SNE Vx, kk",  h == 0x4,             if(*vx != kk)  *pc += 2) \
	c("5xy0", "SE Vx, Vy",   h == 0x5,             if(*vx == *vy) *pc += 2) \
	c("6xkk", "LD Vx, kk",   h == 0x6,             *vx = kk) \
	c("7xkk", "ADD Vx, kk",  h == 0x7,             *vx += kk) \
	c("8xy0", "LD Vx, Vy",   h == 0x8 && n == 0x0, *vx = *vy) \
	c("8xy1", "OR Vx, Vy",   h == 0x8 && n == 0x1, *vx |= *vy) \
	c("8xy2", "AND Vx, Vy",  h == 0x8 && n == 0x2, *vx &= *vy) \
	c("8xy3", "XOR Vx, Vy",  h == 0x8 && n == 0x3, *vx ^= *vy) \
	c("8xy4", "ADD Vx, Vy",  h == 0x8 && n == 0x4, acc = *vx + *vy; *vf = acc >> 8; *vx = acc) \
	c("8xy5", "SUB Vx, Vy",  h == 0x8 && n == 0x5, acc = *vx - *vy; *vf = !(acc >> 8); *vx = acc) \
	c("8xy6", "SHR Vx",      h == 0x8 && n == 0x6, *vf = *vx & 1; *vx >>= 1) \
	c("8xy7", "SUBN Vx, Vy", h == 0x8 && n == 0x7, acc = *vy - *vx; *vf = !(acc >> 8); *vx = acc) \
	c("8xyE", "SHL Vx",      h == 0x8 && n == 0xE, *vf = *vx >> 7 & 0x1; *vx <<= 1) \
	c("Annn", "LD I, nnn",   h == 0xA,             *i = nnn) \
	c("Bnnn", "JP v0, nnn",  h == 0xB,             *pc = nnn + cpu->gpRegs[0]) \
	c("00FD", "EXIT",        op == 0x00FD,         return 1)

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
uint8_t evaluate(CPU*);
void dumpMemory(CPU*);
void dumpRegisters(CPU*);
