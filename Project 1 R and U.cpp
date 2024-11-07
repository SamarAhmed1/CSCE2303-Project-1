// Project 1 R and U.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <utility>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

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
string RInstructions[10] = { "ADD", "SUB", "SLL", "SLT", "SLTU", "XOR", "SRL", "SRA", "OR", "AND" };
vector <int> registers(32, 0);

int extractRegisterNumber(const string& regName) {
    string numStr = regName.substr(1); // Extract the numeric part after 'x'
    return stoi(numStr); // Convert the numeric part to an integer
}

void processRFormat(RFormat& instruction) {
    if (instruction.name == "ADD") {
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

    else if(instruction.name=="OR"){
        instruction.rd.second = instruction.rs1.second | instruction.rs2.second;
    }

    else if (instruction.name == "AND") {
        instruction.rd.second = instruction.rs1.second & instruction.rs2.second;
    }

    int registerNumber = extractRegisterNumber(instruction.rd.first);
    registers[registerNumber] = instruction.rd.second;
}

void processUFormat(UFormat& instruction) {
    if (instruction.name == "LUI") {
        instruction.rd.second = instruction.imm * pow(2, 16);
    }

    else if (instruction.name == "AUIPC") {

    }
}

void readRFormat(const string& line) {
    RFormat R;
    istringstream iss(line);
    string instructionName, rd, rs1, rs2;
    char comma;

    iss >> instructionName >> rd >> comma >> rs1 >> comma >> rs2;

    if (iss.fail()) {
        cout << "Error parsing the instruction: " << line << endl;
        return;
    }

    R.name = instructionName;
    R.rd.first = rd;
    R.rs1.first = rs1;
    R.rs2.first = rs2;
    processRFormat(R);
}

void readAndProcessAssemblyFile(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string firstWord;

        if (iss >> firstWord) {
            for (int i = 0; i < 10; i++) {
                if (firstWord == RInstructions[i]) {
                    readRFormat(line);
                }
            }
        }
    }

    file.close();
}

int main()
{
    readAndProcessAssemblyFile("Rinstruction.txt");


   
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
