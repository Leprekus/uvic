#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

std::string filename;
std::string line;
bool debug_mode = false;
bool flag = false;
int size = -1;
bool compare_arrays(int a[], int b[], int size) {
    for (int i = 0; i < size; ++i) {
        if (a[i] != b[i]) {
            return false;  
        }
    }
    return true;  
}

bool mergesort(std::vector<int> a, std::vector<int>b){
	if(debug_mode)
		std::cout << "mergesort() state: "<<std::endl;
	
	if(flag) return flag;
	
	if(debug_mode)
		std::cout << "a size: " << a.size() << " b size: " << b.size() << std::endl;

	//compare once we've reached the minimum size 
	//2 or when the size is no longer divisible by 2
	if( (a.size() % 2 != 0) || (a.size() == 2) ){
		//chunks_a.push_back(a);
		//chunks_b.push_back(b);
		//compare();
		for(size_t i = 0; i < b.size(); i++){
			if(debug_mode)
				std::cout<< a.at(i) << " -> "<< b.at(i)<<std::endl;;
			if(a.at(i) != b.at(i))
				return false;
		}
			
		return true;
	
	} 

	int half = a.size() / 2;
	//if(half % 2 != 0) {
	//	flag = false;
	//	return flag;
	//}
	std::vector<int> a1(a.begin(), a.begin() + half);
	std::vector<int> a2(a.begin() + half, a.end());
	std::vector<int> b1(b.begin(), b.begin() + half);
	std::vector<int> b2(b.begin() + half, b.end());
	bool a1_b1 = mergesort(a1, b1);
	bool a2_b2 = mergesort(a2, b2);
	bool a1_b2 = mergesort(a1, b2);
	bool a2_b1 = mergesort(a2, b1);
	bool res = (a1_b1 && a2_b2) || (a1_b1 && a1_b2)|| (a2_b1 && a2_b2);
	if(res)
		flag = true;
	return res;
	//bool a1_b2 = mergesort(a1, b2);
	//bool a2_b1 = mergesort(a2, b1);

	//if(a1_b1 && a2_b2)
	//	return true;
	//if(a1_b1 && a1_b2)
	//	return true;
	//if(a2_b1 && a2_b2)
	//	return true;
	//return false;
	
}

bool match(int raw_a[], int raw_b[]){

	if(debug_mode){
		std::cout << "match() initial state:" << std::endl;
		std::cout << "size: " << size << std::endl;
		std::cout << "size is odd: " << ((size%2 != 0) ? "true" : "false") << std::endl;
		std::cout << "or" << std::endl;
		std::cout << "size less than 8: " << ((size < 8) ? "true" : "false") << std::endl;

	}

	if(size % 2 != 0 || size < 8)
		return compare_arrays(raw_a, raw_b, size);
	//guaranteed to have full tree
	//std::vector<int> merged;
	std::vector<int> a(raw_a, raw_a + size);
	std::vector<int> b(raw_b, raw_b + size);
	//merged.insert(merged.end(), b, b + size);
	return mergesort(a, b);
	//std::cout << (flag ? "YES" : "NO") ;
//	return flag;
}

int main(int argc, char *argv[]) {
	if(argc < 2){
		std::cerr << "Please input a filename \n";
		return 1;
	}
	if(argc == 3 ){
		std::string optional_flag = argv[2];
		if(optional_flag == "--debug")
			debug_mode = true;
	}
	if(debug_mode)
		std::cout << "running in debugging mode."<<std::endl;
	
	std::string line;
	std::vector<int> a;
	std::vector<int> b;
	int line_count = 0;
	filename = argv[1];
	std::ifstream InputFile(argv[1]);

	while(getline(InputFile, line)){
		line_count++;
		std::stringstream stream(line);
		std::string token;
		while(getline(stream, token, ' ')){
			if(line_count == 1)
				size = std::atoi(token.c_str());
			if(line_count == 2)
				a.push_back(std::atoi(token.c_str()));
			if(line_count == 3)
				b.push_back(std::atoi(token.c_str()));
		} 
		

	}	
	InputFile.close();


	std::cout << ((match(a.data(), b.data())) ? "YES" : "NO")<< std::endl;
	//match(a.data(), b.data());
	//std::cout<<" end :D";
	return 0;
}
