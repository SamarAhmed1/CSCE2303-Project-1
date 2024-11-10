
#include <iostream>

using namespace std;


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

int main()
{
   int m =5;
   int d =12;
  cout<<DecToBinary(m)<<endl;
  cout<<DecToHex(d);
    
    return 0;
    
}