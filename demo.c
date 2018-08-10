#include <stdio.h>
#include "cpu.h"

int main(int argc, char** argv)
{
	CPU* cpu = newCPU();
	loadROM(cpu, "roms/Particle Demo [zeroZshadow, 2008].ch8");
	for(int i = 0; i < 16; i++)
	{
		evaluate(cpu);
	}
	dumpRegisters(cpu);
}
