#ifndef EMULATOR_H
#define EMULATOR_H
#include<iostream>
#include <string>
#include <vector>
#include<sstream>
#include<iomanip>
#include <bitset>


using namespace std;
class Emulator
{

public:
    unsigned short citajSledeci(){
        unsigned short opCode = mem[regs[7]++];
        return opCode;
    }
    void fetch();
    void push(unsigned short val){
        regs[6]-=2;
        mem[regs[6]]=val;
    }
    unsigned short pop(){
        unsigned short val = mem[regs[6]];
        regs[6]+=2;
        return val;
    }
    unsigned short addr();

   void end(){
    cout<<"Emulated processor executed halt instruction"<<endl;
    cout<<"Emulated processor state: ";
    string pswString="psw=0b";
    string vrednost = "";
    std::bitset<16> newPsw(psw);
    for(int i=15; i>=0;i--){
        pswString+=std::to_string(newPsw[i]);
    }
    cout<<pswString<<endl;
        std::stringstream stream;
        vector<string>vrednosti;
         stream<< std::hex << std::setfill('0');

    for(int i=0; i<regs.size();i++){
        stream<<std::setw(4)<<regs[i];
        std::string result(stream.str());        
    }
    std::string result(stream.str());
    int start = 0;
    for(int i=0; i<regs.size();i++){
        cout<<"r"<<i<<"=0x"<<result.substr(start,4)<<"    ";
        if(i==3)cout<<endl;
        start +=4;
    }
    cout<<endl;
   }


    void loadMemory(ifstream &inputFile);

private:
    unsigned short psw;
    vector<unsigned short>mem;
    vector<unsigned short>regs;

public:

};

#endif