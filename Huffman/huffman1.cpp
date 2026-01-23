#include <vector>
#include <iostream>
#include <fstream>
#include <format>
#include <set>
#include <map>
#include <algorithm>	//std:.sort
#include <utility>
#include <string>

/*
Write a command line program in C++ with this syntax:
huffman1[c | d] <input file> <output file>

*/


// #==========# bitreader #==========#
template <typename T>
std::istream& raw_read(std::istream& is, T& buffer, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast< char*>(&buffer), size);
}

class BitReader{
	std::istream& is_;
	uint8_t buffer_ = 0;
	size_t n_ = 0; 

	uint32_t readbit() {						//uint32_t
		if (n_ == 0) {
			raw_read(is_, buffer_);
			n_ = 8;								//n=8;
		}
		-- n_;
		return (buffer_ >> n_) & 1;				//& 1
	}
public:
	BitReader(std::istream& is) : is_(is) {};

	std::istream& operator()(uint32_t& u, size_t n) {
		u = 0;
		while (n-- > 0) {
			u = (u << 1) | readbit();			// u << 1
		}

		return is_;
	}

};

// #==========# bitwriter #==========#
template< typename T >
std::ostream& raw_write(std::ostream& os, T& val, size_t size = sizeof(T)) {
	return os.write(reinterpret_cast<const char*>(&val), size);
}
class BitWriter{
	std::ostream& os_;
	size_t n_ = 0;
	uint8_t buffer_ = 0;

	void writebit(uint32_t bit) {
		++n_;
		buffer_ = (buffer_ << 1) | (bit & 1);
		if (n_ == 8) {
			raw_write(os_, buffer_);
			n_ = 0;
		}

	}

public:
	BitWriter(std::ostream& os) : os_(os) {};
	~BitWriter() {
		flush();
	}

	std::ostream& operator()(uint32_t u, size_t n) {
		while (n -- > 0) {
			writebit(u >> n);
		}
		return os_;
	}

	std::ostream& flush(uint32_t bit = 0) {
		while (n_ > 0) {
			writebit(bit);
		}
		return os_;
	}
};


// #==========# Nodo (a single node in italian ) #==========#
struct Nodo {
	float freq_ = 0;							//freq of node
	std::vector<uint8_t> simbol_;				// i need a way to collect different simbols from previous node uint8_t simbol_ = 0;						//simbol of the node
	Nodo* left_penultimo_0_ = nullptr;			//indirizzo 1				(son1)		(if doesn't leaf)
	Nodo* right_ultimo_1_ = nullptr;			//indirizzo 2				(son2)		(if doesn't leaf)

	Nodo(std::vector<uint8_t> simbol, float freq, Nodo* left = nullptr, Nodo* right = nullptr) : simbol_(simbol) , freq_(freq) , left_penultimo_0_(left) , right_ultimo_1_(right)  {

		freq_ = compute_frequence(left ,right, freq_);
		compute_symbols(left, right, simbol_);

	};

	float compute_frequence(Nodo* left, Nodo* right, float& freq) {
		if ((left) && (right)) {
			return (left->freq_ + right->freq_);
		}
		return freq_;
	}
	void compute_symbols(Nodo* left, Nodo* right, std::vector<uint8_t>& symbols) {
		if ((left) && (right)) {
			std::copy(left->simbol_.begin(), left->simbol_.end(), symbols.begin());
			std::copy(right->simbol_.begin(), right->simbol_.end(), symbols.begin());
		}
	}
	
};

 void make_table(std::istream& is, size_t& total, std::map<uint8_t, float>& table) {

	BitReader br(is); 
	uint32_t byte;    

	total = 0;        

	while (br(byte, 8)) {
		// 1. Convertion
		uint8_t key = static_cast<uint8_t>(byte);
		// 2. Incrementation for specific byte, if doesn'exist map create it with zero value, than ++
		table[key]++;
		// 3. number of byte
		total++;
	}
}

