#include <iostream>
#include <string>
#include <regex>
#include <unordered_map>
#include <bitset>
#include <fstream>
#include <format>
#include <vector>
#include <array>
#include <stdexcept>
#include <sstream>

using namespace std;


 const int MEMORY_SIZE = 1024;
/*
 11 
 101
+ 11
----
1000


11111111
10111001
--------
01000110 

*/ 
 
// "101"  "011"
// "100" + "100" -> "1000"
                 //-> "000"


 string addition(string a, string b) {
  if(a.size() != b.size())
    throw invalid_argument("error: different size_A");
  else {
  string res(a.size(), '|');
  int carry_bit = 0;//перенос единицы
  
  for(int i = a.size() - 1; i >= 0; i--) {
   int sum = (a.at(i) - '0') + (b.at(i) - '0') + carry_bit;
   if(sum > 1){
     carry_bit = 1;   
     sum -= 2;  
   }
   else {
     carry_bit = 0;
   }     
   
   res.at(i) = '0' + sum;            
  }
   return res; 
  }
 }
 
 string complement_code(string a);
 int binary_to_decimal(string binary);
 
 string subtraction(string a, string b) {
  if(a.size() != b.size())
    throw invalid_argument("error: different size_S");
  else{
    string res(a.size(), '|');
    //int borrow = 0;
     cout<<"a = "<<a<<endl;
     cout<<"complement_code b =  "<<complement_code(b)<<endl;
 
     res = addition(a, complement_code(b));
    /*       
    for(int i = a.size() - 1; i >= 0; i--){
       int difference = (a.at(i) - '0') - (b.at(i) - '0') - borrow;
       if(difference < 0){
         borrow = 1;
         difference += 2;
       }
       else{
         borrow = 0;
       }     
       res.at(i) = '0' + difference;
    }
  */
      
    return res;
  }     
  
 }
 
 string increment(string a) {
 // string one = "001";
 //  return addition(a, one);
  int n = a.size();
  int i = n-1;
  
  while(i >= 0 && a.at(i) == '1') {
   a.at(i) = '0';
   i--;
  }
  if(i >= 0){
   a.at(i) = '1';
  }
  return a;
  
 }
 
 string decrement(string a) {
    int n = a.size();
  int i = n-1;
  
  while(i >= 0 && a.at(i) == '0') {
   a.at(i) = '1';
   i--;
  }
  
  if(i >= 0) {
   a.at(i) = '0';
  }
  else {
   a = '0' + a;
  }
  return a;
   
 }
/* 
"42" * '3'
 
0042
0013
  --
 126
 420
 ---
 546
 
 "0000000000001010"
 */
 string multiply_digit(string num, int multiplier) {
   string res(num.size(), '0');
   
   for(int i = num.size() - 1; i >= 0; i--) {
      //res.at(i) = (num.at(i) * multiplier);
      int digit = num.at(i) - '0';
      int product = digit * multiplier;
      res.at(i) = '0' + (product % 2);
   }
   
   return res;
 }
 
 string shift_left(string num, int shift) {
     if(num.at(0) == '0')
         return num.substr(shift % num.size()) + num.substr(0, shift % num.size());  
    //int actual_shift = shift % num.size();
    //char fill_char = (num.at(0) = '1')? '1' : '0';
    //cout << "fill_char: " << fill_char <<endl;
    //string shifted = num.substr(actual_shift) + string(actual_shift,fill_char);
    
     //shifted = shifted.substr(0, num.size());
    //return shifted;
     return num.substr(shift % num.size()) + num.substr(1, shift % num.size());  
 }
 
 
/*
"124"
"400"
tutorial hell

 */
 
 /*
string multiply(string a, string b) {
 if(a.size() != b.size())
   throw invalid_argument("error: different size_M");
 else {
   string res(a.size(), '0');
   
   for(int i = b.size() - 1; i >= 0; i--) {
       int digit = b.at(i) - '0';
       string l = multiply_digit(a,digit);
       l = shift_left(l, b.size() - 1 - i);
       res = addition(res, l);
   }
   return res;
   }
   
}
 */
 
