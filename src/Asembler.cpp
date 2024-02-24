#include "Asembler.h"
#include <iostream>
#include <iomanip>



void Asembler::backpatch(){
	for(std::pair<const std::string, Symbol*> & itr : table){
	if(itr.second->scope==0){
			 for(flink f:itr.second->patch){
				for(int i=0; i<Asembler::memorija.size();i++){
						if(memorija[i]->name == f.section){
							unsigned char higher = 0x00;
        			unsigned char lower = 0x00;
            higher = (itr.second->value & 0x0000FF00) >> 8;
            lower = (itr.second->value & 0x000000FF);
				    memorija[i]->bytes[locationCounter + f.location] = lower;
						memorija[i]->bytes[locationCounter + f.location+1] = higher;
				} 		
			}
		}
	if(itr.second->sekcija=="UND"){
			 for(flink f:itr.second->patch){
		 for(int i=0; i<memorija.size();i++){
						if(memorija[i]->name == f.section){
							unsigned char higher = 0x00;
    					unsigned char lower = 0x00;
  					memorija[i]->bytes[locationCounter + f.location] = lower;
						memorija[i]->bytes[locationCounter + f.location+1] = higher;
				} 		
			}
	}
}
}
}
}
void Asembler::markSymbolGlobal(std::string symb){
	simboli.push_back(new Symbol(symb,trenSekcija,0,1,false));
	for(std::pair<const std::string, Symbol*> & sym : table){
			if(sym.first == symb){
				sym.second->scope = 1;
			}
	}
}

void Asembler::markSymbolExtern(std::string symb){
		simboli.push_back(new Symbol(symb,trenSekcija,0,1,false));
	for(std::pair<const std::string, Symbol*> & sym : table){
			if(sym.first == symb){
				sym.second->scope = 1;
				sym.second->sekcija = "UND"; //za extern
			}
	}
}
void Asembler::initTable(){
	table.insert(std::make_pair(std::string("UND"),new Symbol(std::string("UND"),trenSekcija,0,0,false)));
}

void Asembler::addSymbol(string naziv){
	for(std::pair<const std::string, Symbol*> & sym : table)
    {  
					if(sym.first==naziv){
					sym.second->value=locationCounter;
					sym.second->sekcija = trenSekcija;  
					sym.second->def = true;
						break;
			}else if(sym.first==naziv && sym.second->sekcija!="UND"){
				cout<<"***GRESKA SIMBOL VEC POSTOJI***"<<endl;
			}
}
	for(int i=0; i<simboli.size();i++){
		Symbol*e = simboli[i];
		if(e->name == naziv){
			if(e->scope==1 && e->sekcija=="UND"){
				//popravlja sekcije globalnim simbolima
				e->sekcija = trenSekcija;
			}
		}
	}
		table.insert(std::make_pair(naziv, new Symbol(naziv,trenSekcija, locationCounter,0,true)));	
}

void Asembler::printAllSectionsMemory(){
	for(int i=0; i<memorija.size();i++){
		cout<<memorija[i]->name<<endl;
		for(int j=0; j<memorija[i]->bytes.size();j++){
			cout<<hex<<setfill('0')<<setw(2)<<(int)(memorija[i]->bytes[j])<<" ";
		}
		cout<<endl;
	}
	cout<<"EOF"<<endl;
}

Symbol* Asembler::findSymbolInTable(string symb){

	   for(std::pair<const std::string, Symbol*> & sym : table)
    { 
					if(sym.first==symb){
						return sym.second;
					}
		 }
	return nullptr;
}

void Asembler::addSection(string sect){
			SectionElem*el = new SectionElem(sect);
			memorija.push_back(el);
}

void Asembler::fixExtern(){
	for(int i=0; i<simboli.size();i++){
		table[simboli[i]->name]->scope = simboli[i]->scope;
}
}

void Asembler::printSymbolTable(){

	cout << "Name    |" << "Section  |" << "Value |" << "Size   |" << "Scope     |" << "defined"<<endl;  
	  for(std::pair<const std::string, Symbol*> & itr : table)
    { 
			cout << itr.first<<" "<< itr.second->sekcija<< " "<< itr.second->value  << " "<< (itr.second->scope == 0 ? "local" : "global")
		<<" "<< (itr.second->def==true?"true":"false") << endl;
	}
	cout << endl;
}

