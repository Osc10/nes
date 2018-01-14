#include <fstream>
#include <iostream>
#include <string>
#include <memory.h>

using namespace std;

class ines
{
public:
	void load(string path, Memory *mem);

private:
	uint8_t inesHeader[16]; 
	ifstream inesFile;
	string inesPath;
	Memory *memory;

	void loadHeader();
	void loadPrgRom();
};