void compute_probability(std::map<uint8_t, float>& table, size_t& numbers) {
	for (auto& x : table) {
		x.second /= numbers;
	}
}

void ordering_elements(std::vector< std::pair<uint8_t, float> >& couples) {
	std::sort(couples.begin(), couples.end(), 
		/*how to compare two element of the vector?*/
		[](const std::pair<uint8_t , float >& a ,std::pair<uint8_t , float >& b ) {					//taking two vectorr's elements 
			return a.second > b.second;																//i'll get a vector that starts from higer values and end with lower --> to use pop_back;
			//std::get<1>(couples.at(0)) < std::get<1>(couples.at(1));	 big ERROR: 
			//															|return missing|is static| moreover i tried to work with std::vector and not its element!
			//saying "std::get<1>(a)" is the same of "a.second"
		});
}

//function to create the HuffmanTable
void create_table(const Nodo* nodo , std::string code , std::map<uint8_t , std::string>& table  ) {
	
	if ((!nodo->left_penultimo_0_) || (!nodo->right_ultimo_1_)) {
		//leaf
		table[nodo->simbol_.at(0)] = code;
		return;
	}
	else if ((nodo->left_penultimo_0_) || (nodo->right_ultimo_1_)) {
		//intermedial node
		 create_table(nodo->left_penultimo_0_, code+'0', table);			
		 //i cannot use append because it returns a reference : that means i'm  adding at the same str the left and the right values
		 //create_table(nodo->right_ultimo_1_, code.append(1, '1'), table);
		 create_table(nodo->right_ultimo_1_, code + '1', table);
	}
	else if (code.empty()) {
		if ((!nodo->left_penultimo_0_) || (!nodo->right_ultimo_1_)) {
			table[nodo->simbol_.at(0)] = code;
			return;
		}
	}
}

std::map<uint8_t, std::string>& huffman_encoding(std::vector<std::pair<uint8_t, float>>& couples, std::map<uint8_t, std::string>& table) {

	/*[Attention] since huffman works with tree nodes, i can't move them in memory! --> push_back() could reallocate the vector memory!
	*		[moreover] even if i .reserve() the space, avoiding the reallocation problem, since i use OBJ and not OBJ* ("new" operator that allocate on heap) i will lose
	*					the OBJ when the function end "}".
	*		[SO] I have have to use pointers in the vector , and allocate the nodes with new*/

	std::vector<Nodo*> tree_nodes;			//-->the vector must contains pointers not obj.		
	//before i used --> std::vector<Nodo> tree_nodes;											[ERROR]
	//					tree_nodes.reserve(couples.size());

	for (auto& t : couples) {
		std::vector<uint8_t> v{ t.first };						//'V's born
		tree_nodes.push_back(new Nodo(v, t.second));			//Nodo tmp(v, t.second);		[ERRPR]
																//tree_nodes.push_back(tmp);	[ERROR]
	}//'v' dies BUt inside Nodo i have done a DEEP COPY so Node is still holding the v values

	//HUFFMAN coding ALGORITHM:
	//(1) i order the Nodo* vector (tree_nodes) from greater frequency to lower ones
	//(2) i take the last two nodes (removing them from the tree_nodes)
	//(3) i create the new node (father) cobining them -->new probability
	//(4) until i geta one vector element

	while (tree_nodes.size() > 1) {
		std::sort(tree_nodes.begin(), tree_nodes.end(), [](const Nodo* a, const Nodo* b) {
			return a->freq_ > b->freq_;
			});

		Nodo* left = tree_nodes.back();
		tree_nodes.pop_back();
		Nodo* right = tree_nodes.back();
		tree_nodes.pop_back();

		std::vector<uint8_t> newSimbol_(left->simbol_);
		newSimbol_.insert(newSimbol_.end(), right->simbol_.begin(), right->simbol_.end());

		//Nodo* newN =
		tree_nodes.push_back(new Nodo(newSimbol_, left->freq_ + right->freq_, left, right));

		/*wrongly before i did:
		//i extract the last two nodes
		Nodo right(tree_nodes.back());
		tree_nodes.pop_back();
		Nodo left(tree_nodes.back());
		tree_nodes.pop_back();

		//new Node creation
		std::vector<uint8_t> newV;
		std::copy(left.simbol_.begin(), left.simbol_.end(), std::back_inserter(newV)); 		// without std::back_inserter(newV) it will crash
		std::copy(right.simbol_.begin(), right.simbol_.end(), std::back_inserter(newV));
		Nodo newN(newV, left.freq_ + right.freq_, &left, &right);							//&right is the adddres of the local variable in the STACK, it will die after "}"
		//adding it to tree_nodes
		tree_nodes.push_back(newN);
		//reordering tree_nodes
		std:sort(tree_nodes.begin(), tree_nodes.end(), [](Nodo a, Nodo b) {
			return a.freq_ > b.freq_;
			});
		*/

	}
	std::string code;
	create_table(tree_nodes.at(0), code, table);

	return table;
}

