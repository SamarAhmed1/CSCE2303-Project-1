// Project 1 R and U.cpp : This file contains the 'main' function.Program execution begins and ends there.
//
#include <iostream>
#include <utility>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <utility>
#include "Memory.h"
using namespace std;


map<string, int> labels;
Memory memory;
vector <string> RInstructions = { "ADD", "SUB", "SLL", "SLT", "SLTU", "XOR", "SRL", "SRA", "OR", "AND" };
vector <string> BInstructions = { "BEQ", "BNE", "BLT", "BGE", "BLTU", "BLGEU" };
vector <string> SInstructions = { "SW", "SH", "SB" };
vector <string> IInstructions = { "LB", "LH", "LW", "LBU", "LHU", "ADDI", "SLTI", "SLTU", "XORI", "ORI","ANDI", "SLLI", "SRLI", "SRAI", "JALR" };
vector <string> JInstructions = { "JAL" };
vector <string> UInstructions = { "LUI", "AUIPC" };
vector <string> haltInstructions = { "ECALL", "EBREAK", "PAUSE", "FENCE", "FENCE.TSO" };
vector <int> registers(32, 0);

int programCounter = 0;
int startAddress = 0;

void updateProgramCounter() {
	programCounter += 4;
	cout << "Program Counter: " << programCounter << endl;
}

struct SFormat
{
	string opcode;
	int imm;
	pair <string, int> rs1;
	pair <string, int> rs2;
};

struct BFormat {
	string opcode;
	pair<string, int> rs1;
	pair<string, int> rs2;
	string imm;
};
struct RFormat {
	string name;
	int opcode[7];
	pair <string, int> rd;
	int funct3[3];
	pair<string, int> rs1;
	pair<string, int> rs2;
	int funct7[7];
};

struct UFormat {
	string name;
	int opcode[7];
	pair <string, int> rd;
	int imm;
};

struct Jformat
{
	string name;
	int opcode[7];
	pair <string, int> rd;
	string imm;
};

struct Iformat
{
	string name;
	//int opcode[7];
	int imm;
	pair <string, int> rs1;
	pair <string, int> rd;
};

void updateMemoryAndDisplay(int offset, int value) {
	//int address = startAddress + offset;
	memory.store(offset, value);
	cout << "Memory Updated -> Address: " << offset << " | Value: " << value << endl;
}

int extractRegisterNumber(const string& regName) {
	string numStr = regName.substr(1); // Extract the numeric part after 'x'
	return stoi(numStr); // Convert the numeric part to an integer
}

string DecToBinary(int Dec)

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

string DecToHex(int Dec)

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

void ProcessJformat(Jformat& instruction, int& counter) {
	string valu = instruction.imm;
	int returnAddress = programCounter + 4;
	int registerNumber = extractRegisterNumber(instruction.rd.first);

	if (instruction.name == "JAL") {
		// Store the return address in the specified register
		registers[registerNumber] = returnAddress;
		cout << "JAL: " << instruction.rd.first << " jumping to address " << valu << endl;
	}

	// Check if the label exists in the map
	if (labels.find(valu) != labels.end()) {
		// Get the address associated with the label
		int jumpAddress = labels[valu];
		cout << "Jumping to label: " << valu << " at address " << jumpAddress << endl;

		// Update the program counter to jump to the label
		programCounter = jumpAddress;
		int offset = (labels[valu] / 4);
		counter = offset;
	}
	else {
		cout << "Label not found: " << valu << endl;
	}

	cout << "Register " << instruction.rd.first << " updated to " << registers[registerNumber]
		<< " after instruction " << instruction.name << endl;
	cout << "The value stored inside the register is " << DecToBinary(registers[registerNumber])
		<< " in binary and " << DecToHex(registers[registerNumber]) << " in hexadecimal" << endl;

	// No need to update the program counter here since it's set to the label address
	//updateProgramCounter();
}



