#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include<vector>
#include<sstream>

#include "emulator.h"

using namespace std;

     

void Emulator::loadMemory(ifstream &inputFile){
      std::string line;
      for(int i=0; i<8;i++)regs.push_back(0x00);
      vector<string>vec;
      while(std::getline(inputFile, line)){
        string pom = "";
        for(int i=6; i<line.size();i++){
          if(line[i]!=' '){
            pom=pom+line[i];
          }else{
            vec.push_back(pom);
            pom="";
          }
        }
        vec.push_back(pom);        
    }
      for(int i=0; i<vec.size();i++){
           std::stringstream ss;
          ss << std::hex << vec[i];
          unsigned short val;
          ss>>val;
          mem.push_back(val);
        }
      for(int i=0; i<(2<<16);i++){
        mem.push_back((unsigned short)0x00);
      }
}

void Emulator::fetch(){
  bool first = true;
  psw = 0x0000;
  unsigned short a = mem[0];
  unsigned short b = mem[1];
  unsigned short staJe = ((b<<8)+a);
  regs[7]=staJe;
  regs[7]++;
  staJe = mem[staJe];
  while(true){
    if(!first){
     staJe = citajSledeci();
    }else first = false;
    if(staJe ==0x00){
      //halt
      break;
    }
    else if(staJe==0x10){
      int drugiBajt = citajSledeci();
      //oblika XXXX 1111 gde je XXXX na koju prekidnu skacemo
      int broj = (drugiBajt & 0x0000000F);
      push(regs[7]);
      push(psw);

      unsigned short a = mem[(regs[broj]%8)*2];
      unsigned short b = mem[(regs[broj]%8)*2+1];
      b=b<<8;
      a = a+b;
      psw |= (0<<13); //zabrani prekid
      regs[7] = a;
    }
    else if(staJe ==0x20){
      //iret
      psw = pop();
      regs[7]=pop();
    }
    else if(staJe==0x30){
      //call
      int operand = addr();
       push(regs[7]);
      regs[7] = operand;
    }
    else if(staJe ==0x40){
      //ret
      regs[7]=pop();
    }
    else if(staJe ==0x51 || staJe==0x52 || staJe==0x53 || staJe==0x54 || staJe==0x50){
      //jump
      if(staJe==0x50){
        unsigned short operand = addr();
        regs[7] = operand;
      }

      if((psw & 0x00000001) && staJe==0x51){
      int operand = addr();
      regs[7] = operand;
      }
      else if((psw & 0x00000008) && staJe==0x52){
      int operand = addr();
      regs[7] = operand;
      }
      else if(!(psw & 0x00000008) && !(psw & 0x000000001) && staJe==0x53){
      int operand = addr();
      regs[7] = operand;
      }
    }
    else if(staJe ==0xA0){
      //POP LDR
      unsigned short sledeci = mem[regs[7]];
      unsigned short dddd = (sledeci & 0x00F0)>>4;
      unsigned short ssss = sledeci & 0x000F;
      unsigned short operand = addr();
      regs[dddd]=operand;
    }
    else if(staJe ==0xB0){
      //PUSH STR
      unsigned short sledeci = citajSledeci();
      unsigned short dddd = (sledeci & 0x00F0)>>4;
      unsigned short ssss = sledeci & 0x000F;
      unsigned short registar = regs[dddd];
      unsigned short koje = citajSledeci();
      unsigned short addressing = koje & 0x0000000F;
      unsigned short up = (koje & 0x00F0)>>4;
      unsigned short value = 0;
  if(addressing==2){
    //regind

    if(up==1){
      regs[ssss]-=2;
    }
    else if(up==2){
      regs[ssss]+=2;

    }
    int val = regs[ssss];
    if(up==3){
      regs[ssss]-=2;
    }
    else if(up==4){
      regs[ssss]+=2;
    }
    value = val;
  }else if(addressing == 4){
   unsigned short a = citajSledeci();
     unsigned short b = citajSledeci();
     b = b<<8;
     value = (unsigned short)(b+a);
  }else if(addressing==0){
    cout<<"***Greska- ne moze STORE sa neposrednim ***"<<endl;
    break;
  }
  else if(addressing==1){
    cout<<"***Greska- ne moze STORE sa registarskim direktnim ***"<<endl;
    break;
  }
  else if(addressing==3){
    unsigned short a = citajSledeci();
    unsigned short b = citajSledeci();
    b = b<<8;
     if(up==1){
      regs[ssss]-=2;
    }
    else if(up==2){
      regs[ssss]+=2;
    }
    int val = regs[ssss] + b + a;
    if(up==3){
      regs[ssss]-=2;
    }
    else if(up==4){
      regs[ssss]+=2;
    }
    value = val;
  }
    mem[value]=regs[dddd];
      
    } 
    else if(staJe==0x70 || staJe==0x71  || staJe==0x72 || staJe==0x73 || staJe==0x74){
      //ARITH
      unsigned short sledeci = citajSledeci();
      unsigned short dddd = (sledeci & 0x000000F0)>>4;
      unsigned short ssss = sledeci & 0x0000000F;
      if(staJe==0x70){
        regs[dddd]=(regs[dddd]+regs[ssss]);
      }
      if(staJe==0x71){
        regs[dddd]=(regs[dddd]-regs[ssss]);
      }
      if(staJe==0x72){
        regs[dddd]=(regs[dddd]*regs[ssss]);
      }
      if(staJe==0x73){
        regs[dddd]=(regs[dddd]/regs[ssss]);
      }
      if(staJe==0x74){
        //cmp
       int temp=(regs[dddd]-regs[ssss]);
       if(temp==0)psw|=0x00000001;
       else if(temp<0)psw|=0x00000008;
      }

    }else if(staJe==0x80 || staJe==0x81 || staJe==0x82 || staJe==0x83 || staJe==0x84){
      //LOGIC
      unsigned short sledeci = citajSledeci();
      unsigned short dddd = (sledeci & 0x000000F0)>>4;
      unsigned short ssss = sledeci & 0x0000000F;
      if(staJe==0x80){
        regs[dddd]=~regs[ssss];
      }
      if(staJe==0x81){
        regs[dddd]=(regs[dddd] & regs[ssss]);
      }
      if(staJe==0x82){
        regs[dddd]=(regs[dddd] | regs[ssss]);
      }
      if(staJe==0x83){
        regs[dddd]=(regs[dddd] ^ regs[ssss]);
      }
      if(staJe==0x84){
        //cmp
       int temp=(regs[dddd] & regs[ssss]);
       if(temp==0)psw|=0x00000001;
       else if(temp<0)psw|=0x00000008;
      }
    }
    else if(staJe==0x90 || staJe==0x91){
      unsigned short sledeci = citajSledeci();
      unsigned short dddd = (sledeci & 0x000000F0)>>4;
      unsigned short ssss = sledeci & 0x0000000F;
      unsigned short previos = regs[dddd];
      if(staJe==0x80){
        regs[dddd]=(regs[dddd] >> regs[ssss]);
      }
      if(staJe==0x81){
        regs[dddd]=(regs[dddd] << regs[ssss]);
      }
      if(regs[dddd]==0)psw|=0x00000001;
       else if(regs[dddd]<0)psw|=0x00000008;

       if((previos>0 && regs[dddd]<0) || (previos<0 && regs[dddd]>0)){
              psw|=0x00000004;
       }
    }
    else {
      cout<<"*** INVALID INSTRUCTION *** "<<endl;
    }
  }
}

