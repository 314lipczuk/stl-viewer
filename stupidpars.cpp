#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
bool isAscii(std::fstream &my_file);
void parseAscii(std::string &name, std::vector<float> &coords,std::vector<float> &normals,std::vector<int> &tris,std::vector<int> &solids,std::fstream &my_file);
void parseBinary(std::string &name, std::vector<float> &coords,std::vector<float> &normals,std::vector<int> &tris,std::vector<int> &solids,std::fstream &my_file);

void parse_stl(std::string name, std::vector<float> coords,std::vector<float> normals,std::vector<int> tris,std::vector<int> solids)
{
	std::fstream my_file;
	my_file.open(name, std::ios::binary | std::ios::in);
	if (!my_file){
		std::cout<< "File fail"<<std::endl;
	}
	if(isAscii(my_file)){
	std::cout<< "Parsing ascii file" <<std::endl;
	}else{
	std::cout<< "Parsing binary file" <<std::endl;
	}
}

void parseAscii(std::string &name, std::vector<float> &coords,std::vector<float> &normals,std::vector<int> &tris,std::vector<int> &solids , std::fstream &my_file){
	while(1){
	
	}	
}
bool isAscii(std::fstream &my_file){
	try{
		std::string line;
		std::getline(my_file, line);
		if( std::regex_match(line, std::regex("(.*)(solid)(.*)")))
			return true;
		return false;
	}
	catch(...)
	{
	std::cout<<"regex fuckup"<<std::endl;
	}
}

int main(){
	std::vector<float> coords, normals;
	std::vector<int> tris, solids;
	parse_stl("basic.stl", coords, normals, tris, solids);
	return 0;
}
