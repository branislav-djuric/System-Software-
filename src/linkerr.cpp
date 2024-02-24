#include"linkerr.h"
#include<iomanip>
#include<fstream>
#include<sstream>

using namespace std;

int Linker::fileId = 0;

void Linker::procitaj_fajl(std::ifstream & file)
{
    std::string line;
    bool flag=true;
    bool zavrsio=false;
    //SYMBOLS
    while(std::getline(file, line))
    {
        if(flag){
            flag=false;
            continue;
        }
        if(line == ""){
                 zavrsio=true;
                        break;

        }
        std::stringstream ss(line);
        Linker::Symbol sym;
        std::string val;
        std::string def;
        std::string scope;
        std::string a,b,c,d,e;
        ss>>a>>b;
      
        ss>>sym.value;
   
        ss>>scope>>e;
            sym.locality=false;
        if(scope=="local"){
            sym.locality=true;
            sym.value=0;
        }

        sym.name=a;
        sym.section=b;
        sym.fileId = fileId;
        if(sym.name!="UND" && sym.name!="ABS"){
          if(sym.section!="UND"){
            symbols[sym.name]=sym;
          }
        }
   }
while(true){
bool fl=true;
 bool fl2=true;
 bool gotov=false;
 bool k=false;
std::string section;

    //RELOCATIONS
    while(std::getline(file, line))
    {
            if(line=="EOF"){ 
            gotov=true;
            break;
            }
            if(line==""){ 
            break;
            }
            if(line=="Relocation: "){
                break;
            }
         std::stringstream ss(line);
        std::string size;
        if(line=="name   |offset  |type"){
            continue;
        }
        if(fl2){
       ss >> std::hex >> section;
       fl2=false;
       continue;
        }
            std::string index;
            Linker::Relocation rel;
            rel.fileId = Linker::fileId;
            std::string pcrel;           
            ss >> rel.symbol >> rel.location >> rel.type;
            rel.section = section;

             if(rel.symbol!=""){
           relocations[section].push_back(rel);
             }  
    }
   if(gotov==true)break;
}
while(true){
  
    //BYTES
    bool gotov1=false;
    bool fl=true;
    std::string section11;

     while(std::getline(file, line))
    {
        if(line == ""){            
            break;
        }
        std::string index;
        std::stringstream ss(line);
         if(fl){
        ss>>index;
         if(index=="EOF"){
            gotov1=true;
            break;
         }  
         
        Linker::Symbol sym;
        section11=line;
        sym.name=section11;
        sections_files[section11].push_back(fileId);
       bool exist = false;
       for(std::string sec: sectionOrder){
          if(sec==section11)exist = true;
       }
       switch(exist){
        case false : 
        {
        sectionOrder.push_back(section11);
        section_locations[section11].push_back(0);
        break;
        }
        case true : {
          section_locations[section11].push_back(bytes[section11].size());
          break;
        } 
       }
    }
  
        std::getline(file, line);
        std::stringstream file_bytes(line);
        unsigned short byte;
        while(file_bytes >> std::hex >> byte)
        {
            bytes[section11].push_back(byte);
        }    
    }
    if(gotov1==true){
        break;
    }
}
fileId++;
}



void Linker::popravi_relokacije(){
    int index = 0;
  for(std::string sec : sectionOrder){
    for(Relocation rel: relocations[sec]){
      index=0;
        if(sections_files[sec].size()>1){

          for(int j=0; j<sections_files[sec].size();j++){
            if(rel.fileId!=sections_files[sec][j])index++;
            else break;
          }
          if(rel.fileId==sections_files[sec][index]){
          rel.location+=(section_locations[sec][index]-section_locations[sec][0]);
          
          for(int k=0; k<relocations[rel.section].size();k++){
            if(relocations[sec][k].symbol==rel.symbol){
            relocations[sec][k]=rel;
            }
          }
          }
        }
    }    
    }  
}

void Linker::slozi_sekcije(){
  int pomeraj = 0;
  int index = 0;
  while(index<sectionOrder.size()){
    std::string trenSekcija = sectionOrder[index];
    for(int i=0; i<section_locations[trenSekcija].size();i++){
    section_locations[trenSekcija][i]+=pomeraj;
    }
    pomeraj+=bytes[trenSekcija].size();
    index++;
  }
}

void Linker::popravi_simbole(){
  for(std::pair<const std::string, Linker::Symbol> & sym : Linker::symbols)
    {     
          int index=0;
          sym.second.value+=section_locations[sym.second.section][0];
          if(sections_files[sym.second.section].size()>1){

          for(int j=0; j<sections_files[sym.second.section].size();j++){
            if(sym.second.fileId!=sections_files[sym.second.section][j])index++;
            else break;
          }
          if(sym.second.fileId==sections_files[sym.second.section][index]){
          sym.second.value+=(section_locations[sym.second.section][index]-section_locations[sym.second.section][0]);
        
          }
        }
    }
}

void Linker::proveri_gresku1(){
  for(std::pair<const std::string, Linker::Symbol> & sym : Linker::symbols)
    {
      if(sym.second.section=="UND"){
        cout<<"GRESKA NEDEFINISAN SIMBOL"<<endl;
        exit(1);
      }
    }
}

void Linker::razresi_relokacije(){
  for(std::string sec : sectionOrder){

    for(Relocation rel: relocations[sec]){
          if(rel.type.compare("R_X86_16")==0){
            //S+A 
            upisi_rec(sec, rel.location, symbols[rel.symbol].value);
          }else{
            //S+A-P
            //Addend je uprogramiran u memoriji
            unsigned short S = symbols[rel.symbol].value;
            unsigned short P = rel.location + section_locations[sec][0];
             upisi_rec(sec,rel.location,S-P);
          }
    }
  }
}

void Linker::upisi_rec(std::string sekcija, int location, unsigned short  sadrzaj){
  unsigned short before_lower = (unsigned short)bytes[sekcija][location];
  unsigned short before_higher = (unsigned short)bytes[sekcija][location+1];

  unsigned short value_before = before_higher*256+before_lower;
  value_before+=sadrzaj;

  bytes[sekcija][location]=value_before;
  bytes[sekcija][location+1]=value_before/256;
}
void Linker::izgenerisi_izlaz(std::ofstream & out){

 out<< std::hex << std::setfill('0');
  for(int i=0; i<sadrzaj.size();i++){
      if(i%8==0){
        out<<std::setw(4)<<i<<": ";
      }
      if(i%8!=7){
        if(i!=sadrzaj.size()-1)
          out<<std::setw(2)<<(unsigned short)sadrzaj[i]<<" ";
        else out<<std::setw(2)<<(unsigned short)sadrzaj[i];
      }
      else{
        if(i!=sadrzaj.size()-1)
          out<<std::setw(2)<<(unsigned short)sadrzaj[i]<<"\n";
          else out<<std::setw(2)<<(unsigned short)sadrzaj[i];
      }
  }
}
void Linker::napravi_ceo(){

  for(std::string & section : sectionOrder)
    {          
        for(unsigned char & byte : bytes[section])
        {
          sadrzaj.push_back(byte);
        }
    }
}