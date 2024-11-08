#include "Memory.h"
void Memory::store(int address, int value) {
    if (address < 0 || address >= Capacity) {
        cout << "Address is out of bounds " << endl;
    }
    memoryMap[address] = value;
    cout << "Stored " << value << " at address 0x" << hex << address << dec << endl;
}
int Memory::load(int address) {
    if (memoryMap.find(address) != memoryMap.end()) {
        return memoryMap[address];
    }
    else {
        cerr << "Address 0x" << hex << address<< dec << " is uninitialized." << endl;
        return 0; // return 0 if address is uninitialized
    }
}
void Memory:: displayMemory() const {
   cout << "Memory Contents:" << endl;
    for (const auto& entry : memoryMap) {
      cout << "Address 0x" << std::hex << entry.first << ": " << dec << entry.second << endl;
    }
}