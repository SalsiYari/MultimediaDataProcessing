#include <vector>
#include <iostream>
#include <fstream>
#include <format>
#include <numbers>              // std::numbers::pi
#include <iterator>
#include <algorithm>
#include <istream>
#include <string>
#include <limits>               // Necessario per std::numeric_limits

//===============================================================================================================================================

std::ostream& printString(std::ostream& os_, std::string& str_) {           // defined to try const_cast
    os_ << str_ << std::endl;
    return os_;
}

struct Cow { int milk; };
class Sheep {
    int milk_;
public:
    Sheep(int milk) : milk_(milk) {};
    // Aggiunto getter per testare (opzionale)
    int getMilk() const { return milk_; }
};

void cast() {
    /* arguments:
    let's focus on CAST between types: the C style was "(int)var" but is considered dangerous, there are 4 different cast methods:
    -static_cast
    -reinterpret_cast
    -const_cast
    -dynamic_cast

    moreover a little disclaimer over the difference between CLASS instantiation (with or without "new"):
    -dynamic allocation (with new)
        object lives on heap
        it stays alive until a "delete" is called
        manually memory managing
        "new" operator returns memory ADDRESS (pointer) of the created obj.
        as a pointer you can access instance's elements using pointer->attribute

    -automatic allocation (without new)
        object lives on stack (safe , faster)
        the lifetime depends on when the "} in which is defined" is closed.
        the memory is automatically deallocated going out of the scope so "delete" is not required" but the destructor is called "~class_name()"
        i can access elements with the dot "." operator as: instance_name.attribute
    */

    /* static_cast */
    // done by different types , done in compile-time, used for numeric conversion, to make it explicit (removing warning), upcast or downcast, from void* to type*
    double pi = 3.14159;
    std::cout << static_cast<int>(std::numbers::pi) << std::endl;           // static_cast<int>(variable)

    /* dynamic_cast */
    // it works run-time used exclusively with polymorphism (class with at least one "virtual" function)

    /* const_cast */
    // used only to add/remove "const"
    const std::string str = "ciao";
    // printString(std::cout, str);                               // error because function expects non-const ref
    printString(std::cout, const_cast<std::string&>(str));        // const_cast removes constness temporarily

    /* reinterpret_cast */
    // low-level cast "use this bit sequence as other type" , it doesn't change bit but the "label". !the code becomes not portable, depends on cpu's endianness
    Cow m;
    m.milk = 10;

    // [FIX] Esempio corretto di reinterpret_cast tra puntatori
    // 1. take the address (&m) -> i get Cow*
    // 2. I Cast the pointer from Cow* to Sheep*
    Sheep* a = reinterpret_cast<Sheep*>(&m);

    // now 'a' points to memory of Cow, but thinks it is a Sheep.
    // accessing a->milk_ will read the bits that in reality belong to m.milk
    // std::cout << a->getMilk() << std::endl; // Dangerous but works here
}

//===============================================================================================================================================

void readingMethods(std::istream& is_) {
    std::vector<int> numbers;
    int num;
    std::string line;

    // (1) (>>) reading somewhere
    // skip whitespaces,tab,next-line,\n until it doesn't find a "correct requested" character (here num is int)
    while (is_ >> num) {
        numbers.push_back(num);
    }

    if (is_.fail()) {       // if it fails, the stream goes into "error state"
        is_.clear();        // [FIX] First clear the error flags
        // is_.seekg(0);    // [NOTE] seekg works on file streams, might fail on std::cin depending on OS. Removed for generic istream safety.
    }

    // (2)
    // attention: if you read an "int" with >>, the "\n" remains in the buffer, and the .getline() will read an empty string
    is_.ignore(std::numeric_limits<std::streamsize>::max(), '\n');     // it clears the buffer

    // std::getline(stream, string)
    // [NOTE] This might not run if previous loop consumed everything (EOF)
    while (std::getline(is_, line)) {
        // work on the line: this read all the character (also whitespace) until "\n". useful with CSV, log, user input       
    }

    // (3)
    is_.clear();
    std::vector<int> inputVector{std::istream_iterator<int>(is_) , std::istream_iterator<int>() }; // Commented to avoid blocking input

    // (4)
    std::copy(std::istream_iterator<int>(is_), std::istream_iterator<int>(), numbers.begin()); // Requires pre-sized vector or back_inserter

    // (5) get() to read character by character
    is_.clear(); // Reset again just in case
    numbers.clear();
    char c_char;
    while (is_.get(c_char)) {
        numbers.push_back(c_char);     }

    // (6) peek() -> used to read next character without "extracting" it from the stream
        // returns EOF(-1) if stream is end
        // it reads a SINGLE character (i.e. "1112edo" -> "1") and converts it to "int"

    int c = is_.peek(); 
    if (c == EOF) {
        // end of file check
    }
    else if (isdigit(c)) {
        // ...
    }

    // (7) .read() used in "rawread" to get raw byte from the stream
    // it requires a char* (destination buffer) and the number of byte to read 
    char buffer[100];
    is_.read(buffer, 100); 

    // to read inside a non-char variable --> reinterpret_cast
    std::cout << "Bytes read: " << is_.gcount() << std::endl;     
    
}

