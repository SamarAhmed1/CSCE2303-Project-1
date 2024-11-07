// assambly project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <map>

using namespace std; 


struct Jformat

{   
    string name;
    int opcode[7]; 
    pair <string, int> rd;
    int imm;
   
};

// so Jformat have only one instruction which is JAL:: its basicaly stores the address ( +4) of the instruction into rd and then update the imm 
void ProcessJformat(Jformat& instruction , int& valu)

{
    if (instruction.name = "JAL") 
    {
        instruction.rd.second = valu + 4;
        valu = valu + instruction.imm;

    } 


 struct Iformat
    {
        string name;
        int opcode[7];
        int imm;
        pair <string, int> rs1;
        pair <string, int> rd;
    };

  void ProcessIformat(Iformat& instruction)
  
  {
      /*
       if (instruction.name == "LB")
      {
           for (int i = 0; i < 8; i++)
           { 
         instruction.rd[i].second = instruction.rs1[i] + instruction.imm[i];
           }
      } 

      else if (instruction.name == "LH")
      {
           for (int i = 0; i < 16; i++)
           {
               instruction.rd[i].second = instruction.rs1[i] + instruction.imm[i];
           }
      }


      else if (instruction.name == "LW")
      {
           instruction.rd[i].second = instruction.rs1[i] + instruction.imm[i];
      }


      else if (instruction.name == "LBU")
      {
           for (int i = 0; i < 8; i++)
           {
               instruction.rd[i].second = instruction.rs1[i] + instruction.imm[i];
           }
      }


      else if (instruction.name == "LHU")
      {
           for (int i = 0; i < 16; i++)
           {
               instruction.rd[i].second = instruction.rs1[i] + instruction.imm[i];
           }
      }

    /*

      else if (instruction.name == "ADDI")
      {
        for (int i = 0; i < 32; i++)
        {
            instruction.rd[i].second = instruction.rs1[i] + instruction.imm[i];
        }
      } 

    
    MAKE SURE FROM THE DR ABOUT THE FORUMALA:::: 
      else if (instruction.name == "SLTI")
      {
        if (instruction.rs1.second < instruction.imm)
        {
            instruction.rs1.second = 1;
        }
        else
        {
            instruction.rs1.second = 0;

        }

    }
      }


    make sure from the doctor
      else if (instruction.name == "SLTU")
    {
        if (instruction.rs1.second < instruction.imm)
        {
            instruction.rs1.second = 1;
        }
        else
        {
            instruction.rs1.second = 0;

        }

    }

   


      else if (instruction.name == "XORI")
      {
          for (int i = 0; i < 32; i++)

          {
              instruction.rd[i].second = instruction.rs1[i] ^ instruction.imm[i];
          }
      }

      else if (instruction.name == "ORI")
      {
          for (int i = 0; i < 32; i++)
          {
              instruction.rd[i].second = instruction.rs1[i] | instruction.imm[i];
          }
      }

      }
    else if (instruction.name == "ANDI")
      {
          for (int i = 0; i < 32; i++)
          {
              instruction.rd[i].second = instruction.rs1[i] & instruction.imm[i];
          }

      }

     
*/

int main()
{
    std::cout << "Hello World!\n";
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
