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
void ProcessJformat(Jformat& instruction)

{ 
int &valu ; 
    if (instruction.name = "JAL")
    {
        instruction.rd.second = valu + 4;
        valu = valu + instruction.imm;
        cout << "JALR:" << instruction.rd.second << "   " << valu << endl;


    }

    int registerNumber = extractRegisterNumber(instruction.rd.first);
    registers[registerNumber] = instruction.rd.second;
} 


  void ProcessIformat(Iformat& instruction)
  
  {
      int address = 0;
      
       if (instruction.name == "LB")
      {
           address = instruction.rs1.second + instruction.imm;
           instruction.rd.second = static_cast<int8_t>memory[address];
           cout << "LB:" << instruction.rs1.second + instruction.imm << " = " << instruction.rd.second << endl;
       
      } 

      else if (instruction.name == "LH")
      {
           address = instruction.rs1.second + instruction.imm;
           instruction.rd.second = static_cast<int16_t>memory[address]
               cout << "LH " << instruction.rs1.second + instruction.imm << " = " << instruction.rd.second << endl;
      }


      else if (instruction.name == "LW")
      {
          address = instruction.rs1.second + instruction.imm;
          instruction.rd.second = memory[address];
          cout << "LW:" << instruction.rs1.second + instruction.imm << " = " << instruction.rd.second << endl;
      }


      else if (instruction.name == "LBU")
      {
             address = instruction.rs1.second + instruction.imm;
             instruction.rd.second = memory[address] & 0xFF;
             cout << "LBU: " << instruction.rs1.second + instruction.imm << " = " << instruction.rd.second << endl;
           
      }


      else if (instruction.name == "LHU")
      {
            address = instruction.rs1.second + instruction.imm;
            instruction.rd.second = memory[address] & 0xFFFF;
            cout << "LHU: " << instruction.rs1.second + instruction.imm << " = " << instruction.rd.second << endl;
          
      }

      else if (instruction.name == "ADDI")
      {
       instruction.rd.second = instruction.rs1.seocnd + instruction.imm;
       cout << "ADDI: " << instruction.rs1.second << " + " << instruction.imm << " = " << instruction.rd.second << endl
    
      } 

      else if (instruction.name == "SLTI")
      {
        if (instruction.rs1.second < instruction.imm)
        {
            instruction.rd.second = 1;
        }
        else
        {
            instruction.rd.second = 0;

        }
      cout << "SLTI: " << instruction.rs1.second << " < " << instruction.imm << " = " << instruction.rd.second << endl;
      }

      else if (instruction.name == "SLTU")
    { 
        instruction.rd.second = static_cast<unsigned>(instruction.rs1.second) < static_cast<unsigned>(instruction.imm) ? 1 : 0;
        cout << "SLTU: " << instruction.rs1.second << " < " << instruction.imm << " = " << instruction.rd.second << endl;

    }

      else if (instruction.name == "XORI")
      {
  
              instruction.rd.second = instruction.rs1.second ^ instruction.imm;
              cout << "XORI: " << instruction.rs1.second << " ^ " << instruction.imm << " = " << instruction.rd.second << endl;
      
      }

      else if (instruction.name == "ORI")
      {
         
              instruction.rd.second = instruction.rs1.second | instruction.imm;
              cout << "ORI: " << instruction.rs1.second << " | " << instruction.imm << " = " << instruction.rd.second << endl; 
     
      }

      else if (instruction.name == "ANDI")
      {
        
              instruction.rd.second = instruction.rs1.second & instruction.imm;
              cout << "ANDI: " << instruction.rs1.second << " & " << instruction.imm << " = " << instruction.rd.second << endl; //JUST FOR TESTING IF THERE IS A MISTAKE. 
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
      I.imm = imm;     
      processIFormat(I);
  }

  void readJFormat(const string& line)
  {
      JFormat J;
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

      processJFormat(J);
  }
  
  void readAndProcessAssemblyFile(const string& filename)
  {
      ifstream file(filename);

      if (!file.is_open()) 
      {
          std::cerr << "Error opening file: " << filename << std::endl;
          return;
      }

      string line;
      while (getline(file, line)) 
      {
          istringstream iss(line);
          string firstWord;

          if (iss >> firstWord) 
          { 
           //IFORMAT:
          for (int i = 0; i < IInstructions.size(); i++)
          {
              if (firstWord == IInstructions[i]) 
              {
                  readIFormat(line);  
                  break;  

              }
          }
          // JFORMAT ;
          for (int i = 0; i < JInstructions.size(); i++)
          {
              if (firstWord == JInstructions[i]) 
              {
                  readJFormat(line); 
                  break; 
              }
          }
      }

      file.close();
  }




int main()
{

    // I NEED TO TEST THEM FIRST :: 
   // readAndProcessAssemblyFile("Iinstruction.txt");
   // readAndProcessAssemblyFile("Jinstruction.txt");
}
