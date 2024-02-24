#ifndef ASEMBLER_H
	#define ASEMBLER_H
	#include <string>
	#include <vector>
  #include <unordered_map>
	using namespace std;


  struct flink{
	int location;
	string section;
	flink(string section, int loc):section(section),location(loc){};
	flink(){};
};

struct Symbol {
	  string name;
    string sekcija;
	  int value;
	  int scope; 
		bool def;
		vector<flink>patch;
	  Symbol(string name, string sekcija, int value, int scope, bool defined) : name(name), sekcija(sekcija), value(value), scope(scope), def(defined) {
			//patch=vector<flink>(size,0);
	  }
	};
	
	struct RelocElem {
		string name;
		int offset;
		int type; 
		RelocElem(string n,int offset, int type):name(n), offset(offset), type(type){}
	};
	
	struct SectionElem {
	  string name;
	  vector<unsigned char> bytes;
	  vector<RelocElem> relocTable;
	  SectionElem(string name):name(name){
		  bytes = vector<unsigned char> ();
		  relocTable = vector<RelocElem> ();
	  }
	};
	
	class Asembler {
	
	public:
	
	 int locationCounter;
	 vector<string> sekcije; 
   vector<SectionElem*> memorija;
	 string trenSekcija;

	
	public:

      Asembler(){
      locationCounter=0;
      trenSekcija = "UND";
     }


		 void printRelocation();
		 void printAllSectionsMemory();

     void psHalt();
     void psInt(int a);
     void psIret();
     void psRet();
     void psCall(int adresiranje, std::string sym, int num);
	   void psJeq(int adresiranje, std::string sym, int num);
     void psJgt(int adresiranje, std::string sym, int num);
     void psJne(int adresiranje, std::string sym, int num);
     void psJump(int adresiranje, std::string sym, int num);
     void psPush(int a);
     void psPop(int a);
     void psXchg(int a, int b);
     void psAdd(int a, int b);
     void psSub(int a, int b);
     void psMul(int a, int b);
     void psDiv(int a, int b);
     void psCmp(int a, int b);
     void psNot(int a);
     void psAnd(int a, int b);
     void psOr(int a, int b);
     void psXor(int a, int b);
     void psTest(int a, int b);
     void psShl(int a, int b);
     void psShr(int a, int b);
     void psLdr(int a,int b,int c, int adresiranje, std::string sym);
     void psStr(int a,int b, int adresiranje, std::string sym);

    //simboli i sekcije
     void psGlobal(vector<std::string> vec);
     void psExtern(vector<std::string> vec);
     void psSection(std::string sym);
     void psWord(vector<std::string> vec);
     void psSkip(int skip);
     void psEnd();
     void psSym(std::string sym);

     //ts logika
      void initTable();
	   void addSymbol(string symb);
	   void addSection(string sect);
	   void fixExtern();
	   void printSymbolTable();
	   Symbol* findSymbolInTable(string sym);
	
	   void markSymbolGlobal(std::string sym);
	   void markSymbolExtern(std::string sym);
		 void backpatch();

		 unordered_map<string, Symbol*> table;
		 vector<Symbol*>simboli;

	};
	
	
	
	#endif