string multiply(string a, string b) {
 if(a.size() != b.size())
   throw invalid_argument("error: different size_M");
 else {
   string res(a.size(), '0');
 
   for(int i = b.size() - 1; i >= 0; i--) {
       int digit = b.at(i) - '0';
       string l = multiply_digit(a,digit);
       l = shift_left(l, b.size() - 1 - i);
       res = addition(res, l);
   }
  
   cout << "Multiply result: " << res <<endl;
   
   return res;
   }
   
}
 
 string division(string dividend, string divisor) {
   string quotient(dividend.size(), '0');//частное
   string remainder = dividend;//остаток
   //string remainder = dividend.substr(0, divisor.size() - 1);
   
   for(int i = 0; i < dividend.size(); i++) {
      //remainder += dividend[i];
      remainder = remainder.substr(1) + dividend.at(i);
      
      if(remainder.size() < divisor.size()) { 
        int shift_amount = divisor.size() - remainder.size();
        shift_left(remainder, remainder.size() - shift_amount);
      }
      
      if(remainder >= divisor) {
        remainder = subtraction(remainder, divisor);
        quotient.at(i) = '1';
        //quotient[i - (divisor.size() - 1)] = '1';
      }
     
   }
   
   return quotient;
 }
 
// LISP - 20 + - * / if define, lambda, first,rest, cons, null
 
  int binary_to_decimal_base(string binary){
    int decimal = 0;
    int power = 1;
    
    for(int i = binary.size() - 1; i >= 0; --i){
       if(binary.at(i) == '1'){
         decimal += power;
       }
    power *= 2;
    }
    return decimal;
  }
  
  int binary_to_decimal(string binary){
    int res;
    if(binary.at(0) == '1'){
      binary = complement_code(binary);
      res = binary_to_decimal_base(binary);
      res = -res;  
    }
    else
      res = binary_to_decimal_base(binary);
    return res;  
  }
  
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
   
 string complement_code(string a){
   string res = a;
   
   for(int i = res.size() - 1; i >= 0; i--){
      if(res.at(i) == '0'){
        res.at(i) = '1';
      }
      else
        res.at(i) = '0';
   }
    //return res;
   return increment(res);
 }
 
 /*
 числа фибоначчи
 1,1,2,3,5,8
 1,2,3,4,5,6
 fib(6)=8
 */
 
 void complement_sub(string minuend, string subtrahend){
   
   string res = addition(minuend, complement_code(subtrahend));
   cout<<"res: "<<res<<endl;
   if(res.at(0) == '1'){
     cout<<"complement_code: "<<complement_code(res)<<endl;
     res = complement_code(res);     
     cout<<"complement_sub: "<<"-"<<binary_to_decimal(res)<<endl; 
   }
   else
     cout<<"complement_sub: "<<binary_to_decimal(res)<<endl;
     
 }
 
 string address_field(string a){
   return a.substr(6, 10);   
 }
 
 string address_mode_field(string a){
   return a.substr(4,2);
 }
 
 string operation_field(string a){
   return a.substr(0, 4);
 }
 
 void binary_to_mnemonic(string binary){
   string operation = operation_field(binary);
   int num = stoi(address_field(binary), NULL, 2);
   if(operation == "0000"){cout<<"HALT"<<endl;}
   else if(operation == "0001"){cout<<"(LOAD"<<" "<<num<<")";}//
   else if(operation == "0010"){cout<<"(STORE"<<" "<<num<<")";}//
   else if(operation == "0011"){cout<<"(CALL"<<" "<<num<<")";}
   else if(operation == "0100"){cout<<"(BR"<<" "<<num<<")";}
   else if(operation == "0101"){cout<<"(BREQ"<<" "<<num<<")";}
   else if(operation == "0110"){cout<<"(BRGE"<<" "<<num<<")";}
   else if(operation == "0111"){cout<<"(BRLT"<<" "<<num<<")";}
   else if(operation == "1000"){cout<<"(ADD"<<" "<<num<<")";}//
   else if(operation == "1001"){cout<<"(SUB"<<" "<<num<<")";}//
   else if(operation == "1010"){cout<<"(MUL"<<" "<<num<<")";}//
   else if(operation == "1011"){cout<<"(DIV"<<" "<<num<<")";}
 }
 
 string highlight_ir(string ir){
   ir.insert(4, "\033[1;32m");
   ir.insert(9+4, "\033[0m");
   return ir;
 }
 
 string &read_from_memory(array<string, MEMORY_SIZE> &memory, string program_counter){
   int index = stoi(program_counter, NULL, 2);
   
   if(index < 0 || index >= 1024){
     throw invalid_argument("error: program counter is out of range");
   }
   return memory[index];
 }
 
 
 /*
0: LOAD   3
1: ADD  @4
2: HALT
3: 42 
4: 5
5: 13
*/
//^[A-Z]+\s+[\$\@]?\d+$
// B @2
// AS    @35
// LOAD 42
// DATA 13,42,666     -> "13","42","666"
 
 
 //"12,42,67" -> "12","42","67"
 
 void reading_outfile(array<string, MEMORY_SIZE> &memory, const char *filename) {
     ifstream file(filename);
     string line;
     int address = 0;
     
     if(!file) {
         cout << "error: reading_outfile " << __LINE__ <<endl;     
     }
     
     stringstream buffer;
     buffer << file.rdbuf();
     string commands = buffer.str();
     cout << commands <<endl;
     
     int origin = stoi(commands.substr(0,10), NULL, 2);
     int words_amount = stoi(commands.substr(10,10), NULL, 2);
     int bit_position = 20;
     address = origin;
      
     for(int i = 0; i <= words_amount; i++) {
     
        string command = commands.substr(bit_position, 16);
        memory[address] = command;
        address++;
        bit_position += 16;     
     }
     
     cout << "ORIGIN: " << origin <<endl;
     cout << "WORDS AMOUNT: " << words_amount <<endl;
     
     file.close();              
 }

 void main_loop(array<string, MEMORY_SIZE> &memory){
//  string arr[1024];
  /*
  for(int i = 0; i <= 1024; i++){
     arr[i] = "0000000000000000";     
  }     
  
  arr[3] = "0000000000101010"; // 42 
  arr[5] = "0000000000001101";// 13
  arr[0] = "0001000000000011";//LOAD 3
  //arr[1] = "1000000000000011";
  arr[1] = "1000110000000100";//ADD @4
  arr[2] = "0000000000000000";//HALT
  arr[4] = "0000000000000101";//
  
  //проверка br,breq,brge
  
  arr[1] = "0100010000001000";
  arr[2] = "0101010000001000";
  arr[5] = "0110010000001000";
  arr[6] = "0000000000000000";
  arr[7] = "0000000000000000";
  arr[8] = "0000000000000000";
  */
  
  string program_counter = "0000000000";
  string accumulator = "0000000000000000";
  string instruction_register = "0000000000000000";
  
  string mar = "0000000000";
  string mbr = "0000000000000000";
  
  for(int i = 0; i == 0; ){
  instruction_register = read_from_memory(memory, program_counter);//
  string code = operation_field(instruction_register);
  string code_address_mode_field = address_mode_field(instruction_register);
  
  if(code_address_mode_field == "00"){
    mar = address_field(instruction_register);
  }
  else if(code_address_mode_field == "01"){
    mar = address_field(instruction_register);
    mbr = "000000" +  mar ;
    
  }
  else if (code_address_mode_field == "10"){
    cout<<"indexed mode of addressing is not implemented"<<endl;
  }
  else if (code_address_mode_field == "11"){
    mar = address_field(instruction_register);
    mbr = read_from_memory(memory, mar);//
    mar = address_field(mbr);
  }
  
    program_counter = increment(program_counter);
  
  if(code == "0000"){
    //HALT
    i++;
  }
  else if(code == "0001"){//возьмем поле адреса и значение по этому адресу из памяти, запишем в аккумулятор
    //LOAD
    
    if(code_address_mode_field != "01"){
      mbr = read_from_memory(memory, mar);//
    }
    accumulator = mbr;

  }
  else if(code == "0010"){//берет аккумулятор и записывает его значение туда куда указывает поле адреса
    //STORE
    mbr = accumulator;
    read_from_memory(memory, mar) = mbr;//
  }
  else if(code == "0011"){
    //CALL
  }
  else if(code == "0100"){
    //BR    
    program_counter = mar;
  }
  else if(code == "0101"){
    //BREQ    
    if (accumulator == "0000000000000000")
      program_counter = mar;
  }
  else if(code == "0110"){
    //BRGE
    if(accumulator.at(0) == '0'){
      program_counter = mar;
    }
  }
  else if(code == "0111"){
    //BRLT
    if(accumulator.at(0) == '1')
      program_counter = mar;
  }
  else if(code == "1000"){//берет поле адреса, идет в память и получает значение по этому адресу,
  //складывает с аккумулятором и записывает в аккумулятор
    //ADD
    if(code_address_mode_field != "01"){
      mbr = read_from_memory(memory, mar);//
    }
    accumulator = addition(accumulator, mbr);
    
  }
  else if(code == "1001"){
    //SUB
    if(code_address_mode_field != "01"){
      mbr = read_from_memory(memory, mar);//
    }
    //accumulator = subtraction(accumulator, mbr);
    
    accumulator = addition(accumulator, complement_code(mbr));
  }
  else if(code == "1010"){
    //MUL
    if(code_address_mode_field != "01"){
      mbr = read_from_memory(memory, mar);//
    }
    accumulator = multiply(accumulator, mbr);
    
  }
  else if(code == "1011"){
    //DIV
  }
  else{
    throw logic_error("error: exception at line " + to_string(__LINE__));
  }

  cout<<"IR: "<<highlight_ir(instruction_register)<<" ";
  binary_to_mnemonic(instruction_register);
  
  cout<<"PC: "<<program_counter<<"("<<stoi(program_counter, NULL,2)<<")";
  cout<<"AC: "<<accumulator<<"("<<binary_to_decimal(accumulator)<<")"<< endl;
  }
  cout<<"accumulator: "<<accumulator<<endl;
 }
 
 int main( int length, char *filename[]) {
  //cout << binary_to_decimal(multiply("11010", "00011")) << endl;//-5*3
  //cout << "Decimal res: " << binary_to_decimal("110000") <<endl;
  //cout<<binary_to_decimal(complement_code(complement_code(decimal_to_binary(55, 16))))<<endl;
  
  cout << binary_to_decimal(multiply(decimal_to_binary(5, 16), decimal_to_binary(2, 16))) << endl;
  
  //cout<<division("1001", "0011")<<endl;
  //cout<<factorial("0001000")<<endl;
  /*
  string a = decimal_to_binary(373);
  string b = decimal_to_binary(185);
  string res = addition(a, complement_code(b));
  cout<<"binary_res: "<<res<<endl;
  cout<<"decimal_res: "<<binary_to_decimal(res)<<endl;
  */
  
  //main_loop();
  //cout<<regex_converting("HALT")<<endl;
  
  //process_file(filename[1]);
  
  
  //vector<string>memory(1024, "0000000000000000");
  
  
  //array<string, MEMORY_SIZE> memory;
  //reading_outfile(memory, filename[1]);
  /*
  main_loop(memory);
  
  for(int i = 0; i < 15; i++){
     if(!memory[i].empty()){
        cout<<memory[i]<<endl;
     }
  }
  */
  
  //complement_sub(decimal_to_binary(4),decimal_to_binary(5));
  
  //тест
  /*
  for(int i = 0; i < length; i++){
     cout<<filename[i]<<endl;
  } 
  */    
  //cout<<decimal_to_binary(14)<<endl;
  //cout<<binary_to_decimal("101101011")<<endl;
  
  /*
  vector<string> res = split_string("1,2,3,4,5");
  for(int i = 0; i < 5; i++){
     cout<<res[i]<<endl;
  }
  */
  
  return 0;
 }

