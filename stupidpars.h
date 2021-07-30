#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include <chrono>
/*
===TODO===
*parsing binary
*parseline func with tri struct
*/

namespace stupidpars{
using Clock = std::chrono::steady_clock;
bool isAscii(std::fstream &my_file);

void parseLine(std::string &line, std::vector<double>& coords,int& tris, std::regex (&regexes)[3]){
  std::regex rg("(facet)(.*)");
	//if (std::regex_match( line, rg )){
	if (std::regex_match( line, regexes[0] )){
    tris++;
  }
	//if( std::regex_match( line, std::regex("(.*)(vertex)(.*)"))){
	if( std::regex_match( line,regexes[1] )){
    //std::regex regexp("(-?[0-9]+\.[0-9]+)");
    std::sregex_iterator next(line.begin(), line.end(),regexes[2]);
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
void parse_stl(std::string name, std::vector<double> &coords, int& tris)
{
	auto tic = Clock::now();
	std::regex re_facet("(facet)(.*)");	
	std::regex re_vertex("(.*)(vertex)(.*)");
	std::regex re_float("(-?[0-9]+\.[0-9]+)");
	std::regex re_ar[3] = {re_facet, re_vertex, re_float};

	std::fstream my_file;
	my_file.open(name, std::ios::binary | std::ios::in);
	if (!my_file){
		std::cout<< "File fail"<<std::endl;
	}
	if(isAscii(my_file)){
		std::cout<< "Parsing ascii file" <<std::endl; std::string line;
		while (std::getline(my_file, line)){
			parseLine( line, coords, tris, re_ar);
		}
	}else{
	std::cout<< "Parsing binary file" <<std::endl;
	}
	my_file.close();
	auto toc = Clock::now();
	std::cout << "stupidpars.h	Elapsed time: " << std::chrono::duration_cast<std::chrono::milliseconds>(toc - tic).count()<<std::endl;
}
bool isAscii(std::fstream &my_file){
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
	}
}
}
