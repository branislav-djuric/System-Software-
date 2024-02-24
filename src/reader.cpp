#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <bits/stdc++.h>
#include "Asembler.h"
 
using namespace std;
int main(int argc,char* argv[]){

  std::ifstream file(argv[3]);
  std::string str;
  FILE *fileOutput;
  string outputName = "izlaz.txt";
  outputName = argv[2];
  bool comment = false;
  Asembler* asemb= new Asembler();
  asemb->initTable();
  while (std::getline(file, str)) {
    vector<string> v;
     std::stringstream  data(str);

    std::string line;
    bool space = false;
    bool empt = false;
    std::string last="";

    while(std::getline(data,line,' '))
    {
      
        if(line!=""){
        v.push_back(line); 
        }
                  
    }
 
    if(line=="" && !space && v.size()==0){
      continue;
    } 
    std::string instr = v[0];
    space = false;

  
    if(instr=="halt"){
      //psHalt();
      asemb->psHalt();
    }
    else if(instr=="int"){
      std::string reg1;
      reg1 = v[1];
      std::string broj1 = reg1.substr(1);
      int num1 = stoi(broj1);
      asemb->psInt(num1);
    }
    else if(instr=="iret"){
      //psIret()
      asemb->psIret();
    }
    else if(instr=="ldr"){
      std::string operand = v[2];
      std::string symb;
      //v[1] je sigurno neki registar
      std::string operand1 = v[1].substr(1,v[1].size()-2);
      int op1 = stoi(operand1);
      int literal;
        if(operand[0]=='$'){
          if((operand[1]>'a' && operand[1]<'z') || (operand[1]>'A' && operand[1]<'Z')){
            symb = operand.substr(1);
            asemb->psLdr(op1,-1,-1, 0, symb);
          }
          else{
            if(operand[1]=='0' && operand[2]=='x'){
              unsigned int x;   
              std::stringstream ss;
              ss << std::hex << operand.substr(1);
              ss >> x;
             asemb->psLdr(op1,x ,-1, 0, "hex");
            }
            else{
            literal = stoi(operand.substr(1));
            asemb->psLdr(op1,literal,-1 , 0, "lit");
            }
          }
        }
        else
        if(operand=="r0" || operand=="r1" || operand=="r2"|| operand=="r3" || operand=="r4"|| operand=="r5"
        || operand=="r6"|| operand=="r7"|| operand=="r8"||operand=="r9"||operand=="r10" || 
        operand=="r11" || operand=="r12"|| operand=="r13"|| operand=="r14"|| operand=="r15" || operand=="r16"){
        std::string broj = operand.substr(1);
        int num = stoi(broj);
        asemb->psLdr(op1,num, -1 , 1, "reg");

          }
        else
        if(operand[0]=='%'){
          std::string sym = v[2].substr(1);
          asemb->psLdr(op1,-1,-1 , 2, sym);
        }
        else
        if(operand[0]=='['){
         bool space = false;
          if(operand[operand.size()-1]!=']'){
            space = true;
          }
          if(space){
           
            int reg1= stoi(v[2].substr(2));
            std::string tip = v[4].substr(0, 1);
            if(tip=="0" || tip=="1" || tip=="2"|| tip=="3" || tip=="4"|| tip=="5"
        || tip=="6"|| tip=="7"|| tip=="8"||tip=="9"||tip=="r10"){
            int literal = stoi(v[4].substr(0,v[4].size()-1));

            asemb->psLdr(op1,reg1,literal , 3, "lit");

        }
        else if(v[4][0]=='0' && v[4][1]=='x'){
           unsigned int x = std::stoul(v[3], nullptr, 16);
             asemb->psLdr(op1,reg1, x , 3, "hex");
        }
        else{
          std::string symb = v[4].substr(0, v[4].size()-1);
          //neki registar na koji dodajemo
          asemb->psLdr(op1,reg1,-1 , 3, symb);
        }
          }
          else{
          std::string type = operand.substr(1, operand.size()-2);
          if(type=="r0" || type=="r1" || type=="r2"|| type=="r3" || type=="r4"|| type=="r5"
        || type=="r6"|| type=="r7"|| type=="r8"||type=="r9"||type=="r10" || 
        type=="r11" || type=="r12"|| type=="r13"|| type=="r14"|| type=="r15" || type=="r16"){
         
        std::string broj = operand.substr(2,operand.size()-1);
        int num = stoi(broj);
        asemb->psLdr(op1,num,-1 , 4, "reg");
          }
          }
        }
        else{
            bool jednoslovan = false;
            std::string tip = v[2].substr(0, 1);
            if(tip=="0" || tip=="1" || tip=="2"|| tip=="3" || tip=="4"|| tip=="5"
        || tip=="6"|| tip=="7"|| tip=="8"||tip=="9"){
            int literal = stoi(v[3]);

            asemb->psLdr(op1,literal,-1 , 5, "lit");
        }
          else if(v[2][0]=='0' && v[2][1]=='x'){
            unsigned int x = std::stoul(v[3], nullptr, 16);
             asemb->psLdr(op1,x, -1 , 5, "hex");
          
        }
        else{
          //sam simbol
          std::string sym = v[2];
          asemb->psLdr(op1,literal,-1 , 5, sym);
        }
        }
      
    }
    else if(instr=="str"){
      std::string operand = v[2];
      std::string symb;
      std::string operand1 = v[1].substr(1,v[1].size()-2);
      int op1 = stoi(operand1);
      int literal;
        if(operand[0]=='$'){
          if((operand[1]>'a' && operand[1]<'z') || (operand[1]>'A' && operand[1]<'Z')){
            symb = operand.substr(1);
           asemb->psStr(op1,-1, 0, symb);
          }
          else{
            if(operand[1]=='0' && operand[2]=='x'){
              unsigned int x = std::stoul(operand.substr(2), nullptr, 16);
             asemb->psStr(op1,x , 0, "hex");
            }
            else{
            literal = stoi(operand.substr(1));
            asemb->psStr(op1,literal , 0, "lit");
            }
          }
        }
        else
        if(operand=="r0" || operand=="r1" || operand=="r2"|| operand=="r3" || operand=="r4"|| operand=="r5"
        || operand=="r6"|| operand=="r7"|| operand=="r8"||operand=="r9"||operand=="r10" || 
        operand=="r11" || operand=="r12"|| operand=="r13"|| operand=="r14"|| operand=="r15" || operand=="r16"){
        std::string broj = operand.substr(1);
        int num = stoi(broj);
        asemb->psStr(op1,num , 1, "reg");
          }
        else
        if(operand[0]=='%'){
          std::string sym = v[2].substr(1);
          asemb->psStr(op1,-1 , 2, sym);
        }
        else
        if(operand[0]=='['){
         bool space = false;
          if(operand[operand.size()-1]!=']'){
            space = true;
          }
          if(space){
            std::string tip = v[4].substr(0, 1);

            if(tip=="0" || tip=="1" || tip=="2"|| tip=="3" || tip=="4"|| tip=="5"
        || tip=="6"|| tip=="7"|| tip=="8"||tip=="9"||tip=="r10"){
            int literal = stoi(v[4].substr(0,v[4].size()-1));
        }
        else if(v[4][0]=='0' && v[4][1]=='x'){
           unsigned int x = std::stoul(v[3], nullptr, 16);
             asemb->psStr(op1,x , 3, "hex");
        }
        else{
          std::string symb = v[4].substr(0, v[4].size()-1);
          asemb->psStr(op1,-1 , 3, symb);
        }
          }
          else{
          std::string type = operand.substr(1, operand.size()-2);
          if(type=="r0" || type=="r1" || type=="r2"|| type=="r3" || type=="r4"|| type=="r5"
        || type=="r6"|| type=="r7"|| type=="r8"||type=="r9"||type=="r10" || 
        type=="r11" || type=="r12"|| type=="r13"|| type=="r14"|| type=="r15" || type=="r16"){
         
        std::string broj = operand.substr(2,operand.size()-1);
        int num = stoi(broj);
        asemb->psStr(op1,num , 4, "reg");
          }
          }
        }
        else{
            bool jednoslovan = false;
            std::string tip = v[2].substr(0, 1);
            if(tip=="0" || tip=="1" || tip=="2"|| tip=="3" || tip=="4"|| tip=="5"
        || tip=="6"|| tip=="7"|| tip=="8"||tip=="9"){
            int literal = stoi(v[2]);
            asemb->psStr(op1,literal , 5, "lit");
        }
          else if(v[2][0]=='0' && v[2][1]=='x'){
            unsigned int x = std::stoul(v[2], nullptr, 16);
             asemb->psStr(op1,x , 5, "hex");
          
        }
        else{
          //sam simbol
          std::string sym = v[2];
          asemb->psStr(op1,literal , 5, sym);
        }
        }
      
    }
    else if(instr=="ret"){
      //psRet();
      asemb->psRet();
    }
    else if(instr=="push"){
      std::string reg1;
      reg1 = v[1];
      std::string broj1 = reg1.substr(1);
      int num1 = stoi(broj1);
      asemb->psPush(num1);
    }
     else if(instr=="pop"){
     std::string reg1;
      reg1 = v[1];
      std::string broj1 = reg1.substr(1);
      int num1 = stoi(broj1);
      asemb->psPop(num1);
    }
     else if(instr.compare("xchg")==0){
      std::string reg1,reg2;
      reg1 = v[1];
      reg2 = v[2];
      std::string broj1 = reg1.substr(1);
      std::string broj2 = reg2.substr(1);
      broj1 = broj1.substr(0, broj1.size()-1);
      broj2 = broj2.substr(0, broj2.size());
      int num1 = stoi(broj1);
      int num2 = stoi(broj2);
      asemb->psXchg(num1,num2);
    }
     else if(instr.compare("add")==0){
      std::string reg1,reg2;
      reg1 = v[1];
      reg2 = v[2];
      std::string broj1 = reg1.substr(1);
      std::string broj2 = reg2.substr(1);
      broj1 = broj1.substr(0, broj1.size()-1);
      broj2 = broj2.substr(0, broj2.size());
      int num1 = stoi(broj1);
      int num2 = stoi(broj2);
      asemb->psAdd(num1,num2);
    }
     else if(instr.compare("sub")==0){
      std::string reg1,reg2;
      reg1 = v[1];
      reg2 = v[2];
      std::string broj1 = reg1.substr(1);
      std::string broj2 = reg2.substr(1);
      broj1 = broj1.substr(0, broj1.size()-1);
      broj2 = broj2.substr(0, broj2.size());
      int num1 = stoi(broj1);
      int num2 = stoi(broj2);
      asemb->psSub(num1,num2);
    }
     else if(instr.compare("mul")==0){
      std::string reg1,reg2;
      reg1 = v[1];
      reg2 = v[2];
      std::string broj1 = reg1.substr(1);
      std::string broj2 = reg2.substr(1);
      broj1 = broj1.substr(0, broj1.size()-1);
      broj2 = broj2.substr(0, broj2.size());
      int num1 = stoi(broj1);
      int num2 = stoi(broj2);
      asemb->psMul(num1,num2);
    }
     else if(instr.compare("div")==0){
      std::string reg1,reg2;
      reg1 = v[1];
      reg2 = v[2];
      std::string broj1 = reg1.substr(1);
      std::string broj2 = reg2.substr(1);
      broj1 = broj1.substr(0, broj1.size()-1);
      broj2 = broj2.substr(0, broj2.size());
      int num1 = stoi(broj1);
      int num2 = stoi(broj2);
      asemb->psDiv(num1,num2);
    }
     else if(instr.compare("cmp")==0){
      std::string reg1,reg2;
      reg1 = v[1];
      reg2 = v[2];
      std::string broj1 = reg1.substr(1);
      std::string broj2 = reg2.substr(1);
      broj1 = broj1.substr(0, broj1.size()-1);
      broj2 = broj2.substr(0, broj2.size());
      int num1 = stoi(broj1);
      int num2 = stoi(broj2);
      asemb->psCmp(num1,num2);
    }
    else if(instr == "not"){
      std::string reg1;
      reg1 = v[1];
      std::string broj1 = reg1.substr(1);
      int num1 = stoi(broj1);
      asemb->psNot(num1);
    }
    else if(instr.compare("and")==0){
      std::string reg1,reg2;
      reg1 = v[1];
      reg2 = v[2];
      std::string broj1 = reg1.substr(1);
      std::string broj2 = reg2.substr(1);
      broj1 = broj1.substr(0, broj1.size()-1);
      broj2 = broj2.substr(0, broj2.size());
      int num1 = stoi(broj1);
      int num2 = stoi(broj2);
      asemb->psAnd(num1,num2);
    }
    else if(instr.compare("or")==0){
      std::string reg1,reg2;
      reg1 = v[1];
      reg2 = v[2];
      std::string broj1 = reg1.substr(1);
      std::string broj2 = reg2.substr(1);
      broj1 = broj1.substr(0, broj1.size()-1);
      broj2 = broj2.substr(0, broj2.size());
      int num1 = stoi(broj1);
      int num2 = stoi(broj2);
      asemb->psOr(num1,num2);
    }
    else if(instr.compare("xor")==0){
      std::string reg1,reg2;
      reg1 = v[1];
      reg2 = v[2];
      std::string broj1 = reg1.substr(1);
      std::string broj2 = reg2.substr(1);
      broj1 = broj1.substr(0, broj1.size()-1);
      broj2 = broj2.substr(0, broj2.size());
      int num1 = stoi(broj1);
      int num2 = stoi(broj2);
      asemb->psXor(num1,num2);
    }
    else if(instr.compare("test")==0){
      std::string reg1,reg2;
      reg1 = v[1];
      reg2 = v[2];
      std::string broj1 = reg1.substr(1);
      std::string broj2 = reg2.substr(1);
      broj1 = broj1.substr(0, broj1.size()-1);
      broj2 = broj2.substr(0, broj2.size());
      int num1 = stoi(broj1);
      int num2 = stoi(broj2);
      asemb->psTest(num1,num2);
    }
    else if(instr.compare("shl")==0){
      std::string reg1,reg2;
      reg1 = v[1];
      reg2 = v[2];
      std::string broj1 = reg1.substr(1);
      std::string broj2 = reg2.substr(1);
      broj1 = broj1.substr(0, broj1.size()-1);
      broj2 = broj2.substr(0, broj2.size());
      int num1 = stoi(broj1);
      int num2 = stoi(broj2);
      asemb->psShl(num1,num2);
    }
    else if(instr.compare("shr")==0){
      std::string reg1,reg2;
      reg1 = v[1];
      reg2 = v[2];
      std::string broj1 = reg1.substr(1);
      std::string broj2 = reg2.substr(1);
      broj1 = broj1.substr(0, broj1.size()-1);
      broj2 = broj2.substr(0, broj2.size());
      int num1 = stoi(broj1);
      int num2 = stoi(broj2);
      asemb->psShr(num1,num2);
    }
    else if(instr=="jgt"){
      std::string operand = v[1];
      std::string symb;
      int literal;
      if(operand[0]=='%'){
          std::string sym = v[1].substr(1);
          asemb->psJgt(0,sym, -1);
        }
        else
        if(operand[0]=='*'){
        if(operand=="*r0" || operand=="*r1" || operand=="*r2"|| operand=="*r3" || operand=="*r4"|| operand=="*r5"
        || operand=="*r6"|| operand=="*r7"|| operand=="*r8"||operand=="*r9"||operand=="*r10" || 
        operand=="*r11" || operand=="*r12"|| operand=="*r13"|| operand=="*r14"|| operand=="*r15" || operand=="*r16"){
        std::string broj = operand.substr(2);
        int num = stoi(broj);
        asemb->psJgt(1,"reg", num);
          }
        else
        if(operand[1]=='['){
         bool space = false;
          if(operand[operand.size()-1]!=']'){
            space = true;
          }
          if(space){
            std::string tip = v[3].substr(0, 1);
            if(tip=="0" || tip=="1" || tip=="2"|| tip=="3" || tip=="4"|| tip=="5"
        || tip=="6"|| tip=="7"|| tip=="8"||tip=="9"||tip=="r10"){
            int literal = stoi(v[3].substr(0,v[3].size()-1));
            asemb->psJgt(2, "lit", literal);
        }
        else{
          std::string symb = v[3].substr(0, v[3].size()-1);
          //psregIndSymbol
          asemb->psJgt(2, symb, -1);
        }
          }
          else{
          std::string type = operand.substr(2, operand.size()-3);
          if(type=="r0" || type=="r1" || type=="r2"|| type=="r3" || type=="r4"|| type=="r5"
        || type=="r6"|| type=="r7"|| type=="r8"||type=="r9"||type=="r10" || 
        type=="r11" || type=="r12"|| type=="r13"|| type=="r14"|| type=="r15" || type=="r16"){
         
        std::string broj = operand.substr(3,operand.size()-3);
        int num = stoi(broj);
        asemb->psJgt(3, "reg", num);
      }
        else{
            std::string tip = v[1].substr(1, 2);
            if(tip=="0" || tip=="1" || tip=="2"|| tip=="3" || tip=="4"|| tip=="5"
        || tip=="6"|| tip=="7"|| tip=="8"||tip=="9"){
            int literal = stoi(v[1]);
           asemb->psJgt(3, "lit", literal);
        }
          else{
            //ovde je symbol
            std::string type = operand.substr(1);
             asemb->psJgt(3, type, -1);
          }
        }
      }
    }
  }
        else{      
            bool jednoslovan = false;
            std::string tip = v[1].substr(0, 1);
            if(tip=="0" || tip=="1" || tip=="2"|| tip=="3" || tip=="4"|| tip=="5"
        || tip=="6"|| tip=="7"|| tip=="8"||tip=="9"){
            int literal = stoi(v[1]);
            asemb->psJgt(4, "lit", literal);
        }
        else{
          //sam simbol
          std::string sym = v[1];
          asemb->psJgt(4, sym, -1);
        }
    }
    }else
if(instr=="jeq"){
      std::string operand = v[1];
      std::string symb;
      int literal;
      if(operand[0]=='%'){
          std::string sym = v[1].substr(1);
          asemb->psJeq(0, sym, -1);
        }
        else
        if(operand[0]=='*'){
  
        if(operand=="*r0" || operand=="*r1" || operand=="*r2"|| operand=="*r3" || operand=="*r4"|| operand=="*r5"
        || operand=="*r6"|| operand=="*r7"|| operand=="*r8"||operand=="*r9"||operand=="*r10" || 
        operand=="*r11" || operand=="*r12"|| operand=="*r13"|| operand=="*r14"|| operand=="*r15" || operand=="*r16"){
        std::string broj = operand.substr(2);
          int num = stoi(broj);
          asemb->psJeq(1, "reg", num);

          }
        else
        if(operand[1]=='['){
         bool space = false;
          if(operand[operand.size()-1]!=']'){
            space = true;
          }
          if(space){
            std::string tip = v[3].substr(0, 1);
            cout<<"Ovo je "<<tip<<endl;
            if(tip=="0" || tip=="1" || tip=="2"|| tip=="3" || tip=="4"|| tip=="5"
        || tip=="6"|| tip=="7"|| tip=="8"||tip=="9"||tip=="r10"){
            int literal = stoi(v[3].substr(0,v[3].size()-1));
           asemb->psJeq(2, "lit", literal);

        }
        else{
          std::string symb = v[3].substr(0, v[3].size()-1);
             asemb->psJeq(2, symb, stoi(operand.substr(1)));
        }
          }
          else{
          std::string type = operand.substr(2, operand.size()-3);
          if(type=="r0" || type=="r1" || type=="r2"|| type=="r3" || type=="r4"|| type=="r5"
        || type=="r6"|| type=="r7"|| type=="r8"||type=="r9"||type=="r10" || 
        type=="r11" || type=="r12"|| type=="r13"|| type=="r14"|| type=="r15" || type=="r16"){
         
        std::string broj = operand.substr(3,operand.size()-3);
        int num = stoi(broj);
          asemb->psJeq(3, "reg", num);
          }
          else{
            std::string tip = v[1].substr(1, 2);
            if(tip=="0" || tip=="1" || tip=="2"|| tip=="3" || tip=="4"|| tip=="5"
        || tip=="6"|| tip=="7"|| tip=="8"||tip=="9"){
            int literal = stoi(v[1]);
            asemb->psJeq(3, "lit", literal);
        }
            else{
            //ovde je symbol
            std::string type = operand.substr(1);
             asemb->psJeq(3, type, -1);
          }
        
          }
          }
        }
        }
        else{      
            bool jednoslovan = false;
            std::string tip = v[1].substr(0, 1);
            if(tip=="0" || tip=="1" || tip=="2"|| tip=="3" || tip=="4"|| tip=="5"
        || tip=="6"|| tip=="7"|| tip=="8"||tip=="9"){
            int literal = stoi(v[1]);
            asemb->psJeq(4, "lit", literal);
        }
        else{
          //sam simbol
          std::string sym = v[1];
         asemb->psJeq(4, sym, -1);
        }
    }  
    }
else
if(instr=="jne"){
      std::string operand = v[1];
      std::string symb;
      int literal;
      if(operand[0]=='%'){
          std::string sym = v[1].substr(1);
          asemb->psJne(0, sym, -1);
        }
        else
        if(operand[0]=='*'){
  
        if(operand=="*r0" || operand=="*r1" || operand=="*r2"|| operand=="*r3" || operand=="*r4"|| operand=="*r5"
        || operand=="*r6"|| operand=="*r7"|| operand=="*r8"||operand=="*r9"||operand=="*r10" || 
        operand=="*r11" || operand=="*r12"|| operand=="*r13"|| operand=="*r14"|| operand=="*r15" || operand=="*r16"){
        std::string broj = operand.substr(2);
        int num = stoi(broj);
       asemb->psJne(1, "reg", num);
          }
        else
        if(operand[1]=='['){
         bool space = false;
          if(operand[operand.size()-1]!=']'){
            space = true;
          }
          if(space){
            std::string tip = v[3].substr(0, 1);
            if(tip=="0" || tip=="1" || tip=="2"|| tip=="3" || tip=="4"|| tip=="5"
        || tip=="6"|| tip=="7"|| tip=="8"||tip=="9"||tip=="r10"){
            int literal = stoi(v[3].substr(0,v[3].size()-1));
            asemb->psJne(2, "lit", literal);
        }
        else{
          std::string symb = v[3].substr(0, v[3].size()-1);
          asemb->psJne(2, symb, -1);
        }
          }
          else{
          std::string type = operand.substr(2, operand.size()-3);
          if(type=="r0" || type=="r1" || type=="r2"|| type=="r3" || type=="r4"|| type=="r5"
        || type=="r6"|| type=="r7"|| type=="r8"||type=="r9"||type=="r10" || 
        type=="r11" || type=="r12"|| type=="r13"|| type=="r14"|| type=="r15" || type=="r16"){
         
        std::string broj = operand.substr(3,operand.size()-3);
        int num = stoi(broj);
        asemb->psJne(3, "reg", num);
          }
          else{
            std::string tip = v[1].substr(1, 2);
            if(tip=="0" || tip=="1" || tip=="2"|| tip=="3" || tip=="4"|| tip=="5"
        || tip=="6"|| tip=="7"|| tip=="8"||tip=="9"){
            int literal = stoi(v[1]);
            asemb->psJne(3, "lit", literal);
        }
            else{
            //ovde je symbol
            std::string type = operand.substr(1);
            asemb->psJne(3, type, -1);
          }
        
          }
          }
        }
        }
        else{      
            bool jednoslovan = false;
            std::string tip = v[1].substr(0, 1);
            if(tip=="0" || tip=="1" || tip=="2"|| tip=="3" || tip=="4"|| tip=="5"
        || tip=="6"|| tip=="7"|| tip=="8"||tip=="9"){
            int literal = stoi(v[1]);
          asemb->psJne(4, "lit", literal);
        }
        else{
          //sam simbol
          std::string sym = v[1];
          asemb->psJne(4, sym, -1);
        }
    }
  }

  else if(instr=="call"){
      std::string operand = v[1];
      std::string symb;
      int literal;
      if(operand[0]=='%'){
          std::string sym = v[1].substr(1);
          asemb->psCall(0, sym, -1);
        }
        else
        if(operand[0]=='*'){
  
        if(operand=="*r0" || operand=="*r1" || operand=="*r2"|| operand=="*r3" || operand=="*r4"|| operand=="*r5"
        || operand=="*r6"|| operand=="*r7"|| operand=="*r8"||operand=="*r9"||operand=="*r10" || 
        operand=="*r11" || operand=="*r12"|| operand=="*r13"|| operand=="*r14"|| operand=="*r15" || operand=="*r16"){
        std::string broj = operand.substr(2);
        int num = stoi(broj);
        asemb->psCall(1, "reg", num);
          }
        else
        if(operand[1]=='['){
         bool space = false;
          if(operand[operand.size()-1]!=']'){
            space = true;
          }
          if(space){
            std::string tip = v[3].substr(0, 1);
            if(tip=="0" || tip=="1" || tip=="2"|| tip=="3" || tip=="4"|| tip=="5"
        || tip=="6"|| tip=="7"|| tip=="8"||tip=="9"||tip=="r10"){
            int literal = stoi(v[3].substr(0,v[3].size()-1));
            asemb->psCall(2, "lit", literal);
        }
        else{
          int literal = stoi(v[1].substr(3));
          std::string symb = v[3].substr(0, v[3].size()-1);
          asemb->psCall(2, symb, literal);
        }
          }
          else{
          std::string type = operand.substr(2, operand.size()-3);
          if(type=="r0" || type=="r1" || type=="r2"|| type=="r3" || type=="r4"|| type=="r5"
        || type=="r6"|| type=="r7"|| type=="r8"||type=="r9"||type=="r10" || 
        type=="r11" || type=="r12"|| type=="r13"|| type=="r14"|| type=="r15" || type=="r16"){
         
        std::string broj = operand.substr(3,operand.size()-3);
        int num = stoi(broj);
        asemb->psCall(3, "reg", num);
        //psCall(broj)
          }
          else{
            std::string tip = v[1].substr(1, 2);
            if(tip=="0" || tip=="1" || tip=="2"|| tip=="3" || tip=="4"|| tip=="5"
        || tip=="6"|| tip=="7"|| tip=="8"||tip=="9"){
            int literal = stoi(v[1]);
            asemb->psCall(3, "lit", literal);
        }
            else{
            //ovde je symbol
            std::string type = operand.substr(1);
           asemb->psCall(3, type, -1);
          }
        }
     }
  }
        }
        else{      
            bool jednoslovan = false;
            std::string tip = v[1].substr(0, 1);
            if(tip=="0" || tip=="1" || tip=="2"|| tip=="3" || tip=="4"|| tip=="5"
        || tip=="6"|| tip=="7"|| tip=="8"||tip=="9"){
            int literal = stoi(v[1]);
           asemb->psCall(4, "lit", literal);
        }
        else{
          //sam simbol
          std::string sym = v[1];
          asemb->psCall(4, sym, -1);
        }
    }
    }
else if(instr=="jmp"){
      std::string operand = v[1];
      std::string symb;
      int literal;
      if(operand[0]=='%'){
          std::string sym = v[1].substr(1);
          asemb->psJump(0, sym, -1);
        }
        else
        if(operand[0]=='*'){
  
        if(operand=="*r0" || operand=="*r1" || operand=="*r2"|| operand=="*r3" || operand=="*r4"|| operand=="*r5"
        || operand=="*r6"|| operand=="*r7"|| operand=="*r8"||operand=="*r9"||operand=="*r10" || 
        operand=="*r11" || operand=="*r12"|| operand=="*r13"|| operand=="*r14"|| operand=="*r15" || operand=="*r16"){
        std::string broj = operand.substr(2);
        int num = stoi(broj);
        asemb->psJump(1, "reg", num);
          }
        else
        if(operand[1]=='['){
         bool space = false;
          if(operand[operand.size()-1]!=']'){
            space = true;
          }
          if(space){
            std::string tip = v[3].substr(0, 1);
            if(tip=="0" || tip=="1" || tip=="2"|| tip=="3" || tip=="4"|| tip=="5"
        || tip=="6"|| tip=="7"|| tip=="8"||tip=="9"||tip=="r10"){

            int literal = stoi(v[3].substr(0,v[3].size()-1));
           asemb->psJump(2, "lit", literal);
        }
        else{
          std::string symb = v[3].substr(0, v[3].size()-1);
          asemb->psJump(2, symb, -1);
        }
          }
          else{
          std::string type = operand.substr(2, operand.size()-3);
          if(type=="r0" || type=="r1" || type=="r2"|| type=="r3" || type=="r4"|| type=="r5"
        || type=="r6"|| type=="r7"|| type=="r8"||type=="r9"||type=="r10" || 
        type=="r11" || type=="r12"|| type=="r13"|| type=="r14"|| type=="r15" || type=="r16"){
         
        std::string broj = operand.substr(3,operand.size()-3);
        int num = stoi(broj);
        asemb->psJump(3, "reg", num);
    
          }
          else{
            std::string tip = v[1].substr(1, 2);
            if(tip=="0" || tip=="1" || tip=="2"|| tip=="3" || tip=="4"|| tip=="5"
        || tip=="6"|| tip=="7"|| tip=="8"||tip=="9"){
            int literal = stoi(v[1]);
            asemb->psJump(3, "lit", literal);
        }
            else{
            //ovde je symbol
            std::string type = operand.substr(1);
          asemb->psJump(3, type, -1);
          }
        }
     }
  }
        }
        else{      
            bool jednoslovan = false;
            std::string tip = v[1].substr(0, 1);
            if(tip=="0" || tip=="1" || tip=="2"|| tip=="3" || tip=="4"|| tip=="5"
        || tip=="6"|| tip=="7"|| tip=="8"||tip=="9"){
            int literal = stoi(v[1]);
           asemb->psJump(4, "lit", literal);
        }
        else{
          //sam simbol
          std::string sym = v[1];
          asemb->psJump(4, sym, -1);
        }
    }
    }

  else if(instr==".global"){
      vector<std::string> send;
      if(v.size()>2){
      for(int i=1 ;i<v.size();i++){
        std::string lit = v[i];
        if(i!=v.size()-1){
           lit = v[i].substr(0,v[i].size()-1);
        }
          send.push_back(lit);       
    }
  }else{
    send.push_back(v[1]); 
      }
      asemb->psGlobal(send);
    }

    else if(instr==".extern"){
       vector<std::string> send;
      if(v.size()>2){
      for(int i=1 ;i<v.size();i++){
        std::string lit = v[i];
        if(i!=v.size()-1){
           lit = v[i].substr(0,v[i].size()-1);
        }
          send.push_back(lit);       
    }
  }else{
    send.push_back(v[1]); 
      }
      asemb->psExtern(send);
    }
    else if(instr==".section"){
      std::string sym;
      sym = v[1];
      asemb->psSection(sym);
    }
    else if(instr==".word"){
      vector<std::string> send;
      if(v.size()>2){
      for(int i=1 ;i<v.size();i++){
        std::string lit = v[i];
        if(i!=v.size()-1){
           lit = v[i].substr(0,v[i].size()-1);
        }
          send.push_back(lit);       
    }
  }else{
    send.push_back(v[1]); 
      }
      asemb->psWord(send);
    }
    else if(instr==".skip"){
      std::string sym;
      sym = v[1];
      int toSkip = stoi(sym);
     asemb->psSkip(toSkip);
    }
    else if(instr==".end"){
      asemb->psEnd();
    }
    else if(instr[instr.size()-1]==':'){
      instr = instr.substr(0,instr.size()-1);
      asemb->psSym(instr);
    } 
    else if(instr=="#"){
      continue;
    }
    else{
      cout<<"***DOGODILA SE GRESKA*** "<<endl;
      exit(1);
    }

  }
 // SymbolTable::printSymbols();
  asemb->fixExtern();
  asemb->backpatch();

  fileOutput = freopen(outputName.c_str(), "w", stdout);

  asemb->printSymbolTable();
  cout<<endl;
  asemb->printRelocation();
  cout<<endl;
  asemb->printAllSectionsMemory();
  fclose(fileOutput);

}