unsigned short Emulator::addr(){
  unsigned short regovi = citajSledeci();
  regovi = regovi & (0x000F);
  unsigned short registar = regs[regovi];
  unsigned short koje = citajSledeci();
  unsigned short addressing = koje & 0x0000000F;
  unsigned short up = (koje & 0x00F0)>>4;

  if(addressing==0){
    //IMMED
    unsigned short a = citajSledeci();
    unsigned short b = citajSledeci();
    b = b<<8;
    return (a+b);
  }
  else if(addressing==1){
    //regdir
    return (unsigned short)registar;
  }
  else if(addressing==2){
    //regind

    if(up==1){
      regs[regovi]-=2;
    }
    else if(up==2){
      regs[regovi]+=2;

    }
    int val = regs[regovi];
    if(up==3){
      regs[regovi]-=2;
    }
    else if(up==4){
      regs[regovi]+=2;
    }
    return (unsigned short)mem[val];
    //regdirpom
  }else if(addressing==3){
    //regindpom

    unsigned short a = citajSledeci();
    unsigned short b = citajSledeci();
    b = b<<8;
     if(up==1){
      regs[regovi]-=2;
    }
    else if(up==2){
      regs[regovi]+=2;
    }
    int val = regs[regovi] + b + a;
    if(up==3){
      regs[regovi]-=2;
    }
    else if(up==4){
      regs[regovi]+=2;
    }
    return(unsigned short)(mem[val]);
  }
  else if(addressing == 4){
     unsigned short a = citajSledeci();
     unsigned short b = citajSledeci();
     b = b<<8;
     return (unsigned short)mem[(b+a)];
  }
  else if(addressing==5){
    int a = citajSledeci();
     int b = citajSledeci();
     b = b<<8;
     return (unsigned short)(registar+3+a+b);
  }
  else{
    cout<<"*** NEPOSTOJECI NACIN ADRESIRANJA! ***"<<endl;
  }
  return 0;
}


int main (int argc, char *argv[])
{


  Emulator*emul = new Emulator();
  if(argc < 2){
    cout<<"Nedovoljno argumenata pri pozivu!"<<endl;
    return 1;
  }
  string fileName=string(argv[1]);
  ifstream inFile(fileName);
  emul->loadMemory(inFile);
  emul->fetch();
  emul->end();
  
     
}
