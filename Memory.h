#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;
class Memory
{
private:
    unordered_map<int, int> memoryMap;
    long long Capacity;
public:
    Memory();
    string DecToBinary(int Dec) const; 
    string DecToHex(int Dec) const;
    // Function to store a value at a specific address
    void store(int address, int value);
    // Function to load a value from a specific address
    int load(int address);
    // Display all non-zero memory values (for debugging)
    void displayMemory() const;
};