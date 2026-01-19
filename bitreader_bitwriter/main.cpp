#include <fstream>
#include <iostream>
#include <vector>
#include <format>
#include <string>

//  #================# BitReader #================#
template <typename T>
std::istream& raw_read( std::istream& is, T& val, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<char*>(&val), size);
}


class BitReader {
	std::istream& is_;
	uint8_t buffer_ = 0 ;
	size_t n_ = 0;

	uint32_t readBit() {
		if (n_ == 0) {						//if i have not already read a byte from the stream (8 bit)
			raw_read(is_ , buffer_);		//i take a bytefrom the stream
			n_ = 8;							//the buffer has 8 bit now
		}
		--n_;
		return (buffer_ >> n_ ) & 1;

	}

public:
	BitReader(std::istream& is) : is_(is) {};

	//reads n bits and put them INTO u, from MSB to LSB	
	std::istream& operator()(uint32_t& u, size_t n) {
		u = 0;
		while ( n --> 0) {
			u = ( u << 1 ) | readBit();
		}
		return is_;
	}
	//-- > i know how many bite i want to read so i don't have to check if buffer still keep some information
};




//	#================# BitWriter #================#

template< typename T>
std::ostream& raw_write(std::ostream& os,const T& buffer, size_t dim = sizeof(T)) {
	return os.write(reinterpret_cast<const char*>(&buffer) , dim);
}

class BitWriter {
	std::ostream& os_;
	uint8_t buffer_ = 0;
	size_t n_ = 0;

	void writeBit(uint32_t bit) {
		buffer_ = (buffer_ << 1) | (bit & 1);
		++n_;
		if (n_ == 8) {
			raw_write( os_, buffer_);
			n_ = 0;
		}
	}

public:

	BitWriter(std::ostream& os) : os_(os) {};
	~BitWriter() {
		flush();
	}

	//writes the n LSBs of u from the MSB ones to LSB ones
	std::ostream& operator()(uint32_t u, size_t n) {
		while (n-- > 0) {
			writeBit(u >> n);
		}
		return os_;
	}

	std::ostream& flush(uint32_t bit = 0) {
		while (n_ > 0) {
			writeBit(bit);

		}
		return os_;
	}

};
	





int main(int argc, char* argv[]) {
	
	//check over number param
	if (argc != 4) {
		std::cout << " you have to pass as parameters <file as input> <0=reader , 1=writer> <binary_filename.bin>";
		return EXIT_FAILURE;
	}

	std::cout << argv[1];
	std::cout << std::format("\n\nparameter passed are:\n- program name: \n	{}\n- mode I/O: \n	{}\n- binary filename: \n	{}\n -textfilename: \n	{}\n\n", argv[0], argv[1], argv[2] , argv[3]	);

	
	if (argv[1][0] == '1') {

		//open binary file as input and check if ok			-->to perform BitReader
		std::ifstream is(argv[2], std::ios::binary);
		if (is.fail()) {
			std::cout << std::format("\n [alert] fail to open {} as input file" , argv[2]);
			return EXIT_FAILURE;
		}

		BitReader br(is);
		std::vector<uint32_t> numbers;
		std::vector<uint8_t> bits;
		std::vector<std::string> str;

		uint32_t read = 0;
		for (size_t i = 0; i < 39; ++i) {				//i know how many byet i have 
			br(read , 8);								//i can read up to 32 bit without losing nothing (4 byte)
			numbers.push_back(read);
		}

		for (auto x : numbers) {
			str.push_back(std::string(1, static_cast<char>(x)));
		}

		is.clear();
		is.seekg(0);

		while (br(read, 8)) {
			bits.push_back(read);
		}
		int o=1;
		
	}
	else if (argv[1][1] == '0') {						
		//open binary file as output						-->to perform BitWriter
		std::ofstream os(argv[2], std::ios::binary);
		if (os.fail()) {
			std::cout << std::format("\n [alert] error while opening {} as binary output file" , argv[3]);
			return EXIT_FAILURE;
		}
	}


	return EXIT_SUCCESS;

}