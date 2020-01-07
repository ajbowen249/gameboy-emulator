#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

#include "Cartridge.h"
#include "CPU.h"
#include "GameboyMotherboard.h"

int main(int argc, char** args) {
    if (argc < 2) {
        std::cout << "Missing ROM path." << std::endl;
        return -1;
    }

    std::ifstream inFile;
    inFile.open(args[1], std::ios::in | std::ios::binary);

    if (!inFile.is_open()) {
        std::cout << "Failure to open " << args[1] << std::endl;
        return -2;
    }

    inFile.unsetf(std::ios::skipws);
    inFile.seekg(0, std::ios::end);
    auto cartSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    std::vector<uint8_t> cartData(cartSize);
    inFile.read((char*)&cartData[0], cartSize);
    inFile.close();

    auto cartridge = std::make_shared<Cartridge>(std::move(cartData));
    auto mobo = std::make_shared<GameboyMotherboard>();
    mobo->setCartridge(cartridge);

    CPU cpu(mobo);

    int iteration = 0;
    while (true) {
        cpu.clock();

        std::this_thread::sleep_for(std::chrono::nanoseconds(238));
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (iteration++ >= 4) {
            std::cout << std::hex << cpu._programCounter << std::endl;
            iteration = 0;
        }
    }

    return 0;
}