void processRFormat(RFormat& instruction) {
	//cout << "In process R" << endl;
	if (instruction.name == "ADD") {
		//	cout << "In ADD" << endl;
		//	cout << instruction.rs1.second << " " << instruction.rs2.second << " " << endl;
		instruction.rd.second = instruction.rs1.second + instruction.rs2.second;
	}

	else if (instruction.name == "SUB") {
		instruction.rd.second = instruction.rs1.second + instruction.rs2.second;
	}

	else if (instruction.name == "SLL") {
		instruction.rd.second = instruction.rs1.second * pow(2, instruction.rs2.second);
	}

	else if (instruction.name == "SLT") {
		instruction.rd.second = instruction.rs1.second < instruction.rs2.second ? 1 : 0;
	}

	else if (instruction.name == "SLTU") {
		if (instruction.rs1.second < 0)
			instruction.rs1.second = abs(instruction.rs1.second);
		if (instruction.rs2.second < 0)
			instruction.rs2.second = abs(instruction.rs2.second);
		instruction.rd.second = instruction.rs1.second < instruction.rs2.second ? 1 : 0;
	}

	else if (instruction.name == "XOR") {
		instruction.rd.second = instruction.rs1.second ^ instruction.rs2.second;
	}

	else if (instruction.name == "SRL") {
		if (instruction.rs1.second < 0)
			instruction.rs1.second = abs(instruction.rs1.second);
		if (instruction.rs2.second < 0)
			instruction.rs2.second = abs(instruction.rs2.second);
		instruction.rd.second = instruction.rs1.second / pow(2, instruction.rs2.second);
	}

	else if (instruction.name == "SRA") {
		instruction.rd.second = instruction.rs1.second / pow(2, instruction.rs2.second);
	}

	else if (instruction.name == "OR") {
		instruction.rd.second = instruction.rs1.second | instruction.rs2.second;
	}

	else if (instruction.name == "AND") {
		instruction.rd.second = instruction.rs1.second & instruction.rs2.second;
	}

	int registerNumber = extractRegisterNumber(instruction.rd.first);
	registers[registerNumber] = instruction.rd.second;
	//cout << registers[registerNumber] << endl;
	cout << "Register " << instruction.rd.first << " updated to " << instruction.rd.second << " after instruction " << instruction.name << endl;
	cout << "The value stored inside the register is  " << DecToBinary(instruction.rd.second) << " in binary and " << DecToHex(instruction.rd.second) << " in hexadecimal" << endl;
	updateProgramCounter();
}

void processUFormat(UFormat& instruction) {
	if (instruction.name == "LUI") {
		instruction.rd.second = instruction.imm * pow(2, 16);
	}

	else if (instruction.name == "AUIPC") {
		instruction.rd.second = programCounter + instruction.imm * pow(2, 12);
	}
	int registerNumber = extractRegisterNumber(instruction.rd.first);
	registers[registerNumber] = instruction.rd.second;
	cout << "Register " << instruction.rd.first << " updated to " << instruction.rd.second << " after instruction " << instruction.name << endl;
	cout << "The value stored inside the register is  " << DecToBinary(instruction.rd.second) << " in binary and " << DecToHex(instruction.rd.second) << " in hexadecimal" << endl;
	updateProgramCounter();
}
void ProcessBFormat(BFormat& value, int& counter) {
	bool Statement = false;

	// Extract integer values for comparison
	int rs1_value = registers[extractRegisterNumber(value.rs1.first)];
	int rs2_value = registers[extractRegisterNumber(value.rs2.first)];

	// Check branch conditions based on the opcode
	if (value.opcode == "BEQ") {
		if (rs1_value == rs2_value) {
			Statement = true;
		}
		else {
			Statement = false;
		}
	}
	else if (value.opcode == "BNE") {
		if (rs1_value != rs2_value) {
			Statement = true;
		}
		else {
			Statement = false;
		}
	}
	else if (value.opcode == "BLT") {
		if (rs1_value < rs2_value) {
			Statement = true;
		}
		else {
			Statement = false;
		}
	}
	else if (value.opcode == "BGE") {
		if (rs1_value >= rs2_value) {
			Statement = true;
		}
		else {
			Statement = false;
		}
	}
	else if (value.opcode == "BLTU") {
		if (static_cast<unsigned>(rs1_value) < static_cast<unsigned>(rs2_value)) {
			Statement = true;
		}
		else {
			Statement = false;
		}
	}
	else if (value.opcode == "BGEU") {
		if (static_cast<unsigned>(rs1_value) >= static_cast<unsigned>(rs2_value)) {
			Statement = true;
		}
		else {
			Statement = false;
		}
	}
	if (Statement == true) {
		if (labels.find(value.imm) != labels.end()) {
			int offset = (labels[value.imm] / 4);

			programCounter = labels[value.imm];
			counter = offset;
			cout << "Branch taken to label: " << value.imm << " at line " << counter << endl;
			cout << "Branch taken, programCounter set to: " << programCounter << endl;
		}
		else {
			cout << "Label not found: " << value.imm << endl;
		}
	}
	else {
		updateProgramCounter();
		cout << "Branch not taken, continuing to next instruction." << endl;
	}
}

