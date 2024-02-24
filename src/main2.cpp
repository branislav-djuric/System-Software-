#include<iostream>
#include<fstream>
#include<string>
#include<regex>
#include<unordered_map>
#include<iomanip>
#include<sstream>
#include<algorithm>
#include"linkerr.h"

using namespace std;

int main (int argc, char *argv[])
{
    std::string file_name = "";
    std::stringstream ss;
    std::vector<std::string> files;

    if(argc>4){
        if(argv[1]!=std::string("-hex"))return -2;
        if(argv[2]!=std::string("-o"))return -1;
        file_name=argv[3];
    for(int i = 4; i < argc; ++i)
    {
        files.push_back(argv[i]); 
    }

    }else{
        cout<<"Prazan?"<<endl;
        return 0;
    }
   

    Linker*linker = new Linker();

    for(std::string filename : files)
    {
        std::ifstream file(filename);
        linker->procitaj_fajl(file);
        file.close();
    }
    std::ofstream out(file_name, std::ofstream::trunc);
    
     linker->slozi_sekcije();
     linker->popravi_relokacije();
     linker->popravi_simbole();
     linker->razresi_relokacije();
     linker->napravi_ceo();
     linker->izgenerisi_izlaz(out);

    out.close();

    return 0;
}