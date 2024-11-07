#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

std::string filename;
int main(int argc, char *argv[]) {
	
	if(argc < 2){
		std::cerr << "Please input a filename \n";
		return 1;
	}
		
	std::string line;
	std::vector<int> nums;
	// all numbers are positive so if an index == -1 we know it's uninitialized
	const int MAGIC_NUMBER = 225;
	std::vector<int> window(MAGIC_NUMBER + 1, -1);
	filename = argv[1];
	std::ifstream InputFile(argv[1]);

	while(getline(InputFile, line)){
		std::stringstream stream(line);
		std::string token;
		while(getline(stream, token, ' '))
				nums.push_back(std::atoi(token.c_str()));
		

	}	
	InputFile.close();

	for(int i = 0; i < nums.size(); i++){
		//to be valid a num must be <= 255 since all are positive ints
		if(nums.at(i) > MAGIC_NUMBER)
			continue;
		int diff = MAGIC_NUMBER - nums.at(i);
		
		if(diff < 0 || diff > 225)
			continue;
		//in this case we have seen the "missing" number and can safely say it's true
		if( window.at(diff) != -1 && (window.at(diff)+nums.at(i)) == MAGIC_NUMBER ){
			std::cout << nums.at(i) << " " << window.at(diff) << std::endl;
			std::cout << "true" << std::endl;
			return 0;
		}

		//otherwise we store the number so it becomes the "missing" number of a future diff
		if( 0 <= nums.at(i) && nums.at(i) <= MAGIC_NUMBER)
			window[nums.at(i)] = nums.at(i);
	}


	//for(int i = 0; i < nums.size(); i++){
	//	//std::cout<<nums.at(i)<<std::endl;
	//	for(int j = 0; j < nums.size(); j++)
	//		if(nums.at(i) + nums.at(j) == MAGIC_NUMBER){
	//			std::cout << "brute true" << std::endl;
	//			return 0;
	//		}
	//}
	std::cout << "false" << std::endl;
	return 0;
}
