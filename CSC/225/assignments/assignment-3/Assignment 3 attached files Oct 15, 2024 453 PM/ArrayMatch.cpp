#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

int size = -1;
std::string filename = "";

bool compare_arrays(int *a, int *b, int size){
	for(int i = 0; i < size; i++)
		if(a[i] != b[i])
			return false;
	return true;
}
std::vector<std::vector<int> > chunks;
bool flag = false;
void compare(){
	if(flag) return;

	//for(size_t i = 0; i < chunks.size(); i++){
	//	std::cout<<"[ ";
	//	for(size_t j = 0; j < chunks.at(i).size(); j++){
	//		std::cout<<chunks.at(i).at(j)<<" ";
	//	}
	//	std::cout<<"], ";
	//}
	//std::cout<<std::endl;
	int half = (int)(chunks.size()/2);
	//std::cout << "half " <<half <<" size "<<chunks.size()<<std::endl;
	for(int i = 0, j = 1, k = half, l = half+1; j < half; i+=2, j+=2, k+=2, l+=2 ){
		std::vector<int> a1 = chunks.at(i);
		std::vector<int> a2 = chunks.at(j);
		std::vector<int> b1 = chunks.at(k);
		std::vector<int> b2 = chunks.at(l);


		size_t m = 0, n = 0, o = 0, p = 0;
		while(m < a1.size() && n < a2.size() && o < b1.size() && p < b2.size()){
			//std::cout << "a1[n]: " << a1.at(n) << " b1[n]: " << b1.at(n)<<"\n" 
			//<< " a2[n]: " << a2.at(n) << " b2[n]: " << b2.at(n) << std::endl;
			//a1 == b1 && a2 == b2
			if(a1.at(m) ==  b1.at(m) && a2.at(m) == b2.at(m)){
				m++;
				continue;
			}
			//a1 == b1 && a1 == b2
			else if(a1.at(n) == b1.at(n) && a1.at(n) == b2.at(n)){
				n++;
				continue;
			}
			//a2 == b1 && a2 == b2
			else if(a2.at(o) == b1.at(o) && a2.at(o) == b2.at(o)){
				o++;
				continue;
			}
			else p++;
		}

		//std::cout<<"m "<<m<<"\n"<<"n "<<n<<"\n"<<"o "<<o<<"\n"<<"p "<<p<<std::endl;
		if(m == a1.size() || n == a1.size() || o == a1.size()){
			flag = true;
			return;
		}
	}
	flag = false;

	
//	if(l == a1.size() * 4){
//		std::cout << "NO" << std::endl;
//		return false;
//	}

	//return true;
}
std::vector<int> mergesort(std::vector<int> s){
	
	if(s.size() == 2){
		chunks.push_back(s);
		compare();
		return s;
	
	}
	int half = s.size() / 2;
	std::vector<int> s1(s.begin(), s.begin() + half);
	std::vector<int> s2(s.begin() + half, s.end());
	
	s1 = mergesort(s1);
	s2 = mergesort(s2);

	return s;
}

bool match(int a[], int b[]){

	if(size % 2 != 0 || size < 8)
		return compare_arrays(a, b, size);
	//guaranteed to have full tree
	std::vector<int> merged;
	merged.insert(merged.end(), a, a + size);
	merged.insert(merged.end(), b, b + size);
	mergesort(merged);
	return flag;
}

int main(int argc, char *argv[]) {
	if(argc < 2){
		std::cerr << "Please input a filename \n";
		return 1;
	}
	
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


	std::cout << ((match(a.data(), b.data())) ? "YES" : "NO") ;
	//match(a.data(), b.data());
	return 0;
}