std::ofstream& printHuffman(std::ofstream& os, std::map<uint8_t, std::string>& huffamnTable , std::ifstream& is , size_t totalSymbols) {
		
	BitWriter bw(os);

	//--Header: "HUFFMAN1"--
	uint8_t magicNumber[] = { 'H', 'U', 'F', 'F', 'M', 'A', 'N', '1' };
	raw_write(os, magicNumber);												// or --> for (auto& c : magicNumber)	bw(c, 8);
		
	//--ableEntries --
	// "Number of items in the following Huffman table (0 means 256 symbols)." --> having a 0 entries doesn't make sense, wile we have 256 possible ymbols (from 0 to 255) 0 is used as 256 uint8_t
	
	size_t entries = huffamnTable.size();
	if (entries == 256) {
		entries = 0;
	}
	bw(static_cast<uint32_t>(entries), 8);

	//-- HuffmanTable --
	for (const auto& y : huffamnTable) {
		bw(static_cast<uint32_t>(y.first), 8);
		bw(static_cast<uint32_t>(y.second.size() ), 5);

		//FROM std::string TO uint32-t CONVERISON:
		uint64_t number = std::stoll(y.second , nullptr , 2 );		//the second parameter is to say "where to stop the reading" and the third specifies the "base"
		bw(static_cast<uint32_t>(number), y.second.size());
	}

	//--NumSymbols --
	bw(static_cast<uint32_t>(totalSymbols) , 32);


	//now print data unfortunatly i have read the guidelines late and now i prefer to read again the input file
	is.clear();
	is.seekg(0);
	uint32_t byte = 0;
	BitReader br(is);

	while (br(byte, 8)) {
		//from-- > huffamnTable[static_cast<uint8_t>(byte)]								-- > i get a string, code related to the read byte
		//from --> std::stol(huffamnTable[static_cast<uint8_t>(byte)], nullptr, 2);		-->i get a unsigned long
		
		bw(static_cast<uint32_t>(std::stoul(huffamnTable[static_cast<uint8_t>(byte)], nullptr, 2) ) , huffamnTable[static_cast<uint8_t>(byte)].size());
	}

	return os;
}

//####################################################################### ['d' functionionality functions] ##################################################################
bool getMagicNumber( std::ifstream& is , std::ofstream& os) {
	
	char magic[8];

	raw_read(is, magic);
	if(std::string(magic , 8) != "HUFFMAN1"){								//to convert a raw char vector into a string 
		std::cerr << "file not in huffman1 format " << std::endl;
		return false ;
	}

	return true;
}

uint8_t getTableEntries(std::ifstream& is, std::ofstream& os, BitReader& br) {
	uint8_t entries = 0;
	raw_read(is, entries);
	return entries;
}

void getHuffmanTable(std::ifstream& is, std::map< std::vector<char> , uint8_t>& huffman_table , BitReader& br , const uint8_t& items) {

	std::vector<char> tmp_code;

	for (size_t count = 0; count < items; ++count) {
		uint32_t sym, len , bit_code;
		
		br(sym, 8);
		br(len, 5);

		for (size_t i = 0; i < len; ++i) {
			br(bit_code, 1);
			tmp_code.push_back(static_cast<char>(bit_code));
		}
		huffman_table[tmp_code] = static_cast<uint8_t>(sym);
		tmp_code.clear();

	}
}

