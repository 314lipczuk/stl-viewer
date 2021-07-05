#include <iostream>
#include <string>
#include <fstream>
#include <iostream>

int main(){
std::vector<float> coords, normals;
std::vector<int> tris, solids;
return 0;
}

float[] parse_stl(string name, std::vector &coords,std::vector &normals,std::vector &tris,std::vector &solids)
{
	std::fstream my_file;
	my_file.open(name, ios::in);
	std::string linestr;
	if (!my_file){
		std::cout<< "No such file"<<std::endl;
	}
	else{
		while(1){
		
		}
	}
}

