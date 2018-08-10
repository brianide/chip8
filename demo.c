#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "cpu.h"

int main(int argc, char** argv)
{
	CPU* cpu = newCPU();
	loadROM(cpu, "roms/test-adder.ch8");
	while(!evaluate(cpu));
	dumpMemory(cpu);
	dumpRegisters(cpu);
	free(cpu);
}