//MAIN


int main(int argc, char* argv[]) {

	if (argc != 4) {
		return EXIT_FAILURE;
	}
	std::ifstream is(argv[2], std::ios::binary);
	if (!is) {
		return EXIT_FAILURE;
	}
	std::ofstream os(argv[3], std::ios::binary);
	if (!os) {
		return EXIT_FAILURE;
	}

	//start
	if (*argv[1] == 'c' || *argv[1] == 'C') {

		size_t total;										//number of elements
		std::map<uint8_t, float> table;						//table ordered by keys=Byte , freq=probability			-->map doesn't allow duplication
	    make_table(is, total, table);	

		compute_probability(table, total);
		//i have to order the map's value:					--> ORDERING A VECTOR IS COMPUTATIONALLY FASTER


		std::vector<std::pair< uint8_t, float>> couples(table.begin(), table.end());
		ordering_elements(couples );						//i get a ordered vector of pairs from lower freq

		std::map<uint8_t, std::string> huffman_table;
		huffman_encoding(couples , huffman_table );

		printHuffman(os , huffman_table , is  , total);
	}
	else if (*argv[1] == 'd' || *argv[1] == 'D') {				//decompression

		is.clear();
		is.seekg(0);
		BitReader br(is);
		BitWriter bw(os);

		if (!getMagicNumber(is, os )) {
			return EXIT_FAILURE;
		}
		uint8_t entriesRaw = getTableEntries(is , os , br );
		uint8_t entries = (entriesRaw == 0) ? 256 : entriesRaw;

		std::map<std::vector<char> , uint8_t> huffmanTable;		// symbol-code

		getHuffmanTable(is , huffmanTable , br , entries);

		uint32_t numSymbols = 0;
		br(numSymbols, 32);										//stored in Big Endian

		//DECODING + WRITING
		uint32_t bit;
		std::vector<char> code;


		while (numSymbols > 0) {

			if  (br(bit, 1)) {
				static_cast<char>(bit);
				code.push_back(bit);
				
				auto it = huffmanTable.find(code);
				if (it != huffmanTable.end()) {
					//find!
					bw(static_cast<uint32_t>(it->second) , 8);

					code.clear();								//unforgettable
					numSymbols--;
				}
					//else try again -> not found
			}

		}
		
	}




	return EXIT_SUCCESS;
}



//CONCLUSION!!
/*

This code represents my first approach to programming in C++.
The writing is quite wordy and does not follow good code reuse practices.
There are many comments and several choices are limited by my lack of familiarity with the language.

Inside the project you can find:

the implementation of a BitReader and a BitWriter

the definition of a struct called Nodo

a tree structure

a recursive function used to build the tree

sorting functions that use lambda expressions as comparison functions

the use of maps

the use of vectors of pairs

in the huffman_encoding section, I had to manage the creation of a class on the heap

raw bytes printed directly to the output stream

the use of static_cast, reinterpret_cast, and string to unsigned long conversion (stoul)
*/

//CORRECTIONS
/*
(1)
for the huffman code is better to use "uint32_t code" and "uint32_t len" instead of using "std::string "as i used. the code construction with bitwise become:
make_codes(n->left_, code << 1, len + 1);        // add '0'
make_codes(n->right_, (code << 1) | 1, len + 1); // add '1'

(2)
i allocate nodes with "new Nodo(..) but i don't "delete" it --> MEMORY LEACK
to solve it the MOVE SEMANTICS is required or using "std::unique_ptr" + a vecor that holds all nodes that when it will be destructed at the end of the function,
all the nodes will be automatically deallocated.

(3)
i get O(N^2 * logN) complexity since i put the sort function inside the while . using "std::priority_queue" is better O(N *logN)

*/