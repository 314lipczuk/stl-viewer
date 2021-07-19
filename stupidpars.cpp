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

bool isAscii(std::fstream &my_file);
void parseLine(std::string &line, std::vector<float> &coords,std::vector<float> &normals,std::vector<int> &tris,std::vector<int> &solids, tri &curtri );

struct tri{
	float[3] norm;
	float** points; 
	tri(float[3] norm);
	~tri();
  int num_points;
};
tri::tri(float[3] norm){
  this->num_points=0;
  this->norm = norm;
  this->points = new float*[3];
  for(int i=0;i<3;i++){
    this->points[i] = new float[3];
    for(int j=0;j<3,j++){
      this->points[i][j]=0.0;
    }
  }
}
tri::~tri(){
  for(int i=0;i<3;i++){
    delete[] this->points[i];
  }
  delete[] this->points;
}
void parse_stl(std::string name, std::vector<float> coords,std::vector<float> normals,std::vector<int> tris,std::vector<int> solids)
{
	std::fstream my_file;
	my_file.open(name, std::ios::binary | std::ios::in);
	if (!my_file){
		std::cout<< "File fail"<<std::endl;
	}
	if(isAscii(my_file)){
		std::cout<< "Parsing ascii file" <<std::endl;
		std::string line;
		//tri curtri;
		while (std::getline(my_file, line)){
			std::cout<<line<<std::endl;
			parseLine(&line, &coords, &normals, &tris, &solids, &curtri);
		}
	}else{
	std::cout<< "Parsing binary file" <<std::endl;
		
	}
}
void parseLine(std::string &line,std::string &name, std::vector<float> &coords,std::vector<float> &normals,std::vector<int> &tris,std::vector<int> &solids ){
  std::regexp("(-?[0-9]+\.[0-9]+)");
  std::sregex_iterator next(line.begin(), line.end(), regexp);
  std::sregex_iterator end;
  float tmp[3]={};
  int floatnum=0;

	if( std::regex_match( line, std::regex("(*)(normal)(*)"))){
    while(next!=end){
      std::smatch match = *next;
      tmp[floatnum] = std::stof(match.str());
      next++;
      floatnum++;
	  }
  
  }
	if( std::regex_match( line, std::regex("(*)(vertex)(*)"))){
		std::cout<<""<<std::endl;	
	}
	if( std::regex_match( line, std::regex("(*)(endfacet)(*)"))){
		std::cout<<"normal"<<std::endl;	
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
