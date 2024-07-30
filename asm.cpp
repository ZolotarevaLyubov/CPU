#include <iostream>
#include <string>
#include <regex>
#include <unordered_map>
#include <bitset>
#include <fstream>
#include <format>

using namespace std;
 
 
 string decimal_to_binary(int num) {
   string binary(16, '0');
   int index = 15; 
   
   while(num > 0 && index >=0){
       int remainder = num % 2;
       binary[index] = '0' + remainder;
       num /= 2;
       index--;
   }
   
   return binary;
 }

 string regex_converting(const string &mnemonic){
   unordered_map<string, string> command_map;
     command_map ["HALT"] = "0000";
     command_map ["LOAD"] = "0001";
     command_map ["STORE"] =  "0010";
     command_map ["CALL"] = "0011";
     command_map ["BR"] = "0100";
     command_map ["BREQ"] = "0101";
     command_map ["BRGE"] = "0110";
     command_map ["BRLT"] = "0111";
     command_map ["ADD"] = "1000";
     command_map ["SUB"] = "1001";
     command_map ["MUL"] = "1010";
     command_map ["DIV"] = "1011";
   
   regex pattern("^([A-Z]+)\\s+([$@=]?)(\\d+)+(\\s?)+$");   
   regex pattern_without_operand("^([A-Z]+)$");
   
   smatch matches;
   if(regex_match(mnemonic, matches, pattern)){
        string command = matches[1];
        string symbol = matches[2];
        string num = matches[3];
   
     //string binary_word;
     
        string binary_command = command_map[command];
        
     //string binary_command = bitset<4>(command[0]).to_string();
     //string binary_symbol = (symbol == "$") ? "10" : "01";
        string address_mode_binary;
        
        if(symbol == "$")
           address_mode_binary = "10";
        else if(symbol == "@")
           address_mode_binary = "11";
        else if (symbol == "=")
           address_mode_binary = "01";
        else 
           address_mode_binary = "00";    
           
        string binary_number = bitset<10>(stoi(num)).to_string();
        
        cout<<"(r_c)command: "<<command<<endl;
        cout<<"(r_c)symbol: "<<symbol<<endl; 
        cout<<"(r_c)num: "<<num<<endl;
        
        cout<<"(r_c)result: "<<binary_command+address_mode_binary+binary_number<<endl;
        
        return binary_command + address_mode_binary + binary_number;
   }
   //HALT
   else if(regex_match(mnemonic, matches, pattern_without_operand)){
          string command = matches[1];
      
          if(command == "HALT"){
             return "0000000000000000";
           } 
   }
   else 
      throw logic_error("error: regex_converting " + to_string(__LINE__));   
 }
 vector<string> split_string(const string &str){
   
  vector<string> res;
  string buffer;
  
  for(char c : str){
     if(c == ','){
       res.push_back(buffer);
       buffer.clear();
     }
     else{
       buffer += c;
     }
  }
  if(!buffer.empty()){
    res.push_back(buffer);
  }
  return res;
 }
 
 void load_program_from_file(const char *filename1, const char *filename2, vector<string> &memory) {
     //vector<string> memory;
     ifstream file(filename1);
     if(!file.is_open())
         cout<<"error: load_program_from_file"<<endl;
       
     string line;//текущая строка
     int address = 0;  //текущий адрес в памяти
     regex find_data("^(DATA\\s+(\\d)+(,\\d)+)+$");
     
     // DATA 42,13  bla-BLA
     while(getline(file,line)){
     //DATA
      
       smatch match;
       if(regex_search(line, match,find_data)){
         string data_part = line.erase(0,5);
         vector<string> numbers = split_string(data_part);
         
         for(int i = 0; i < numbers.size(); i++){
            int num = stoi(numbers[i]);
            string machine_code = decimal_to_binary(num);
            //memory[address++] = machine_code;
            
            cout<<"numbers: "<<numbers[i]<<endl;
            cout<<"machine_code: "<<machine_code<<endl;
         }
       }
       else{
           string machine_code = regex_converting(line);
           //memory[address++] = machine_code;
           memory.push_back(machine_code);
       }       
     }
     
     
     ofstream outfile(filename2);
     for(int i = 0; i < memory.size(); i++){
        outfile<<memory[i]<<endl;
     }
     outfile.close();
     
     
     file.close();
 }
 
 int main(int length, char *filename[]){
   vector<string>memory;
   
   
   load_program_from_file(filename[1], filename[2], memory);

   return 0;
 }
  
