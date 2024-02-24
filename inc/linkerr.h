#ifndef LINKERR_H
#define LINKERR_H
#include<iostream>
#include <string>
#include <list>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>

using namespace std;

class Linker{

  public:
  struct Symbol{
    std::string name;
    int value;
    std::string section;
    bool locality;
    int fileId;
  };
  struct Relocation{
    std::string symbol;
    std::string type;
    std::string section;
    int location;
    int fileId;
  };

  void procitaj_fajl(std::ifstream &file);

  void popravi_relokacije();

  void slozi_sekcije();

  void popravi_simbole();

  void proveri_gresku1();

  void razresi_relokacije();

  void upisi_rec(std::string sekcija, int location, unsigned short  sadrzaj);

  void izgenerisi_izlaz(std::ofstream &);

  void napravi_ceo();
  

  private:
  //ovde cemo da imamo sve strukture koje su nam potrebne
    std::vector<string> sectionOrder;
    std::unordered_map<std::string, std::vector<Relocation>> relocations;
    std::unordered_map<std::string, Symbol> symbols; //ovo mu je sym table fakticki
    std::unordered_map<std::string, vector<unsigned short>> section_locations;
    std::unordered_map<std::string, std::vector<unsigned char>> bytes;
    std::unordered_map<std::string, vector<int>> sections_files;
    static int fileId;
    std::vector<unsigned char>sadrzaj;
};

#endif