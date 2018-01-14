#include <ines.h>

int main()
{
	Memory mem;
	ines donkeykong;
	donkeykong.load("../donkeykong.nes", &mem);
}
