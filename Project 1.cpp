#include <iostream>
#include <utility>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <utility>
using namespace std;

//vector<string> labels;
map<string, int> labels;
map<int, int> memory;
vector <string> RInstructions = { "ADD", "SUB", "SLL", "SLT", "SLTU", "XOR", "SRL", "SRA", "OR", "AND" };
vector <string> BInstructions = { "BEQ", "BNE", "BLT", "BGE", "BLTU", "BLGEU" };
vector <string> SInstructions = { "SW", "SH", "SB" };
vector <string> IInstructions = { "LB", "LH", "LW", "LBU", "LHU", "ADDI", "SLTI", "SLTU", "XORI", "ORI","ANDI", "SLLI", "SRLI", "SRAI"};
vector <string> JInstructions = { "JAL" };
vector <string> UInstructions = { "LUI", "AUIPC" };
vector <string> haltInstructions = {"ECALL", "EBREAK", "PAUSE", "FENCE", "FENCE.TSO"};
vector <int> registers(32, 0);

struct SFormat {
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
	int imm;
};

struct Iformat
{
	string name;
	//int opcode[7];
	int imm;
	pair <string, int> rs1;
	pair <string, int> rd;
};


int extractRegisterNumber(const string& regName) {
	string numStr = regName.substr(1); // Extract the numeric part after 'x'
	return stoi(numStr); // Convert the numeric part to an integer
}

void ProcessJformat(Jformat& instruction)

{
	int valu=0; 
	if (instruction.name == "JAL")
	{
		instruction.rd.second = valu + 4;
		valu = valu + instruction.imm;
		cout << "JALR:" << instruction.rd.second << "   " << valu << endl;


	}

	int registerNumber = extractRegisterNumber(instruction.rd.first);
	registers[registerNumber] = instruction.rd.second;
	for (int i = 0; i < registers.size(); i++) {
		cout << "x" << i << " = " << registers[i] << endl;
	}
}


