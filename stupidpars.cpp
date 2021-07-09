#include <iostream>
#include <string>
#include <fstream>
#include <iostream>

int main(){
std::vector<float> coords, normals;
std::vector<int> tris, solids;
return 0;
}

void parse_stl(string name, std::vector &coords,std::vector &normals,std::vector &tris,std::vector &solids)
{
	std::fstream my_file;
	my_file.open(name, ios::in | ios::binary);
	if (!my_file){
		std::cout<< "No such file"<<std::endl;
		exit("fail");
	}
	try {
		uint32 x = num_of_tris;
		my_file.seekp(80,ios::beg);
		my_file.read()

	}
}

