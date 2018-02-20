#include "ines.h"

void ines::load(string path, CPUMemory *mem, PPUMemory *ppumem)
{
	inesPath = path;
	memory = mem;
	ppumemory = ppumem;
	inesFile.open(inesPath, ios::binary);

	if(!inesFile.is_open())
		cerr << "Invalid path to .nes file: " << inesPath << "\n";

	loadHeader();
	loadRom();

	memory->isLoaded = true;

	inesFile.close();	
}

void ines::loadHeader()
{
	inesFile.read((char*)inesHeader, 16);
	if(!inesFile)
		cerr << "Failed to read .nes header!\n";
}

void ines::loadRom()
{
	//Loading PRG ROM (CPU memory, contains the program code)
	uint8_t sizeOfPrgRom = inesHeader[4]; // Either 1 or 2 in 16KB units
	int trainerOffset = ((inesHeader[6] & 0x4) == 0) ? 16 : 512 + 16;

	inesFile.seekg(trainerOffset);

	if(sizeOfPrgRom == 1)
	{
		inesFile.read((char*)(memory->programROM), 16384);
		if(!inesFile)
		{
			cerr << "Failed to read program ROM (mirror 1)!\n";
			cerr << "Only " << inesFile.gcount() << " could be read.\n";
		}
		inesFile.seekg(trainerOffset);
		inesFile.read((char*)(&memory->programROM[0x4000]), 16384);
		if(!inesFile)
		{
			cerr << "Failed to read program ROM (mirror 2)!\n";
			cerr << "Only " << inesFile.gcount() << " could be read.\n";
		}
	}
	else
	{
		inesFile.read((char*)(memory->programROM), 32768);
		if(!inesFile)
			cerr << "Failed to read program ROM!\n";
	}

	
	//Loading CHR ROM (PPU memory, contains the pattern tables)
	uint8_t sizeOfChrRom = inesHeader[5]; // Size in 8KB units
	int offset = trainerOffset + 16384*sizeOfPrgRom;
	if(sizeOfChrRom == 0)
	{
#ifdef NDEBUG
		cout << "Using CHR RAM instead." << endl;
#endif
		// TODO: Implement CHR RAM.
	}
	else if(sizeOfChrRom > 1)
	{
		// TODO: load CHR ROM when there is more than one 8KB unit.
	}
	else
	{
		inesFile.seekg(offset);
		inesFile.read((char*)(ppumemory->patternTables), 8192);
		if(!inesFile)
			cerr << "Failed to CHR ROM!\n";
	}
} 