void Asembler::printRelocation(){
					cout << "Relocation: "<<endl;
	for(int i=0; i<memorija.size(); i++){
				cout<< memorija[i]->name << endl;
		cout<<"name   |" << "offset  |" << "type"<< endl; 
		for(int j=0; j<memorija[i]->relocTable.size();j++){
			cout<<memorija[i]->relocTable[j].name<<" " << memorija[i]->relocTable[j].offset << " " << (memorija[i]->relocTable[j].type == 1 ? "R_X86_PC16" : "R_X86_16")<< endl; 
		} 
		
	cout<<endl;
	}
	cout<<"EOF"<<endl;
}



void Asembler::psGlobal(vector<std::string> vec){
  for(int i=0; i<vec.size();i++){
    markSymbolGlobal(vec[i]);
  }
}

void Asembler::psExtern(vector<std::string> vec){
    for(int i=0; i<vec.size();i++){
    markSymbolExtern(vec[i]);
  }
}
void Asembler::psSection(std::string sym){
    addSection(sym);
		trenSekcija = sym;
		locationCounter=0;
}
void Asembler::psWord(vector<std::string> vec){
for(int j=0; j<memorija.size();j++){
			if(memorija[j]->name == trenSekcija){
		for(int i=0 ; i<vec.size();i++){
			if(vec[i][0]>'a' && vec[i][0]<'z'){
    		Symbol*foundSym = findSymbolInTable(vec[i]);

				if(foundSym==nullptr){
				table.insert(std::make_pair(vec[i], new Symbol(vec[i],"UND",  0, 0, false)));
				}
				foundSym = findSymbolInTable(vec[i]);	

				memorija[j]->relocTable.push_back(RelocElem(vec[i],locationCounter, 0));
			

			unsigned char higher = foundSym->scope ? 0x00 : (foundSym->value & 0x0000FF00)/256;
			unsigned char lower = foundSym->scope ? 0x00 : foundSym->value;
        
				flink fl=flink(trenSekcija,locationCounter+3);

				foundSym->patch.push_back(fl);
				memorija[j]->bytes.push_back(lower);
				memorija[j]->bytes.push_back(higher);

		}else{
			int lit = stoi(vec[i]);

			unsigned char higher = (lit & 0x0000FF00) >> 8;
      unsigned char lower = (lit & 0x000000FF);
				memorija[j]->bytes.push_back(lower);
				memorija[j]->bytes.push_back(higher);

			
		}
		locationCounter+=2;
		}
	}
}
}
void Asembler::psSkip(int skip){
    locationCounter+=skip;
		for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
				for(int j=0 ; j<skip;j++)
				memorija[i]->bytes.push_back(0x00);
			}
		}
}

void Asembler::psEnd(){
    locationCounter=0;
}

void Asembler::psSym(std::string sym){
		addSymbol(sym);
}
   

void Asembler::psNot(int a){
for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
				memorija[i]->bytes.push_back(0x80);
				memorija[i]->bytes.push_back(a);
		}
	}
	locationCounter+=2;
}

void Asembler::psAdd(int a, int b){
for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
				memorija[i]->bytes.push_back(0x70);
				memorija[i]->bytes.push_back((a * 16) | (b));

		}
	}
	locationCounter+=2;
}

void Asembler::psXchg(int a, int b){
for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
				memorija[i]->bytes.push_back(0x60);
				memorija[i]->bytes.push_back((a * 16) | (b));

		}
	}
	locationCounter+=2;
}

void Asembler::psCmp(int a, int b){
for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
				memorija[i]->bytes.push_back(0x74);
				memorija[i]->bytes.push_back((a * 16) | (b));

		}
	}
	locationCounter+=2;
}

void Asembler::psAnd(int a, int b){
for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
				memorija[i]->bytes.push_back(0x81);
				memorija[i]->bytes.push_back((a * 16) | (b));

		}
	}
	locationCounter+=2;
}

