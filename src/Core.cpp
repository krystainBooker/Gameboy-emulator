#include "Core.h"
#include "MemoryManagementUnit.h"

Cartridge cartridge;
MemoryManagementUnit memoryManagementUnit;

Core::Core(){
    cartridge.LoadBootRom(PROJECT_SOURCE_DIR"/roms/bootrom.gb");
    cartridge.LoadRom(PROJECT_SOURCE_DIR"/roms/tetris.gb");


    //Boot rom is $0000 (0) - $0100 (256)
    memoryManagementUnit.wram = std::vector<uint8_t>(0x2000, 0); //8192 bytes (8 KiB)
    for(int i = 0; i < 0x0FF; ++i){ //255
        memoryManagementUnit.wram[i] = cartridge.bootRom[i];
    }

    //Game rom is $0100 (256) - 0x2000 (8192)
    for(int i = 0x0100; i < 0x2000; ++i){
        memoryManagementUnit.wram[i] = cartridge.cartridgeRom[i - 256];
    }

    std::cout << "Core initialized" << std::endl;
}

void Core::EmulateCycle(){
    uint8_t byte = memoryManagementUnit.wram[pc.Read()];

    switch(byte){
        case 0xC3: {
            unsigned short pcVal = pc.Read();
            pcVal++;
            pc.Write(pcVal);
            break;
        }
        default:
            char buffer[100];
            sprintf(buffer,"%x",byte);
            std::cerr << "Unknown opcode: 0x" << buffer << std::endl;
            exit(1);
    }
}

void Core::JPnn(){

}