//===============================================================================================================================================

void writingMethods(std::ostream& os_) {
    // (1) (<<) writing a variable and convert it in a text (FORMATTED WRITING)
    // by default operator(<<) prints number in decimal.

    os_ << 97 << "\n";              // print : 97
    os_ << 0x61 << "\n";            // print : 97
    os_ << 0b01100001 << "\n";      // print : 97
    os_ << 0141 << "\n";            // print : 97 (octal)

    os_ << 'a' << "\n";             // print : 'a'

    os_ << std::dec << 97 << "\n";      // print: 97 (DEC, default)
    os_ << std::hex << 97 << "\n";      // print: 61 (HEX)
    // Or                           
    os_ << std::hex << 0x61 << "\n";    // print: 61 (HEX same result)
    os_ << std::oct << 97 << "\n";      // print: 141 (In octal)

    // (2) using copy
    std::vector<int> numbers = { 3,2,21,124,7,5 };
    std::copy(numbers.begin(), numbers.end(), std::ostream_iterator<int>(os_, " "));
    os_ << "\n";

    // (3) put(char c) equivalent to .get() (UNFORMATTED)
    // it returns the stream itself so it can be chained
    os_.put('B').put('C'); 
    os_ << "\n";

    // .write(const char* s, streamsize n); equivalent to read() to write raw data
    struct Dati {
        int id;
        double valore;
    };

    Dati d = { 10, 3.14 };

    os_.write(reinterpret_cast<const char*>(&d), sizeof(d));
}

//## HOW TO READ LITTLE ENDIAN BYTE ##
template <typename T>
std::ifstream& readLittleEndian(std::ifstream& is, T& val, size_t n) {
    val = 0;
    uint8_t byte;

    // loop to count from 0 to n-1
    for (size_t i = 0; i < n; i++) {
        is.get(reinterpret_cast<char&>(byte));
        
        // 1. static_cast<T>(byte):      convert byte into final type
        // 2. << (i * 8):                move to left of 0, 8, 16, 24 bit...
        // 3. |= : Uniamo i bit al risultato
        val |= (static_cast<T>(byte) << (i * 8));
    }
    return is;
}



int main(int argc, char* argv[]) {

    // Esempio rapido per provare il codice senza file binari
    std::cout << "--- CAST TEST ---\n";
    cast();

    std::cout << "\n--- WRITE TEST ---\n";
    writingMethods(std::cout);

    std::cout << "\n--- READ TEST (Type something) ---\n";
    // readingMethods(std::cin);

    if (argc != 3) {
        // std::cout << std::format("> Usage: {} <input_binary_file> <output_binary_file>\n", argv[0]);
        // return EXIT_FAILURE;
    } else {
        std::ifstream is(argv[1] , std::ios::binary);
        if (!is) {
            std::cout << std::format("> Error opening input file {}\n", argv[1]);
        }
        std::ofstream os(argv[2] , std::ios::binary);
        if (!os) {
            std::cout << std::format("> Error opening output file {}\n", argv[2]);
        }

        readingMethods(is);
        writingMethods(os);
    }

    std::cout << " \n\n end \n\n";
    return EXIT_SUCCESS;

}



/*
NOTE:
a template class such as 
template<typename T>
class mat {
	int rows_ = 0, cols_ = 0;
	std::vector<T> data_;
public:
	mat() {}
    .....

MUST BE ISTANTIATE with TYPE AS:
mat<uint8_t> m(10, 20);            //example in which <uint8_t> is shown
                                    //mat is a template , mat<uint32_t> is a class (different from mat<uint8_t>




*/
