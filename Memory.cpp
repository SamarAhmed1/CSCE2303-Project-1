#include "Memory.h"
Memory::Memory() {
    Capacity = 4LL * 1024 * 1024 * 1024;
}
void Memory::store(int address, int value) {
    cout << "Capacity: " << Capacity << ", Address: " << address << endl;
    if (address < 0 || address >= Capacity) {
        cerr << "Error: Address " << address << " is out of bounds." << endl;
        return;
    }
    else {
        memoryMap[address] = value;
        cout << "Stored " << value << " at address " << address << endl;
    }
}
int Memory::load(int address) {
    if (memoryMap.find(address) != memoryMap.end()) 
            return memoryMap[address];
    else {
        cout << "No value found in this memory address, returning 0" << endl;
        //cerr << "Address 0x" << hex << address << dec << " is uninitialized." << endl;
        return 0; // return 0 if address is uninitialized
    }
}
void Memory::displayMemory() const {
    cout << "Memory Contents:" << endl;
    for (const auto& entry : memoryMap) {
        cout << "Address 0x" << std::hex << entry.first << ": " << dec << entry.second << endl;
    }
}