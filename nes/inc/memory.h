#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>

using namespace std;

class Memory
{
public:
	uint8_t mem[65536];	
	bool isLoaded = false;
};

#endif