void Asembler::psOr(int a, int b){
for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
				memorija[i]->bytes.push_back(0x82);
				memorija[i]->bytes.push_back((a * 16) | (b));

		}
	}
	locationCounter+=2;
}

void Asembler::psXor(int a, int b){
for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
				memorija[i]->bytes.push_back(0x83);
				memorija[i]->bytes.push_back((a * 16) | (b));

		}
	}
	locationCounter+=2;
}

void Asembler::psShl(int a, int b){
for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
				memorija[i]->bytes.push_back(0x90);
				memorija[i]->bytes.push_back((a * 16) | (b));

		}
	}
	locationCounter+=2;
}
void Asembler::psShr(int a, int b){
for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
				memorija[i]->bytes.push_back(0x91);
				memorija[i]->bytes.push_back((a * 16) | (b));

		}
	}
	locationCounter+=2;
}
void Asembler::psTest(int a, int b){
for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
				memorija[i]->bytes.push_back(0x84);
				memorija[i]->bytes.push_back((a * 16) | (b));

		}
	}
	locationCounter+=2;
}
void Asembler::psSub(int a, int b){
for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
				memorija[i]->bytes.push_back(0x71);
				memorija[i]->bytes.push_back((a * 16) | (b));

		}
	}
	locationCounter+=2;
}
void Asembler::psMul(int a, int b){
for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
				memorija[i]->bytes.push_back(0x72);
				memorija[i]->bytes.push_back((a * 16) | (b));

		}
	}
	locationCounter+=2;
}
void Asembler::psDiv(int a, int b){
for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
				memorija[i]->bytes.push_back(0x73);
				memorija[i]->bytes.push_back((a * 16) | (b));

		}
	}
	locationCounter+=2;
}
void Asembler::psHalt(){
		for(int i=0; i<Asembler::memorija.size();i++){
			if(memorija[i]->name == Asembler::trenSekcija){
				memorija[i]->bytes.push_back(0x00);
		}
}
		locationCounter+=1;
}

void Asembler::psInt(int a){
for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
				memorija[i]->bytes.push_back(0x10);
				memorija[i]->bytes.push_back((0xF0) | (a));
		}
	}
	locationCounter+=2;
}

