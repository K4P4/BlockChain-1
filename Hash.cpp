#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <stdint.h>
#include <limits.h>
#include <chrono>
#include <vector>
#include <algorithm>

using std::vector;
using std::string;
using std::cout;
using std::endl;
using namespace std::chrono;

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
    std::ifstream input(validateFileName());
    string data;

    input.seekg(0, std::ios::end);
    data.reserve(input.tellg());
    input.seekg(0, std::ios::beg);

    data.assign((std::istreambuf_iterator<char>(input)),std::istreambuf_iterator<char>());

    input.close();
    return data;
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

uint64_t** convertDecimal(string s, int & n, int & sizeOfMessage){
    sizeOfMessage = s.size();

	n = sizeOfMessage/32;
	if(sizeOfMessage%32 == 0) n--;

	for(int i = 0; i < (n+1)*32-sizeOfMessage; i++) s+="0";

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

uint64_t hashCycle(uint64_t ** decimalArray, int k, int sizeOfMessage){
    uint64_t result = 241074407325576511301;
    for(int i = 0; i < 4; i++){
        decimalArray[k][i] = decimalArray[k][i] ^ (result*sizeOfMessage);
    }
    for(int l = 0; l < 4; l++){
            result = result + rotl(decimalArray[k][l], 7);
            rotr(result, 7);
    for(int j = 0; j < 4; j++){
            result = result ^ rotl(decimalArray[k][j], 5);
            rotr(result, j*3);
    for(int i = 0; i < 4; i++){
            result = result * rotl(decimalArray[k][i], 3);
            rotr(result, i);
    }
    }
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

string hashFunction(string s){
    int n = 0;
    int sizeOfMessage = 0;

    uint64_t** myArray = convertDecimal(s, n, sizeOfMessage);

    uint64_t result = hashCycle(myArray, 0, sizeOfMessage);
    for(int i = 1; i < n; i++){
        result = hashCycle(myArray, i, sizeOfMessage) ^ result;
    }
    return n2hexstr(result);
}

uint64_t hashFunctionInt(string s){
    int n = 0;
    int sizeOfMessage = 0;

    uint64_t** myArray = convertDecimal(s, n, sizeOfMessage);

    uint64_t result = hashCycle(myArray, 0, sizeOfMessage);
    for(int i = 1; i < n; i++){
        result = hashCycle(myArray, i, sizeOfMessage) ^ result;
    }
    return result;
}

void testSpeed(){
    string message;
    int sizeOfMessage;
    int n = 0;
    double laikas = 0;
    std::ifstream in(validateFileName());
    while(std::getline(in, message)){
        auto start = high_resolution_clock::now();
        uint64_t** myArray = convertDecimal(message, n, sizeOfMessage);

        uint64_t result = hashCycle(myArray, 0, sizeOfMessage);
        for(int i = 1; i < n; i++){
        result = hashCycle(myArray, i, sizeOfMessage) ^ result;
        auto ending = high_resolution_clock::now();
        duration<double> diff = ending-start;
        laikas += diff.count();
    }
    //cout << n2hexstr(result) << endl;
    }
    cout << "Time elapsed: " << laikas << "s" << endl;
}

void checkPairs(){
    std::ifstream in("1mil.txt");
    string input;
    int collisionAmount=0;
    getline(in, input);
    string previous = hashFunction(input);
    string current;
    while(getline(in, input)){
        current = hashFunction(input);
        if(current == previous) collisionAmount++;
        current = previous;
    }
    cout << collisionAmount << " collisions detected" << endl;
}

int compareBits(uint64_t A, uint64_t B)
{
    int differentBits = 0;
    for (int i = 0; i < 64; i++){
        if (((A >> i) & 1) != ((B >> i) & 1)){
            differentBits++;
        }
    }
    return differentBits;
}

checkPercentage(){
    std::ifstream in("1mil_2.txt");
    string input;
    getline(in, input);
    uint64_t previous = hashFunctionInt(input);
    uint64_t current;
    int iterations = 0;
    int sum = 0;
    int minPerc = 100;
    int maxPerc = 0;
    while(getline(in, input))
    {
        current = hashFunctionInt(input);
        int temp = 0;

        temp = compareBits(current, previous);

        if(temp < minPerc){
            minPerc = temp;
        }
        if (temp > maxPerc){
            maxPerc = temp;
        }
        sum += temp;
        iterations++;
        previous = current;
    }

    double avg = (double)sum/(double)iterations;
    double maxP = (double)maxPerc/64*100;
    double minP = (double)minPerc/64*100;
    cout << minPerc << endl;
    cout << "Vidurkis: " << avg / 64*100 << "%"<<endl;
    cout << "Max: " << maxP << "%"<<endl;
    cout << "Min: " << minP << "%"<<endl;
    in.close();

}

int main()
{
    int selection;
    cout << "Do you wish to hash or test the algorithm?" << endl;
    cout << "1. Hash a message." << endl;
    cout << "2. Test the algorithm speed." << endl;
    cout << "3. Check the amount of collisions out of a million pairs." << endl;
    cout << "4. Check the similarity out of a million hashes." << endl;
	std::cin >> selection;
	switch (selection) {
	case 1: cout << hashFunction(getInput());
        break;
	case 2: testSpeed();
		break;
    case 3: checkPairs();
        break;
    case 4: checkPercentage();
        break;
	default:
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	    cout << "Error. Invalid selection." << endl;
		break;
	}
}
