#include "ines.h"

void ines::load(std::string path, CPU *c, PPU *p)
{
	inesPath = path;
    cpu = c;
    ppu = p;
    inesFile.open(inesPath, std::ios::binary);

	if(!inesFile.is_open())
        std::cerr << "Invalid path to .nes file: " << inesPath << "\n";

	loadHeader();
	loadRom();
    setMirroringMode();

	inesFile.close();	
}

void ines::loadHeader()
{
	inesFile.read((char*)inesHeader, 16);
	if(!inesFile)
        std::cerr << "Failed to read .nes header!\n";
}

void ines::loadRom()
{
	//Loading PRG ROM (CPU memory, contains the program code)
	uint8_t sizeOfPrgRom = inesHeader[4]; // Either 1 or 2 in 16KB units
	int trainerOffset = ((inesHeader[6] & 0x4) == 0) ? 16 : 512 + 16;

	inesFile.seekg(trainerOffset);

    //If sizeOfPrgRom == 1, the memory is mirrored and loaded in twice.
    if(sizeOfPrgRom == 1)
	{
        cpu->loadPRGROM(&inesFile, 16384, 0);
		if(!inesFile)
            std::cerr << "Failed to read program ROM (mirror 1)!\n";
		inesFile.seekg(trainerOffset);
        cpu->loadPRGROM(&inesFile, 16384, 0x4000);
		if(!inesFile)
            std::cerr << "Failed to read program ROM (mirror 2)!\n";
	}
	else
	{
        cpu->loadPRGROM(&inesFile, 32768, 0);
		if(!inesFile)
            std::cerr << "Failed to read program ROM!\n";
	}

	//Loading CHR ROM (PPU memory, contains the pattern tables)
	uint8_t sizeOfChrRom = inesHeader[5]; // Size in 8KB units
	int offset = trainerOffset + 16384*sizeOfPrgRom;
	if(sizeOfChrRom == 0)
	{
#ifndef NDEBUG
        std::cout << "Using CHR RAM instead." << std::endl;
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
        ppu->loadPatternTables(&inesFile, 8192, 0);
		if(!inesFile)
            std::cerr << "Failed to read CHR ROM!\n";
	}
} 

void ines::setMirroringMode()
{
    mirroringMode m = (inesHeader[6] & 0x1) ? mirrorVertical : mirrorHorizontal;
    ppu->setNametableMirroring(m);
    //TODO: Implement proper mirror mode behaviour after implementing mappers.
    //See https://wiki.nesdev.com/w/index.php/INES#Flags_6 for more details.
}
