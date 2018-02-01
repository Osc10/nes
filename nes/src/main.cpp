#include "ines.h"
#include "memory.h"
#include "cpu.h"

int main()
{
	CPUMemory mem;
	CPU cpu(&mem);

	ines rom;
	//rom.load("../donkeykong.nes", &mem);
	rom.load("../test.nes", &mem);

	cpu.run();
}
