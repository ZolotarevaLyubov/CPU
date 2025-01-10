
#include <iostream>
#include <string>
#include <regex>
#include <unordered_map>
#include <bitset>
#include <fstream>
#include <format>
#include <array>
#include <stdexcept>
#include <vector>
#include <utility>

using namespace std;
 
 
 unordered_map<string, int> label_table;
 //const int MEMORY_SIZE = 256;
 
 string decimal_to_binary(int num, int bit_width) {
   string binary(bit_width, '0');
   int index = bit_width - 1; 
   
   while(num > 0 && index >=0){
       int remainder = num % 2;
       binary[index] = '0' + remainder;
       num /= 2;
       index--;
   }
   
   return binary;
 }

 string regex_converting(const string &mnemonic,){
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
   
   regex pattern(R"(^([A-Z]+)\s+([$@=]?)((\d+)|([A-Z]+))(\s?)+$)");   
   regex pattern_without_operand("^HALT$");
   
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
               
        cout << "[regex_converting]" << __LINE__ <<endl;   
        string binary_number = bitset<10>(stoi(num)).to_string();
        
        cout<<"(r_c)command: "<<command<<endl;
        cout<<"(r_c)symbol: "<<symbol<<endl; 
        cout<<"(r_c)num: "<<num<<endl;
        
        cout<<"(r_c)result: "<<binary_command+address_mode_binary+binary_number<<endl;
        
        return binary_command + address_mode_binary + binary_number;
   }
   //HALT
   else if(regex_match(mnemonic, matches, pattern_without_operand)){
          return "0000000000000000";
           
   }
   else 
      throw logic_error("error: regex_converting " + to_string(__LINE__));   
 }
 
 vector<string> split_string(const string &str, int count){
  //array<string, MEMORY_SIZE> res;
  vector<string> res;
  string buffer;
  count = 0;
  
  for(char c : str){
     if(c == ','){
       res.push_back(buffer);
       //res[count++] = buffer;
       buffer.clear();
     }
     else{
       buffer += c;
     }
  }
  if(!buffer.empty()){
    res.push_back(buffer);
    //res[count++] = buffer;
  }
  return res;
 }
 
 
 void process_file(const char*  filename){
   ifstream file(filename);
   if (!file.is_open()){
      cerr << "unable to open file(error: process_file)"<<endl;
      return;
   }
   string line;
   while(getline(file, line)){
      if(line[0] == '*'){
        continue;
      }
      cout<<regex_converting(line)<<endl;
   }
   file.close();
 }
 
 void add_label(string label, int address) {
     label_table[label] = address;
 }
 
 int find_label(string label) {
     return label_table[label];
 }
 
 void print_label_table() {
     for(const auto entry : label_table) {
         cout << "label: " << entry.first << "  " << "address: " << entry.second << endl;
     }
 }
 
 vector<pair <string, string>> load_program_with_labels (const char *filename1) {
     vector<pair <string, string>> instructions;
     ifstream file(filename1);
     if(!file.is_open())
         cout<<"error: load_program_from_file"<<endl;
       
     string line;//текущая строка
     
     regex with_label(R"(^([A-Z]+)\s+(.+)$)");
     regex without_label(R"(^((\s)*(.+)$))");
     //regex org,data,end
     smatch match;
     int address = 0;
          
     while(getline(file, line)) {
              
         if(regex_match(line, match, with_label)) {
              string label = match[1].str();
              string command = match[2].str();
              //cout << "{ " << label << ", " << command << " }" << endl;
              instructions.emplace_back(label, command);  
              
              add_label(label, address);  
              
     }
         else if(regex_match(line, match, without_label)) {
             string command = match[1].str();
             //cout << "{ " << "\" \"" << ", " << command << " }" << endl;
             instructions.emplace_back("", command);
         }
         address++; 
         /*
         for (int i = 0;  i < match.size(); i++) {
             cout << "match: "<< match[i].str() << endl;
         }     
         */
     }
     
    
     file.close();
     return instructions;
 }  
 
 void pair_print (vector<pair <string, string>> instructions ) {
  
     for(const pair n : instructions) {
         cout << "{ " << n.first << ", " << n.second << " }" << endl;
     }
 }
 
 void load_program_from_file(const char *filename1, const char *filename2, vector<string> &object_file) {
     vector<string> instructions;
     ifstream file(filename1);
     if(!file.is_open())
         cout<<"error: load_program_from_file"<<endl;
       
     string line;//текущая строка
     int address = 0;  //текущий адрес в памяти
     regex find_data(R"(^(DATA\s+(\d)+(,\d)+)+$)");
     //regex find_org("^(ORG\\s+(\\d)+(\\s?)+$");
     regex find_org(R"(^ORG\s+(\d+)$)");
     //regex END 
     regex find_end(R"(^END\s+(\d+)$)");
     //regex find_command_with_label()
     string org_address;
      
     // DATA 42,13  bla-BLA
     while(getline(file,line)){
     //DATA
       cout << line <<endl;
       smatch match;
      
       if(regex_search(line, match,find_data)){
         //string data_part = line.erase(0,5);
         string data_part = line.substr(5);
        
         int count = 0;
         //array<string, MEMORY_SIZE> numbers = split_string(data_part, count);
          vector<string> numbers = split_string(data_part, count);
         
         for(int i = 0; i < numbers.size(); i++) {
            cout << "[load_program_from_file]" << __LINE__ <<endl;
            int num = stoi(numbers[i]);
            string machine_code = decimal_to_binary(num, 16);
            //object_file[address++] = machine_code;
            instructions.push_back(machine_code);
            
            cout<<"numbers: "<<numbers[i]<<endl;
            cout<<"machine_code: "<<machine_code<<endl;
         }
       }
       else if(regex_search(line, match, find_org)) {//ORG
            int instructions_amount = instructions.size();
            org_address = match[1];
            if(instructions_amount == 0) {
                cout << "[load_program_from_file]" << __LINE__ <<endl;
                cout << org_address <<endl;
                object_file.push_back(decimal_to_binary(stoi(org_address), 10));
            }
            else {
                object_file.push_back(decimal_to_binary(instructions_amount, 10));//количество слов
            
                for(int i = 0; i <= instructions_amount; i++) {//инструкции
                    object_file.push_back(instructions[i]);//
                    
                }
                instructions.clear();
                    
                cout << "[load_program_from_file]" << __LINE__ <<endl;
                object_file.push_back(decimal_to_binary(stoi(org_address), 10));
            }
       }
       
       else if(regex_search(line, match, find_end)) {//END
           org_address = match[1];
           object_file.insert(object_file.begin(), decimal_to_binary(stoi(org_address), 10));
       }
       
       //else if()
       
       else {
           string machine_code = regex_converting(line);
           //instructions[address++] = machine_code;
           instructions.push_back(machine_code);
           //object_file.push_back(machine_code);
       }       
     }
     
             if(instructions.size() > 0) {
             object_file.push_back(decimal_to_binary(instructions.size(), 10));
             for(int i = 0; i < instructions.size(); i++) {
                object_file.push_back(instructions[i]);
            }
            
        }    

     
     ofstream outfile(filename2);
     
     for(int i = 0; i < object_file.size(); i++) {//
        outfile<<object_file[i];
     }
     
     outfile.close();
     
     
     file.close();
 }
 
 int main(int length, char *filename[]) {
 if(length == 3) {
   vector<string>object_file;
   
   //load_program_from_file(filename[1], filename[2], object_file);
   
   //pair_print(load_program_with_labels(filename[1]));
   load_program_with_labels(filename[1]);
   print_label_table();
   
   return 0;
  }
  
 else
   throw logic_error("number of arguments is wrong"); 
  
  
  
   
 }
  
