#include <iostream>
#include <fstream>
bool match(int a[], int b){
	return true;
}

int main(int argc, char *argv[]) {
	if(argc < 2){
		std::cerr << "Please input a filename \n";
		return 1;
	}
	
	std::string text;
	std::ifstream InputFile(argv[1]);
	while(getline(InputFile, text)){
		std::cout << text;
	}	
	InputFile.close();
	return 0;
}
