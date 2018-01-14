#include "ines.h"
#include "memory.h"
#include "cpu.h"

int main()
{
	Memory mem;
	CPU cpu(&mem);

	ines donkeykong;
	donkeykong.load("../donkeykong.nes", &mem);
}