void ProcessSFormat(SFormat& value) {
	//	int baseAddr = registers[extractRegisterNumber(value.rs1.second)];
	int address = value.imm + value.rs1.second;  // Adjusted for 4-byte addressing
	cout << "rs2 at process is " << value.rs2.second << endl;
	//memory[address + 1] = (x2 >> 8) & 0xFF; // Store upper byte
	if (value.opcode == "SB") {
		//	memory.store(address, value.rs2.first & 0xFF);
		//value.rs2.first = value.rs1.first;
		//value.rs2.second = value.rs1.second;
		//updateMemoryAndDisplay(address, 0); // Byte
		memory.store(address, value.rs2.second);
	}
	else if (value.opcode == "SH") {
		//	memory.store(address, value.rs2.first & 0xFFFF);
		//value.rs2.first = value.rs1.first;
		//value.rs2.second = value.rs1.second;
		//updateMemoryAndDisplay(address, 0); // Halfword
		memory.store(address, value.rs2.second);
	}
	else if (value.opcode == "SW") {
		//	memory.store(address, value.rs2.first);
		//value.rs2.first = value.rs1.first;
		//value.rs2.second = value.rs1.second;
		//updateMemoryAndDisplay(address, 0); // Word
		memory.store(address, value.rs2.second);
	}
	else {
		cout << "Unknown S-format opcode: " << value.opcode << endl;
	}
	updateProgramCounter();

}


