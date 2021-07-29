#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
/*
===TODO===
*parsing binary
*parseline func with tri struct
*/
namespace stupidpars{
bool isAscii(std::fstream &my_file);
void parseLine(std::string &line, std::vector<float>& coords,int& tris){
  std::regex rg("(facet)(.*)");
	if (std::regex_match( line, rg )){
    tris++;
  }
	if( std::regex_match( line, std::regex("(.*)(vertex)(.*)"))){
    std::regex regexp("(-?[0-9]+\.[0-9]+)");
    std::sregex_iterator next(line.begin(), line.end(), regexp);
    std::sregex_iterator end;
    while(next!=end){
      std::smatch match = *next;
      coords.push_back(std::stof(match.str()));
      next++;
	    }
	}
	/*if( std::regex_match( line, std::regex("(.*)(endfacet)(.*)"))){
		std::cout<<"normal match"<<std::endl;	
	}*/
}
void parse_stl(std::string name, std::vector<float> &coords, int& tris)
{
	std::fstream my_file;
	my_file.open(name, std::ios::binary | std::ios::in);
	if (!my_file){
		std::cout<< "File fail"<<std::endl;
	}
	if(isAscii(my_file)){
		std::cout<< "Parsing ascii file" <<std::endl; std::string line;
		while (std::getline(my_file, line)){
			parseLine( line, coords, tris);
		}
	}else{
	std::cout<< "Parsing binary file" <<std::endl;
	}
}
bool isAscii(std::fstream &my_file){
	try{
		std::string line;
		std::getline(my_file, line);
		if( std::regex_match(line, std::regex("(.*)(solid)(.*)"))){
			return true;
    }
		return false;
	}
	catch(...)
	{
	std::cout<<"regex fuckup"<<std::endl;
	}
}
}
