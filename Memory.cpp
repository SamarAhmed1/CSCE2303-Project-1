#include "Memory.h"
Memory::Memory() {
    Capacity = 4LL * 1024 * 1024 * 1024;
}
string Memory::DecToBinary(int Dec) const

{
	string Binary_String = "";
	if (Dec == 0)
		return "0";
	while (Dec > 0)
	{
		Binary_String = to_string(Dec % 2) + Binary_String;
		Dec = Dec / 2;
	}
	return Binary_String;
}

string Memory::DecToHex(int Dec) const

{
	string Hex_Str = "";
	int HexValue = 0;
	int HexLength = 0;
	while (Dec > 0) {
		HexValue = Dec % 16;
		if (HexValue < 10) {
			Hex_Str = to_string(HexValue) + Hex_Str;
		}
		else
		{
			Hex_Str = char(HexValue - 10 + 'A') + Hex_Str;
		}
		Dec /= 16;
		HexLength++;
	}
	if (HexLength < 1)
	{
		Hex_Str = "0" + Hex_Str;
	}
	return Hex_Str;
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
    cout << "\n=== Final Memory State ===" << endl;
    for (const auto& entry : memoryMap) {
        cout << "Address: " << entry.first << " | Value: " << entry.second << endl;
    }
    cout << "==========================" << endl;
	cout << "\n=== Final Memory State in Binary ===" << endl;
	for (const auto& entry : memoryMap) {
		cout << "Address: " << DecToBinary(entry.first) << " | Value: " << DecToBinary(entry.second) << endl;
	}
	cout << "==========================" << endl;
	cout << "\n=== Final Memory State in Hexadecimal ===" << endl;
	for (const auto& entry : memoryMap) {
		cout << "Address: " << DecToHex(entry.first) << " | Value: " << DecToHex(entry.second) << endl;
	}
	cout << "==========================" << endl;
}