void Asembler::psRet(){
for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
				memorija[i]->bytes.push_back(0x40);
		}
}
		locationCounter+=1;
}
void Asembler::psIret(){
for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
				memorija[i]->bytes.push_back(0x20);
		}
}
		locationCounter+=1;
}
void Asembler::psCall(int a, std::string sym, int lit){

	for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
				memorija[i]->bytes.push_back(0x30);
				//popravi za call ako nesto lose upisuje

				if(a==0){
				memorija[i]->bytes.push_back(0xF7);
				memorija[i]->bytes.push_back(0x05);

				Symbol*foundSym = findSymbolInTable(sym);
		
				if(foundSym==nullptr){
				table.insert(std::make_pair(sym, new Symbol(sym,"UND", 0, 0, false)));
				//pravimo relokacije
				}


				foundSym = findSymbolInTable(sym);

				memorija[i]->relocTable.push_back(RelocElem(sym,locationCounter+3, 1));
			

			unsigned char higher = foundSym->scope ? 0x00 : ((foundSym->value -2) & 0x0000FF00)/256;
			unsigned char lower = foundSym->scope ? 0x00 : foundSym->value-2;
        
				flink fl=flink(trenSekcija,locationCounter+3);

				foundSym->patch.push_back(fl);
				memorija[i]->bytes.push_back(lower);
				memorija[i]->bytes.push_back(higher);
				
				//povecamo na kraju za 2
				locationCounter+=2;			
				}
				else if(a==3 && sym=="lit"){
					memorija[i]->bytes.push_back(0xF0);
					memorija[i]->bytes.push_back(0x04);
					memorija[i]->bytes.push_back(lit);
					memorija[i]->bytes.push_back((lit & 0x0000FF00) /256);
					locationCounter+=2;
				}

			else if(a==4 && sym=="lit"){
					memorija[i]->bytes.push_back(0xF0);
					memorija[i]->bytes.push_back(0x00);
					memorija[i]->bytes.push_back(lit);
					memorija[i]->bytes.push_back((lit & 0x0000FF00) /256);
					locationCounter+=2;

			}

			//CALL SYMBOL - ABS ADRESIRANJE SIMBOL
			else if(a==4 && sym!="lit"){
				memorija[i]->bytes.push_back(0xF0);
				memorija[i]->bytes.push_back(0x00);

	
				Symbol*foundSym = findSymbolInTable(sym);
		
				if(foundSym==nullptr){
				table.insert(std::make_pair(sym, new Symbol(sym,"UND", 0, 0, false)));
				//pravimo relokacije
				}


				foundSym = findSymbolInTable(sym);

				memorija[i]->relocTable.push_back(RelocElem(sym,locationCounter+3, 0));
			

			unsigned char higher = foundSym->scope ? 0x00 : ((foundSym->value) & 0x0000FF00)/256;
			unsigned char lower = foundSym->scope ? 0x00 : foundSym->value;
        
				flink fl=flink(trenSekcija,locationCounter+3);

				foundSym->patch.push_back(fl);
				memorija[i]->bytes.push_back(lower);
				memorija[i]->bytes.push_back(higher);
				
				//povecamo na kraju za 2
				locationCounter+=2;	
			}
			else if(a==2 && sym=="lit"){
				//reg ind sa 16b pomeraj literla
				memorija[i]->bytes.push_back(0xF0);
				memorija[i]->bytes.push_back(0x03);
				memorija[i]->bytes.push_back(lit);
				memorija[i]->bytes.push_back((lit & 0x0000FF00) /256);
				locationCounter+=2;
			}
			else if(a==3 && sym=="reg"){
					memorija[i]->bytes.push_back((lit)|(0xF0));
					memorija[i]->bytes.push_back(0x02);

			}
			else if(a==1 && sym=="reg"){
					memorija[i]->bytes.push_back((lit)|(0xF0));
					memorija[i]->bytes.push_back(0x01);
			}

			else if(a==2 && sym!="lit" && sym!="reg"){
					memorija[i]->bytes.push_back((lit)|(0xF0));
					memorija[i]->bytes.push_back(0x03);
				
					Symbol*foundSym = findSymbolInTable(sym);
		
				if(foundSym==nullptr){
				table.insert(std::make_pair(sym, new Symbol(sym,"UND", 0, 0, false)));
				//pravimo relokacije
				}


				foundSym = findSymbolInTable(sym);

				memorija[i]->relocTable.push_back(RelocElem(sym,locationCounter+3, 0));
			

			unsigned char higher = foundSym->scope ? 0x00 : ((foundSym->value) & 0x0000FF00)/256;
			unsigned char lower = foundSym->scope ? 0x00 : foundSym->value;
        
				flink fl=flink(trenSekcija,locationCounter+3);

				foundSym->patch.push_back(fl);
				memorija[i]->bytes.push_back(lower);
				memorija[i]->bytes.push_back(higher);
				
				locationCounter+=2;	
			}
		}

		locationCounter+=3;
}

}
void Asembler::psLdr(int a,int b, int c,int adr, std::string sym){
		for(int i=0; i<memorija.size();i++){
					if(memorija[i]->name == trenSekcija){
						memorija[i]->bytes.push_back(0xA0);

				if(adr==3 && sym=="lit"){
					memorija[i]->bytes.push_back(((a *16) | (b)));
					memorija[i]->bytes.push_back(0x03);
					memorija[i]->bytes.push_back(c);
					memorija[i]->bytes.push_back((c & 0x0000FF00) /256);

					locationCounter+=5;
				}
				else if(adr==0 && sym!="reg" && sym!="hex" && sym!="lit"){
					//data DOLAR SYMB
					memorija[i]->bytes.push_back(((a *16) | (0x00))); //regs
					memorija[i]->bytes.push_back(0x00);


				Symbol*foundSym = findSymbolInTable(sym);
		
				if(foundSym==nullptr){
				table.insert(std::make_pair(sym, new Symbol(sym,"UND", 0, 0, false)));
				//pravimo relokacije
				}

				foundSym = findSymbolInTable(sym);

				memorija[i]->relocTable.push_back(RelocElem(sym,locationCounter+3, 0));
			

			unsigned char higher = foundSym->scope ? 0x00 : ((foundSym->value) & 0x0000FF00)/256;
			unsigned char lower = foundSym->scope ? 0x00 : foundSym->value;
        
				flink fl=flink(trenSekcija,locationCounter+3);

				foundSym->patch.push_back(fl);
				memorija[i]->bytes.push_back(lower);
				memorija[i]->bytes.push_back(higher);
				
				
				locationCounter+=5;
									}
				else if(adr==0 && sym=="hex" || sym=="lit"){
					memorija[i]->bytes.push_back(((a *16) | (0x00))); //regs
					memorija[i]->bytes.push_back(0x00);
					memorija[i]->bytes.push_back(b);
					memorija[i]->bytes.push_back((b & 0x0000FF00) /256);
					locationCounter+=5;

				}
					else if(adr==5 && sym!="reg" && sym!="hex" && sym!="lit"){
					memorija[i]->bytes.push_back(((a *16) | (0x00))); //regs
					memorija[i]->bytes.push_back(0x04);

			Symbol*foundSym = findSymbolInTable(sym);
		
				if(foundSym==nullptr){
				table.insert(std::make_pair(sym, new Symbol(sym,"UND", 0,  0, false)));
				//pravimo relokacije
				}


				foundSym = findSymbolInTable(sym);

				memorija[i]->relocTable.push_back(RelocElem(sym,locationCounter+3, 0));
			

			unsigned char higher = foundSym->scope ? 0x00 : ((foundSym->value) & 0x0000FF00)/256;
			unsigned char lower = foundSym->scope ? 0x00 : foundSym->value;
        
				flink fl=flink(trenSekcija,locationCounter+3);

				foundSym->patch.push_back(fl);
				memorija[i]->bytes.push_back(lower);
				memorija[i]->bytes.push_back(higher);
				
				//povecamo na kraju za 2
				
				locationCounter+=5;

				}else if(adr==4 && sym=="reg"){
					//regind 
					memorija[i]->bytes.push_back(b); //regs
					memorija[i]->bytes.push_back(0x02);

					locationCounter+=3;
				}

				else locationCounter+=3;
			}
		}
}
void Asembler::psStr(int a,int b, int adr, std::string sym){
for(int i=0; i<memorija.size();i++){
					if(memorija[i]->name == trenSekcija){
						memorija[i]->bytes.push_back(0xB0);


				if(adr==3 && sym=="lit"){
					cout<<"Udje ovde"<<endl;
									memorija[i]->bytes.push_back(((a *16) | (0x00))); //regs					

					memorija[i]->bytes.push_back(0x03);
				memorija[i]->bytes.push_back(0x00);
					memorija[i]->bytes.push_back(0x00);
					locationCounter+=5;
				}
				else if(adr==0 && sym!="reg" && sym!="hex" && sym!="lit"){
					//data DOLAR SYMB
					memorija[i]->bytes.push_back(((a *16) | (0x00))); //regs
					memorija[i]->bytes.push_back(0x00);

				Symbol*foundSym = findSymbolInTable(sym);
		
				if(foundSym==nullptr){
				table.insert(std::make_pair(sym, new Symbol(sym, "UND", 0, 0, false)));
				//pravimo relokacije
				}


				foundSym = findSymbolInTable(sym);

				memorija[i]->relocTable.push_back(RelocElem(sym,locationCounter+3, 0));
			

			unsigned char higher = foundSym->scope ? 0x00 : ((foundSym->value) & 0x0000FF00)/256;
			unsigned char lower = foundSym->scope ? 0x00 : foundSym->value;
        
				flink fl=flink(trenSekcija,locationCounter+3);

				foundSym->patch.push_back(fl);
				memorija[i]->bytes.push_back(lower);
				memorija[i]->bytes.push_back(higher);
				
				//povecamo na kraju za 2
				locationCounter+=5;

				}
				else if(adr==0 && sym=="hex"){
					memorija[i]->bytes.push_back(((a *16) | (0x00))); //regs
					memorija[i]->bytes.push_back(0x00);
					unsigned char higher = (b& 0x0000FF00) / 256;
    			unsigned char lower = (b & 0x000000FF);
					memorija[i]->bytes.push_back(lower);
					memorija[i]->bytes.push_back(higher);

					locationCounter+=5;

				}
				else if(adr==4 && sym=="reg"){
					//regind 
					memorija[i]->bytes.push_back((b)); //regs
					memorija[i]->bytes.push_back(0x02);


					locationCounter+=3;
				}
				else if(adr==5 && sym!="reg" && sym!="hex" && sym!="lit"){
					memorija[i]->bytes.push_back(((a *16) | (0x00))); //regs
					memorija[i]->bytes.push_back(0x04);
		

					Symbol*foundSym = findSymbolInTable(sym);
		
				if(foundSym==nullptr){
				table.insert(std::make_pair(sym, new Symbol(sym,"UND", 0,  0, false)));
				//pravimo relokacije
				}


				foundSym = findSymbolInTable(sym);

				memorija[i]->relocTable.push_back(RelocElem(sym,locationCounter+3, 0));
			

			unsigned char higher = foundSym->scope ? 0x00 : ((foundSym->value) & 0x0000FF00)/256;
			unsigned char lower = foundSym->scope ? 0x00 : foundSym->value;
        
				flink fl=flink(trenSekcija,locationCounter+3);

				foundSym->patch.push_back(fl);
				memorija[i]->bytes.push_back(lower);
				memorija[i]->bytes.push_back(higher);
				
				//povecamo na kraju za 2
				locationCounter+=5;	
				}
				else locationCounter+=3;
			}
		
		}
}
void Asembler::psJump(int a, std::string sym, int lit){
	for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
				memorija[i]->bytes.push_back(0x50);
				memorija[i]->bytes.push_back(0xF0);
				memorija[i]->bytes.push_back(0x00);
		

			if(a==4 && sym!="lit"){
	
				Symbol*foundSym = findSymbolInTable(sym);
		
				if(foundSym==nullptr){
				table.insert(std::make_pair(sym, new Symbol(sym, "UND", 0, 0, false)));
				//pravimo relokacije
				}


				foundSym = findSymbolInTable(sym);

				memorija[i]->relocTable.push_back(RelocElem(sym,locationCounter+3, 0));
			

			unsigned char higher = foundSym->scope ? 0x00 : ((foundSym->value) & 0x0000FF00)/256;
			unsigned char lower = foundSym->scope ? 0x00 : foundSym->value;
        
				flink fl=flink(trenSekcija,locationCounter+3);

				foundSym->patch.push_back(fl);
				memorija[i]->bytes.push_back(lower);
				memorija[i]->bytes.push_back(higher);
				
				//povecamo na kraju za 2
				locationCounter+=5;
			}
		}
}

}

