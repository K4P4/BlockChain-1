#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <stdint.h>
#include <limits.h>

using std::string;
using std::cout;
using std::endl;



string validateFileName();

string readConsole(){
    string message;
    cout << "Enter the message:" << endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, message);
    return message;
}

string readFile(){
    string message;
    std::ifstream in(validateFileName());
    std::getline(in, message);
    return message;
}

string getInput(){
    int selection;
    cout << "How do you wish to enter the message? (Select by entering the number)" << endl;
    cout << "1. In the console." << endl;
    cout << "2. Read from file." << endl;
	std::cin >> selection;
	switch (selection) {
	case 1: return readConsole();
		break;
	case 2: return readFile();
		break;
	default:
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	    cout << "Error. Invalid selection." << endl;
		return getInput();
		break;
	}
}

string validateFileName() {
	string fileName;
	while (true) {
		std::cout << "Enter the file name: ";
		std::cin >> fileName;
		std::ifstream in(fileName);
		if (!in.good()) {
			std::cerr << "Error. File does not exist." << std::endl;
		}
		else {
			in.close();
			return fileName;
			break;
		}
	}
}

uint64_t** convertDecimal(string s, int & n){
    int sizeOfMessage = s.size();

	n = sizeOfMessage/32;
	if(sizeOfMessage%32 == 0) n--;

	for(int i = 0; i < (n+1)*32-sizeOfMessage; i++) s+="0";
	cout << s << endl;


	uint64_t** decimalArray = 0;
	decimalArray = new uint64_t*[n+1];
	int pos = 0;

	for (int h = 0; h < n+1; h++)
      {
            decimalArray[h] = new uint64_t[4];

            for (int w = 0; w < 4; w++)
            {
                string temp = s.substr(pos, 8);

                uint64_t ans = (uint64_t)temp[0];
                for(int i = 1; i < 8; i++){
                    ans = ans | (uint64_t)(temp[i] << 8*i);
                }
                decimalArray[h][w] = ans;
                pos+=8;
            }
      }
	return decimalArray;
}

uint64_t rotl (uint64_t n, unsigned int c)
{
  const unsigned int mask = (CHAR_BIT*sizeof(n) - 1);
  c &= mask;
  return (n<<c) | (n>>( (-c)&mask ));
}

uint64_t rotr (uint64_t n, unsigned int c)
{
  const unsigned int mask = (CHAR_BIT*sizeof(n) - 1);
  c &= mask;
  return (n>>c) | (n<<( (-c)&mask ));
}

uint64_t hashCycle(uint64_t ** decimalArray, int k){
    uint64_t result;
    for(int i = 1; i < 4; i++){
    result = 2440764073279551311;
    result = result ^ decimalArray[k][0];
    result = rotr(result, i);
    result = result ^ decimalArray[k][1];
    result = rotr(result, i);
    result = result ^ decimalArray[k][2];
    result = rotr(result, i);
    result = result ^ decimalArray[k][3];
    result = rotr(result, i);
    }
    return result;
}

template <typename I> std::string n2hexstr(I w, size_t hex_len = sizeof(I)<<1) {
    static const char* digits = "0123456789ABCDEF";
    std::string rc(hex_len,'0');
    for (size_t i=0, j=(hex_len-1)*4 ; i<hex_len; ++i,j-=4)
        rc[i] = digits[(w>>j) & 0x0f];
    return rc;
}

int main()
{
    for(int i = 0; i < 3; i++){
    int n = 0;
    uint64_t** myArray = convertDecimal(getInput(), n);

      uint64_t result = hashCycle(myArray, 0);
      cout << n2hexstr(result) << endl;
    }
    return 0;
}
