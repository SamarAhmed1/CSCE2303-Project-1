// assambly project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <map>
#include <utility>
#include <cmath>
#include <fstream>
#include <sstream> 
using namespace std; 



map<string, int> labels;
map<int, int> memory;
vector <string> IInstructions = { "LB", "LH", "LW", "LBU", "LHU", "ADDI", "SLTI", "SLTU", "XORI", "ORI","ANDI"};
vector <string> JInstructions = { "JAL" };
vector <int> registers(32, 0); 

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

int extractRegisterNumber(const string& regName) 
{ string numStr = regName.substr(1); // Extract the numeric part after 'x'
  return stoi(numStr); // Convert the numeric part to an integer
} 

// so Jformat have only one instruction which is JAL:: its basicaly stores the address ( +4) of the instruction into rd and then update the imm 
void ProcessJformat(Jformat& instruction) {
	int valu = 0;
	if (instruction.name == "JAL") {
		instruction.rd.second = valu + 4;
		valu = valu + instruction.imm;
		cout << "JALR:" << instruction.rd.second << "jumping to adress   " << valu << endl;
	}

	int registerNumber = extractRegisterNumber(instruction.rd.first);
	registers[registerNumber] = instruction.rd.second;
}



  void ProcessIformat(Iformat& instruction, Memory&memory) {
	int baseAddr = registers[extractRegisterNumber(instruction.rs1.first)];
	int address = baseAddr + instruction.imm * 4;  // Adjusted for 4-byte addressing

	if (instruction.name == "LB") {
		instruction.rd.second = static_cast<int8_t>(memory.load(address));
		cout << "LB: Loaded byte " << instruction.rd.second << " from address " << address << std::endl;
	}
	else if (instruction.name == "LH") {
		instruction.rd.second = static_cast<int16_t>(memory.load(address));
		cout << "LH: Loaded halfword " << instruction.rd.second << " from address " << address << std::endl;
	}
	else if (instruction.name == "LW") {
		instruction.rd.second = memory.load(address);
		cout << "LW: Loaded word " << instruction.rd.second << " from address " << address << std::endl;
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
}

void readIformat(const string& line)
{
    Iformat I; 
    stringstream ss(line);
    string instructionName, rd, rs1,imm;

    getline(ss, instructionName, ' ');
    getline(ss, rd, ',');
    getline(ss, rs1, ',');
    getline(ss, imm, ' ');

    //removes commas : 
    if (!rd.empty() && rd.back() == ',')
    {
        rd = rd.substr(0, rd.size() - 1);
    }
    if (!rs1.empty() && rs1.back() == ',') 
    {
        rs1 = rs1.substr(0, rs1.size() - 1);
    }

    int immm = stoi(imm);
    I.name = instructionName;
    I.rd.first = rd; 
    I.rd.second = registers[extractRegisterNumber(rd)];
    I.rs1.first = rs1; 
    I.rs1.second= registers[extractRegisterNumber(rs1)];
    I.imm = imm;     
    processIFormat(I);
}

void readJFormat(const string& line)
{
    JFormat J;
    stringstream ss(line);
    string instructionName, rd,imm1;
   
    getline(ss, instructionName, ' ');
    getline(ss, rd, ',');
    getline(ss, imm1, ' ');
   
    if (!imm1.empty() && imm1.back() == ':') 
    {
        imm1 = imm1.substr(0, imm1.size() - 1);
    }


    J.name = instructionName;
    J.rd.first = rd;  
    J.rd.second = registers[extractRegisterNumber(rd)]
    J.imm = stoi(imm1);      

    processJFormat(J);
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
				labels[label] = counter; // Store the label with its line number
			}
		}
		else {
			counter++; // Count non-label lines for instruction counting

		}
		lines.push_back(line);
	}
	cout << "Labels map contents:" << endl;
	for (const auto& label : labels) {
		cout << "Label: " << label.first << ", Line: " << label.second << endl;
	}

	// Reset file and counter for the second pass
	file.clear();       // Clear EOF flag
	file.seekg(0);      // Move back to the start of the file
	counter = 0;
		while (counter<lines.size()) {
			cout << "Entered second loop " << endl;
			line = lines[counter];
			istringstream iss(line);
			string firstWord;

			// Skip empty lines
			if (!(iss >> firstWord)) {
				//counter++;
				continue;
			}

			// Check if the line is a label, skip processing if true
			if (labels.find(firstWord.substr(0, firstWord.size() - 1)) != labels.end()) {
				cout << "First word " << firstWord.substr(0, firstWord.size() - 1) << endl;
				counter++;
				continue;
			}

			// Process the line as an instruction if it isn't just a label
			bool instructionProcessed = false;
           //IFORMAT:
          for (int i = 0; i < IInstructions.size(); i++)
			{
				if (firstWord == IInstructions[i])
				{
					readIformat(line, memory);
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
          }
      }

      file.close();
  }




int main()
{
string RegisterN;
	int RegisterValue;
	int num;
	string name;
        cout << "Please enter your Name;";
        cin >> name;
        cout << "Hello" << "  " << name << "        " << "Welcome to Risk - V Assambler"<<endl;
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
	for (int i = 0; i < registers.size(); i++) {
		cout << "x" << i << " = " << registers[i] << endl;
	}

	memory.displayMemory();
	return 0;
}