void ProcessIformat(Iformat& instruction, int& counter) {
	int address = instruction.imm + instruction.rs1.second;
	int returnAddress = programCounter + 4;
	if (instruction.name == "LB") {
		instruction.rd.second = memory.load(address) & 0xFF;
	}
	else if (instruction.name == "LH") {
		instruction.rd.second = memory.load(address) & 0xFFFF;
	}
	else if (instruction.name == "LW") {
		cout << "In LW" << endl;
		instruction.rd.second = memory.load(address);
	}
	else if (instruction.name == "LBU") {
		address = instruction.rs1.second + instruction.imm;
		instruction.rd.second = memory.load(address) & 0xFF;
		cout << "LBU: " << instruction.rs1.second + instruction.imm << " = " << instruction.rd.second << endl;
	}
	else if (instruction.name == "LHU") {
		address = instruction.rs1.second + instruction.imm;
		instruction.rd.second = memory.load(address) & 0xFFFF;
		cout << "LHU: " << instruction.rs1.second + instruction.imm << " = " << instruction.rd.second << endl;
	}
	else if (instruction.name == "ADDI") {
		instruction.rd.second = instruction.rs1.second + instruction.imm;
		cout << "ADDI: " << instruction.rs1.second << " + " << instruction.imm << " = " << instruction.rd.second << endl;
	}
	else if (instruction.name == "SLTI") {
		instruction.rd.second = instruction.rs1.second < instruction.imm ? 1 : 0;
	}
	else if (instruction.name == "SLTU") {
		instruction.rd.second = static_cast<unsigned int>(instruction.rs1.second) < static_cast<unsigned int>(instruction.imm) ? 1 : 0;
	}
	else if (instruction.name == "XORI") {
		instruction.rd.second = instruction.rs1.second ^ instruction.imm;
	}
	else if (instruction.name == "ORI") {
		instruction.rd.second = instruction.rs1.second | instruction.imm;
	}
	else if (instruction.name == "ANDI") {
		instruction.rd.second = instruction.rs1.second & instruction.imm;
	}
	else if (instruction.name == "SLLI") {
		instruction.rd.second = instruction.rs1.second << instruction.imm;
	}
	else if (instruction.name == "SRLI") {
		instruction.rd.second = static_cast<unsigned int>(instruction.rs1.second) >> instruction.imm;
	}
	else if (instruction.name == "SRAI") {
		instruction.rd.second = instruction.rs1.second >> instruction.imm;
	}
	else if (instruction.name == "JALR") {
		cout << "Entering JALR" << endl;
		// assuming `rs1` is defined in `Jformat`
		int target = (instruction.rs1.second + instruction.imm); // align the address to even boundary
		instruction.rd.second = returnAddress;
		cout << "JALR: Setting return address " << instruction.rd.second
			<< ", jumping to address " << target << endl;
		counter = (returnAddress / 4)-1;
		programCounter = target;
		cout << "COUNTER" << counter << endl;
	}

	else {
		cout << "Unknown I-format opcode: " << instruction.name << endl;
	}

	int registerNumber = extractRegisterNumber(instruction.rd.first);
	if (registerNumber >= 0 && registerNumber < registers.size()) {
		registers[registerNumber] = instruction.rd.second;
		cout << "Register " << instruction.rd.first << " updated to " << instruction.rd.second << " after instruction " << instruction.name << endl;
		cout << "The value stored inside the register is  " << DecToBinary(instruction.rd.second) << " in binary and " << DecToHex(instruction.rd.second) << " in hexadecimal" << endl;
	}
	else {
		cout << "Error: Invalid register index " << registerNumber << endl;
	}
	if (instruction.name != "JALR")
		updateProgramCounter();
}

void readIformat(const string& line, int& counter)
{
	Iformat I;
	stringstream ss(line);
	string temp;
	string instructionName, rd, rs1, imm;

	getline(ss, instructionName, ' ');
	if (instructionName == "LW" || instructionName == "LB" || instructionName == "LH" || instructionName == "LBU" || instructionName == "LHU" || instructionName == "JALR")
	{
		getline(ss, temp, ',');
		rd = temp;
		getline(ss, temp, '(');
		imm = temp;
		getline(ss, temp, ')');
		rs1 = temp;
	}
	else {
		getline(ss, temp, ',');
		rd = temp;
		getline(ss, temp, ' ');
		getline(ss, temp, ',');
		rs1 = temp;
		getline(ss, temp, ' ');
		getline(ss, temp, ' ');
		imm = temp;

	}
	int immm = stoi(imm);
	I.name = instructionName;
	I.rd.first = rd;
	I.rd.second = registers[extractRegisterNumber(rd)];
	I.rs1.first = rs1;
	I.rs1.second = registers[extractRegisterNumber(rs1)];
	I.imm = immm;
	ProcessIformat(I, counter);
}

void readJFormat(const string& line, int &counter)
{
	Jformat J;
	stringstream ss(line);
	string instructionName, rd, imm1, temp;

	getline(ss, instructionName, ' ');
	getline(ss, rd, ',');
	getline(ss, temp, ' ');
	getline(ss, temp, ' ');
	imm1 = temp;

	J.name = instructionName;
	J.rd.first = rd;
	J.rd.second = registers[extractRegisterNumber(rd)];
	J.imm = imm1;

	ProcessJformat(J, counter);
}

