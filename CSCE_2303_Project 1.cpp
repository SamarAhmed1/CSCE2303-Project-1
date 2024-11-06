#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <map>
using namespace std;

struct SFormat {
	string opcode;
	int funct3;
	int imm1;
	int rs1;
	int rs2;
	int imm2;
};

struct BFormat {
	string opcode;
	int rs1;
	int rs2;
	string imm;
};

bool ProcessSFormat(SFormat& value, int& counter) {

}

bool ProcessBFormat(BFormat& value, const map<string, int>& labels, int& counter) {
	bool Statement = false;

	// Check branch conditions based on the opcode
	if (value.opcode == "BEQ") {
		Statement = (value.rs1 == value.rs2);
	}
	else if (value.opcode == "BNE") {
		Statement = (value.rs1 != value.rs2);
	}
	else if (value.opcode == "BLT") {
		Statement = (value.rs1 < value.rs2);
	}
	else if (value.opcode == "BGE") {
		Statement = (value.rs1 >= value.rs2);
	}
	else if (value.opcode == "BLTU") {
		Statement = (static_cast<unsigned>(value.rs1) < static_cast<unsigned>(value.rs2));
	}
	else if (value.opcode == "BGEU") {
		Statement = (static_cast<unsigned>(value.rs1) >= static_cast<unsigned>(value.rs2));
	}

	if (Statement) {
		// Attempt to find the target label
		auto it = labels.find(value.imm);
		if (it != labels.end()) {
			// Set counter to the line number associated with the label
			counter = it->second;
			cout << "Branch taken to label: " << value.imm << " at line " << counter << endl;
		}
		else {
			cout << "Label not found: " << value.imm << endl;
		}
	}
	else {
		// If branch not taken, move to the next instruction
		counter++;
		cout << "Branch not taken, continuing to next instruction." << endl;
	}

	return Statement;
}

int main()
{
	// Map of labels to line numbers
	map<string, int> labels = { {"label_4", 4}, {"label_8", 10} };

	vector<BFormat> instructions = {
		{"BEQ", 5, 5, "label_4"},  // BEQ to label_4 if rs1 == rs2
		{"BNE", 1, 6, "label_8"}   // BNE to label_8 if rs1 != rs2
		// Add more instructions as needed
	};

	int counter = 0;
	while (counter < instructions.size()) {
		BFormat currentInstruction = instructions[counter];
		ProcessBFormat(currentInstruction, labels, counter);
	}

	return 0;
}
