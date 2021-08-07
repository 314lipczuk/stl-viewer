#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include <chrono>
/*
===TODO===
* implement parsing for multiple solids in same file
*/

namespace stupidpars{
bool isAscii(std::fstream &my_file);

void parse_binary(std::fstream & my_file, std::vector<float> &coords, int &tris);
void parseLine(std::string &line, std::vector<float> &coords,int &tris, std::regex (&regexes)[3]){
  std::regex rg("(facet)(.*)");
	if (std::regex_match( line, regexes[0] )){
    tris++;
  }
	if( std::regex_match( line,regexes[1] )){
    std::sregex_iterator next(line.begin(), line.end(),regexes[2]);
    std::sregex_iterator end;
    while(next!=end){
      std::smatch match = *next;
      coords.push_back(std::stof(match.str()));
      next++;
	    }
	}
	
}
void parse_stl(std::string name, std::vector<float> &coords, int &tris)
  {
	  std::regex re_facet("(facet)(.*)");	
	  std::regex re_vertex("(.*)(vertex)(.*)");
	  std::regex re_float("(-?[0-9]+\.[0-9]+)");
	  std::regex re_ar[3] = {re_facet, re_vertex, re_float};
	  std::fstream my_file;
	  my_file.open(name, std::ios::binary | std::ios::in);
	  if (!my_file){
	  	std::cout<< "File fail"<<std::endl;
	  	std::exit(1);
	  }
	  if(isAscii(my_file)){
	  	std::cout<< "Parsing ascii file" <<std::endl;
	  	std::string line;
	  	while (std::getline(my_file, line)){
	  		parseLine( line, coords, tris, re_ar);
	  	}
	  }else{
	  	std::cout<< "Parsing binary file" <<std::endl;
	  	parse_binary(my_file,coords, tris );
	  	
	  }
	  my_file.close();
  }
bool isAscii(std::fstream &my_file)
  {
	  try{
	  	std::string line;
	  	std::getline(my_file, line);
	  	if(std::regex_match(line, std::regex("(.*)(solid)(.*)"))){
	  		return true;
      }
	  	return false;
	  }
	  catch(...)
	  {
	  std::cout<<"regex at call to isAscii() fuckup"<<std::endl;
    std::exit(1);
	  }
  }
void parse_binary(std::fstream & my_file, std::vector<float> &coords, int &tris)
	{
		uint32_t a;
		my_file.seekg(80);
		my_file.read(reinterpret_cast<char *>(&a) , sizeof(a));
    tris = a;
		for(int i = 0; i<a; i++){
			my_file.ignore(12);
			for(int j = 0;j<9;j++){
				float x;
				my_file.read(reinterpret_cast<char *>(&x), sizeof(x));
				coords.push_back(x);
			}
      my_file.ignore(2);
		}
  }