void readRFormat(const string& line) {
	RFormat R;
	string instructionName, rd, rs1, rs2;
	stringstream ss(line);
	string temp;
	getline(ss, instructionName, ' ');
	getline(ss, temp, ',');
	rd = temp;
	getline(ss, temp, ' ');
	getline(ss, temp, ',');
	rs1 = temp;
	getline(ss, temp, ' ');

	getline(ss, temp, ' ');
	rs2 = temp;


	R.name = instructionName;
	R.rd.first = rd;
	R.rs1.first = rs1;
	R.rs1.second = registers[extractRegisterNumber(rs1)];
	R.rs2.first = rs2;
	R.rs2.second = registers[extractRegisterNumber(rs2)];
	processRFormat(R);
}


void readSFormat(const string& line) {
	SFormat S;
	string instructionName, rs1, rs2;
	int imm;
	char comma, bracketOpen, bracketClosed;
	stringstream ss(line);
	string temp;
	getline(ss, instructionName, ' ');
	getline(ss, temp, ',');
	rs2 = temp;
	getline(ss, temp, '(');
	imm = stoi(temp);
	getline(ss, temp, ')');
	rs1 = temp;
	S.opcode = instructionName;
	S.imm = imm;
	S.rs1.first = rs1;
	S.rs1.second = registers[extractRegisterNumber(rs1)];
	S.rs2.first = rs2;
	S.rs2.second = registers[extractRegisterNumber(rs2)];
	cout << "Value of rs2 at store is" << S.rs2.second << endl;
	ProcessSFormat(S);

}

void readBFormat(const string& line, int& counter) {
	BFormat B;
	string instructionName, rs1, rs2, imm;
	stringstream ss(line);
	string temp;

	getline(ss, instructionName, ' ');
	getline(ss, temp, ',');
	rs1 = temp;
	getline(ss, temp, ' ');
	getline(ss, temp, ',');
	rs2 = temp;
	getline(ss, temp, ' ');
	getline(ss, temp, ' ');
	imm = temp;

	if (!imm.empty() && imm.back() == ':') {
		imm = imm.substr(0, imm.size() - 1);
	}
	B.opcode = instructionName;
	B.rs1.first = rs1;
	B.rs1.second = registers[extractRegisterNumber(rs1)];
	B.rs2.first = rs2;
	B.rs2.second = registers[extractRegisterNumber(rs2)];
	B.imm = imm;
	ProcessBFormat(B, counter);
}