void Asembler::psJne(int a, std::string sym, int lit){
	for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
				memorija[i]->bytes.push_back(0x52);
				memorija[i]->bytes.push_back(0xF0);
				memorija[i]->bytes.push_back(0x00);

		}
}
}

void Asembler::psJeq(int a, std::string sym, int lit){
	for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
			memorija[i]->bytes.push_back(0x51);
				memorija[i]->bytes.push_back(0xF0);
				memorija[i]->bytes.push_back(0x00);
		}
}
}
void Asembler::psJgt(int a, std::string sym, int lit){
	for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
				memorija[i]->bytes.push_back(0x53);
				memorija[i]->bytes.push_back(0xF0);
				memorija[i]->bytes.push_back(0x00);
		}
}
}

void Asembler::psPush(int a){
for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
				memorija[i]->bytes.push_back(0xB0);
				memorija[i]->bytes.push_back((a*16 | (0x6)));
				memorija[i]->bytes.push_back(0x12);
		}
	}
	locationCounter+=3;
}

void Asembler::psPop(int a){
for(int i=0; i<memorija.size();i++){
			if(memorija[i]->name == trenSekcija){
				memorija[i]->bytes.push_back(0xA0);
				memorija[i]->bytes.push_back((a*16 | (0x6)));
				memorija[i]->bytes.push_back(0x42);
		}
	}
	locationCounter+=3;
}