void processRFormat(RFormat& instruction) {
	cout << "In process R" << endl;
	if (instruction.name == "ADD") {
		cout << "In ADD" << endl;
		cout << instruction.rs1.second << " " << instruction.rs2.second;
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
	cout << registers[registerNumber] << endl;
	for (int i = 0; i < registers.size(); i++) {
		cout << "x" << i << " = " << registers[i] << endl;
	}
}

void processUFormat(UFormat& instruction) {
	if (instruction.name == "LUI") {
		instruction.rd.second = instruction.imm * pow(2, 16);
	}

	/*else if (instruction.name == "AUIPC") {

	}*/
	int registerNumber = extractRegisterNumber(instruction.rd.first);
	registers[registerNumber] = instruction.rd.second;
	for (int i = 0; i < registers.size(); i++) {
		cout << "x" << i << " = " << registers[i] << endl;
	}
}
void ProcessBFormat(BFormat& value, int& counter) {
	bool Statement = false;

	// Extract integer values for comparison
	int rs1_value = value.rs1.second;
	int rs2_value = value.rs2.second;

	// Check branch conditions based on the opcode
	if (value.opcode == "BEQ") {
		Statement = (rs1_value == rs2_value);
	}
	else if (value.opcode == "BNE") {
		Statement = (rs1_value != rs2_value);
	}
	else if (value.opcode == "BLT") {
		Statement = (rs1_value < rs2_value);
	}
	else if (value.opcode == "BGE") {
		Statement = (rs1_value >= rs2_value);
	}
	else if (value.opcode == "BLTU") {
		Statement = (static_cast<unsigned>(rs1_value) < static_cast<unsigned>(rs2_value));
	}
	else if (value.opcode == "BGEU") {
		Statement = (static_cast<unsigned>(rs1_value) >= static_cast<unsigned>(rs2_value));
	}
	if (Statement) {
		if (labels.find(value.imm) != labels.end()) {
			counter = labels[value.imm];
			cout << "Branch taken to label: " << value.imm << " at line " << counter << endl;
		}
		else {
			cout << "Label not found: " << value.imm << endl;
			// Handle the situation gracefully, maybe continue execution or terminate gracefully
		}
	}
	else {
		cout << "Branch not taken, continuing to next instruction." << endl;
	}
}

void ProcessSFormat(SFormat& value) {
	// Calculate the effective address by adding immediate to rs1
	int address = registers[extractRegisterNumber(value.rs1.first)] + value.imm;

	// Perform the appropriate store operation based on the opcode
	if (value.opcode == "SB") {
		memory[address] = value.rs2.second & 0xFF;  // Store only the least significant byte
		cout << "Stored Byte at address " << address << " with value " << (value.rs2.second & 0xFF) << endl;
	}
	else if (value.opcode == "SH") {
		memory[address] = value.rs2.second & 0xFFFF;  // Store the least significant 2 bytes
		cout << "Stored Halfword at address " << address << " with value " << (value.rs2.second & 0xFFFF) << endl;
	}
	else if (value.opcode == "SW") {
		memory[address] = value.rs2.second;  // Store the full 4-byte word
		cout << "Stored Word at address " << address << " with value " << value.rs2.second << endl;
	}
	else {
		cout << "Unknown S-format opcode: " << value.opcode << endl;
	}
}

void ProcessIformat(Iformat& instruction) {
	int address = 0;
	if (instruction.name == "LB") {
		address = instruction.rs1.second + instruction.imm;
		instruction.rd.second = static_cast<int8_t>(memory[address]);
		cout << "LB:" << instruction.rs1.second + instruction.imm << " = " << instruction.rd.second << endl;
	}
	else if (instruction.name == "LH") {
		address = instruction.rs1.second + instruction.imm;
		instruction.rd.second = static_cast<int16_t>(memory[address]);
		cout << "LH " << instruction.rs1.second + instruction.imm << " = " << instruction.rd.second << endl;
	}
	else if (instruction.name == "LW") {
		address = instruction.rs1.second + instruction.imm;
		instruction.rd.second = memory[address];
		cout << "LW:" << instruction.rs1.second + instruction.imm << " = " << instruction.rd.second << endl;
	}
	else if (instruction.name == "LBU") {
		address = instruction.rs1.second + instruction.imm;
		instruction.rd.second = memory[address] & 0xFF;
		cout << "LBU: " << instruction.rs1.second + instruction.imm << " = " << instruction.rd.second << endl;
	}
	else if (instruction.name == "LHU") {
		address = instruction.rs1.second + instruction.imm;
		instruction.rd.second = memory[address] & 0xFFFF;
		cout << "LHU: " << instruction.rs1.second + instruction.imm << " = " << instruction.rd.second << endl;
	}
	else if (instruction.name == "ADDI") {
		instruction.rd.second = instruction.rs1.second + instruction.imm;
		cout << "ADDI: " << instruction.rs1.second << " + " << instruction.imm << " = " << instruction.rd.second << endl;
	}
	else if (instruction.name == "SLTI") {
		instruction.rd.second = instruction.rs1.second < instruction.imm ? 1 : 0;
		cout << "SLTI: " << instruction.rs1.second << " < " << instruction.imm << " = " << instruction.rd.second << endl;
	}
	else if (instruction.name == "SLTU") {
		instruction.rd.second = static_cast<unsigned int>(instruction.rs1.second) < static_cast<unsigned int>(instruction.imm) ? 1 : 0;
		cout << "SLTIU: " << instruction.rs1.second << " < " << instruction.imm << " = " << instruction.rd.second << endl;
	}
	else if (instruction.name == "XORI") {
		instruction.rd.second = instruction.rs1.second ^ instruction.imm;
		cout << "XORI: " << instruction.rs1.second << " ^ " << instruction.imm << " = " << instruction.rd.second << endl;
	}
	else if (instruction.name == "ORI") {
		instruction.rd.second = instruction.rs1.second | instruction.imm;
		cout << "ORI: " << instruction.rs1.second << " | " << instruction.imm << " = " << instruction.rd.second << endl;
	}
	else if (instruction.name == "ANDI") {
		instruction.rd.second = instruction.rs1.second & instruction.imm;
		cout << "ANDI: " << instruction.rs1.second << " & " << instruction.imm << " = " << instruction.rd.second << endl;
	}
	else if (instruction.name == "SLLI") {
		instruction.rd.second = instruction.rs1.second * pow(2, instruction.imm);
	}
	else if (instruction.name == "SRLI") {
		if (instruction.rs1.second < 0)
			instruction.rs1.second = abs(instruction.rs1.second);
		if (instruction.imm < 0)
			instruction.imm = abs(instruction.imm);
		instruction.rd.second = instruction.rs1.second / pow(2, instruction.imm);
	}
	else if (instruction.name == "SRAI") {
		instruction.rd.second = instruction.rs1.second / pow(2, instruction.imm);
	}
	else {
		cout << "Unknown I-format opcode: " << instruction.name << endl;
	}
	int registerNumber = extractRegisterNumber(instruction.rd.first);
	registers[registerNumber] = instruction.rd.second;
	for (int i = 0; i < registers.size(); i++) {
		cout << "x" << i << " = " << registers[i] << endl;
	}
}

void readIformat(const string& line)
{
	Iformat I;
	istringstream iss(line);
	string instructionName, rd, rs1;
	char comma;
	int imm;

	iss >> instructionName >> rd >> comma >> rs1 >> comma >> imm;

	if (iss.fail())
	{
		cout << "Error parsing the instruction: " << line << endl;
		return;
	}

	I.name = instructionName;
	I.rd.first = rd;
	I.rs1.first = rs1;
	I.rs1.second = registers[extractRegisterNumber(rs1)];
	I.imm = imm;
	ProcessIformat(I);
}

void readJFormat(const string& line)
{
	Jformat J;
	istringstream iss(line);
	string instructionName, rd;
	char comma;
	int imm;

	iss >> instructionName >> rd >> comma >> imm;

	if (iss.fail())
	{
		cout << "Error parsing the instruction: " << line << endl;
		return;
	}

	J.name = instructionName;
	J.rd.first = rd;
	J.imm = imm;

	ProcessJformat(J);
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
	getline(ss, rs1, ')');
	S.opcode = instructionName;
	S.imm = imm;
	S.rs1.first = rs1;
	S.rs1.second = registers[extractRegisterNumber(rs1)];
	S.rs2.first = rs2;
	S.rs2.second = registers[extractRegisterNumber(rs2)];
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

	string line;
	int counter = 0;
	// First pass: collect label positions
	while (getline(file, line)) {
		cout << "Entered loop " << endl;
		istringstream iss(line);
		string firstWord;

		// Skip empty lines
		if (!(iss >> firstWord)) {
			continue;
		}

		// Check if the line is a label (ends with ':')
		if (firstWord.back() == ':') {
			string label = firstWord.substr(0, firstWord.size() - 1);
			labels[label] = counter; // Store the label with its line number
		}
		else {
			counter++; // Count non-label lines for instruction counting

		}
	}

	// Reset file and counter for the second pass
	file.clear();       // Clear EOF flag
	file.seekg(0);      // Move back to the start of the file
	counter = 0;
	while (getline(file, line)) {
		cout << "Entered second loop " << endl;

		istringstream iss(line);
		string firstWord;

		// Skip empty lines
		if (!(iss >> firstWord)) {
			continue;
		}

		// Check if the line is a label, skip processing if true
		if (labels.find(firstWord.substr(0, firstWord.size() - 1)) != labels.end()) {
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
				readIformat(line);
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
				readJFormat(line);
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
			continue;
		}

		// Check for B-type instructions
		for (int i = 0; i < BInstructions.size(); i++) {
			if (firstWord == BInstructions[i]) {
				readBFormat(line, counter);  // counter passed by reference
				instructionProcessed = true;
				break;
			}
		}

		for (int i = 0; i < haltInstructions.size(); i++) {
			if (firstWord == haltInstructions[i]) {
				cout << "Program stopped at: " << firstWord;
				file.close();
				return;
			}
		}

		counter++; // Increment for each non-label, non-empty line
	}
	
}



/*	while (getline(file, line)) {
		istringstream iss(line);
		string firstWord;

		if (iss >> firstWord) {
			for (int i = 0; i < 10; i++) {
				if (firstWord == RInstructions[i]) {
					readRFormat(line);
				}
			}
			for (int i = 0; i < 6; i++) {
				if (firstWord == BInstructions[i]) {
					readBFormat(line);
				}
				for (int i = 0; i < 3; i++) {
					if (firstWord == SInstructions[i]) {
						readSFormat(line);
					}
				}
			}
		}
	}
}
file.close();
}*/


int main()
{
	string RegisterN;
	int RegisterValue;
	int num;
	cout << "Enter the number of registers you wish to initialize" << endl;
	cin >> num;
	for (int i = 0; i < num; i++) {
		cout << "Enter the desired name for the registers" << endl;
		cin >> RegisterN;
		cout << "Enter its value" << endl;
		cin >> RegisterValue;
		registers[extractRegisterNumber(RegisterN)] = RegisterValue;
	}
	readAndProcessAssemblyFile("Assembly_Test.txt");
	
	return 0;
}