void readAndProcessAssemblyFile(const string& filename) {
	ifstream file(filename);

	if (!file.is_open()) {
		cout << "Error opening file: " << filename << endl;
		return;
	}

	vector<string> lines;
	string line;
	int counter = 0;
	// First pass: collect label positions
	while (getline(file, line)) {
		istringstream iss(line);
		string firstWord;

		// Skip empty lines
		if (!(iss >> firstWord)) {
			lines.push_back("");
			continue;
		}


		// Check if the line is a label (ends with ':')
		if (firstWord.back() == ':') {
			string label = firstWord.substr(0, firstWord.size() - 1);
			// Only add the label if it hasn't been added before
			if (labels.find(label) == labels.end()) {
				counter++;
				programCounter += 4;
				labels[label] = programCounter; // Store the label with its line number
			}
		}
		else {
			counter++; // Count non-label lines for instruction counting
			programCounter += 4;

		}
		lines.push_back(line);
	}
	/*cout << "Labels map contents:" << endl;
	for (const auto& label : labels) {
		cout << "Label: " << label.first << ", Line: " << label.second << endl;
	}*/

	// Reset file and counter for the second pass
	file.clear();       // Clear EOF flag
	file.seekg(0);      // Move back to the start of the file
	counter = 1;
	programCounter = 0;
	int ProgramLineSize = lines.size() * 4;
	cout << ProgramLineSize << endl;
	while (programCounter < ProgramLineSize) {
		//cout << "PROGRAM LINE SIZE: " << ProgramLineSize << "PROGRAM COUNTER: " << programCounter << endl;
		//cout << "Entered second loop " << endl;
		line = lines[counter - 1];
		istringstream iss(line);
		string firstWord;

		// Skip empty lines
		if (!(iss >> firstWord)) {
			counter++;
			continue;
		}

		// Check if the line is a label, skip processing if true
		if (labels.find(firstWord.substr(0, firstWord.size() - 1)) != labels.end()) {
			//	cout << "First word " << firstWord.substr(0, firstWord.size() - 1) << endl;
			counter++;
			programCounter += 4;
			continue;
		}

		// Process the line as an instruction if it isn't just a label
		bool instructionProcessed = false;

		// Check for R-type instructions
		for (int i = 0; i < RInstructions.size(); i++) {
			if (firstWord == RInstructions[i]) {
				readRFormat(line);
				instructionProcessed = true;
				break;
			}
		}
		if (instructionProcessed) {
			counter++;
			continue;
		}

		for (int i = 0; i < IInstructions.size(); i++)
		{
			if (firstWord == IInstructions[i])
			{
				readIformat(line, counter);
				instructionProcessed = true;
				break;

			}
		}
		if (instructionProcessed) {
			counter++;
			continue;
		}
		// JFORMAT ;
		for (int i = 0; i < JInstructions.size(); i++)
		{
			if (firstWord == JInstructions[i])
			{
				readJFormat(line, counter);
				instructionProcessed = true;
				break;
			}
		}

		if (instructionProcessed) {
			counter++;
			continue;
		}
		// Check for S-type instructions
		for (int i = 0; i < SInstructions.size(); i++) {
			if (firstWord == SInstructions[i]) {
				readSFormat(line);
				instructionProcessed = true;
				break;
			}
		}
		if (instructionProcessed) {
			counter++;
			//cout << counter << " COUNTER " << endl;

			continue;
		}

		// Check for B-type instructions
		for (int i = 0; i < BInstructions.size(); i++) {
			if (firstWord == BInstructions[i]) {
				readBFormat(line, counter);  // Counter updated by reference
				instructionProcessed = true;
				break;
			}
		}
		if (instructionProcessed) {
			counter++;
			continue;
		}
		for (int i = 0; i < haltInstructions.size(); i++) {
			if (firstWord == haltInstructions[i]) {
				cout << "Program stopped at a halt instruction: " << firstWord << endl;
				counter++;
				programCounter += 4;
				file.close();
				return;
			}

		}
	}
	file.close();
}



int main()
{
	//string RegisterN;
	//int RegisterValue;
	int memoryAddress;
	int memoryValue;
	string type;
	int num;
	string name;

	cout << "Please enter your Name ";
	cin >> name;
	cout << "Hello" << " " << name << " " << "Welcome to Risk - V Assembler" << endl;
	cout << "Enter the number of addresses you wish to initialize" << endl;
	cin >> num;
	for (int i = 0; i < num; i++) {

		cout << "Enter the desired address" << endl;
		cin >> memoryAddress;
		cout << "Enter its value" << endl;
		cin >> memoryValue;
		memory.store(memoryAddress, memoryValue);
		cout << "Memory  Address " << memoryAddress << endl;
		//registers[extractRegisterNumber(RegisterN)] = RegisterValue;
	}
	readAndProcessAssemblyFile("Bonus_Case4.txt");
	cout << "The final Register: " << endl;
	for (int i = 0; i < registers.size(); i++) {
		cout << "x" << i << " = " << registers[i] << endl;
	}

	memory.displayMemory();
	cout << "The final program counter is: " << programCounter << endl;

	cout << "Thank you for using this Risc-V, assembly simulator!. " << endl;
	return 0;
}