#include <fstream>
#include <iostream>
#include <format>					//to print as python NOT present in Cpp 14 version 
#include <vector>			
#include <algorithm>				//to have copy
#include <string>



//----------------------------------- DON'T FOCUS ON THE FUNTIONs / CLASS BELOW UNTIL THEY DON'T APPEAR IN MAIN FUNCTION---------------------------------------------


template<typename T>	
std::ostream& stampVector(std::ostream& os_, const std::vector<T>& v) {
	os_ << "\n vector's elements: \n";
	for (auto x : v) {
		os_ <<x << std::endl;
	}
	os_ << std::endl;
	return os_;
}
//template< typename T> defines a generic structure --> is permitted as type of function parameters
//						-->allows to constraint two parameters to have the same tipe even if not known by priory (T a , T b) while (auto a, auto b) doesn't
//auto tells to compiler to understand the type from the context BUT it is NOT permitted as tipe of functio parameter




template<typename T>
std::ostream& stampMatrix(std::ostream& os_, const std::vector<std::vector<T>> v) {		// if i used only "const std::vector<T> v" , 
	for (const auto& x : v) {
		for (const auto& y : x) {											//i would have an error over here because the compiler wouldn't'know x is itself a container
			os_ << y << " ";	
		}
		os_ << std::endl;
	}
	return os_;
}





//let's define a generic class (used to show how "sort" works)
class Window {
	std::string material_ = "stone";
	bool open_ = false;

public:																		//in CPP class ar similar to struct BUT in struct everithing is "public" by default while class has "private" 
	Window(std::string material, bool open) : material_(material), open_(open) {}
	Window() {	}

	std::string getMaterial() {												//i need it sice material_ is private
		return material_;
	}

	bool getOpen() {
		return open_;
	}

	//toString doesn't exist but i can overload the Operator << to print it

	friend std::ostream& operator<<(std::ostream& os_, Window& w) {						
		
		//defining an OPERATOR inside a CLASS without "friend" the first parameter will be by default "this" followed by "os_" and "w". three param while operator<< accepts only two!
		//"friend" says "this is an external function that has the permit to stays inside the class and touch the private param 
		// (below i use material_ , to move outside the operator<<() function only getter must be used 
		os_ << "the window in " << w.material_ << " is " << std::boolalpha << w.getOpen() << std::endl;
		return os_;
	}	// thanks to it when in "stampVector" it will be called (os_ << x ...) the personalized function 
};





//-----------------------START FROM HERE ------------------------------------------


int main(int argc, char* argv[]) {
	//check over parameter
	if (argc != 3) {
		std::cout << std::format("required {}<inputFIle><outputFile> ", argv[0]);
		return EXIT_FAILURE;
	}

	//opening file
	std::ifstream is(argv[1], std::ios::binary);
	if (is.fail()) {															//i could write (!is) as same
		std::cout << "error while opening " << argv[1] << std::endl;
		return EXIT_FAILURE;
	}
	std::ofstream os(argv[2],  std::ios::binary | std::ios_base::app);			//opening a file in append mode IT MUST EXIST
	if ( !os) {
		std::cout << "error while opening " << argv[2] << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "VECTOR INITIALIZATION" << std::endl;
	//use a vector 
	std::vector<int> v;								//initialization empty vector (size=0)
	std::vector<int> v1= { 1,2,3,4,5,6,7,8,9 };		//initialization with list
	std::vector<char> v2{ 'a','b','c','d' };		//initialization with list
	std::vector<int> v3(10);						//initialization with dimentions and all values at 0 by default
	std::vector<char> v4(10, 'e');					//initialization with dimentiona and value specified for all elements
	std::vector<int> v5(v3);						//initialization with copy


	stampVector(std::cout , v1) << std::endl;
	stampVector(std::cout , v2) << std::endl;
	stampVector(std::cout , v3) << std::endl;
	stampVector(std::cout , v4) << std::endl;


	std::vector<int> v6(v1.begin(), v1.end());		//initialization with iterator
	stampVector(std::cout, v6) << std::endl;


	v5 = v1;										//copy (overwrite)
	stampVector(std::cout , v5) << std::endl;
	v5.insert(v5.end() , v1.begin() ,v1.end() );	//append v5+v1
	stampVector(std::cout , v5) << std::endl;

	//other methods

	v5 = v1;										//copy (overwrite)
	stampVector(std::cout , v5) << std::endl;

	std::vector<std::vector<int>> matrix;			//vector of vectors
	std::vector<std::vector<int>> matrix1 = {
		{1,2,3},
		{4,5,6},
		{7,8,9}
	};
	stampMatrix(std::cout, matrix1) << std::endl;

	//---ITERATORs
	std::cout << "ITERATORS" << std::endl;

	/* every iteratos support:
		initialization: it = v.begin()		to point at first element
		access to data: *it					dereferencing
		progress:		it++				to next element
		comparison:		it != v.end()		have i finished?
	
		attention: .end() points at the first "valid "NULL" element AFTER the container 
	*/

	/*iterator(standard) */
	std::vector<int>::iterator it = v1.begin();
	*it = 10;											//modify the value
	/*const_iterator (only read) */
	std::vector<int>::const_iterator ct = v1.cbegin();	//*ct=10 not allowed while std::cout<<*it is
	/*reverse iterator */
	for (auto rt = v1.rbegin(); rt != v1.rend(); ++rt) {
		std::cout << *rt << " ";						//even if seems forward, it print backward
	}

	//ITERATORS MUST USED when I HAVE TO REMOVE ELEMNTS WHILE ITERATING OVER ELEMENTs (instead auto range-for)
	for (; it != v1.end(); /* norhing here or i will jump twice*/) {
		if (*it % 2 != 0) {
			it = v1.erase(it);
		}
		else {
			++it;
		}
	}
	//PROBLEM: a vector is a CONTIGUOUS memory set of eleemnts, if you increase it it may be moved somewhere in memory to find
	/*			enought space for it(for all its elements), that means->Dangling Pointers problem
	 if you modify the vector's structure with (push_back , insert, erase..) all old iterators over it are invalid. */

	auto it2 = v.begin(); // example 
	v.push_back(100);    // here v could be moved somewhere in memory
	/* std::cout << *it2;    Dangling pointers  */

	//iterator ADAPTORS are fake iterator of standard library to perform specific powerful tasks as:
	/* iostream_iterator*/
	std::vector<int> numbers{ std::istream_iterator<int>(is), std::istream_iterator<int>() };		//to put file value into vector (input) {} are allowed to say "we are not making a function
	/* ostream_iterator */
	std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, ", "));			//to print 
	
			//i.e.: copying empty vector ERROR
	std::vector<int> sorgente = { 1, 2, 3 };
	std::vector<int> destinazione; // empty!
	// std::copy(sorgente.begin(), sorgente.end(), destinazione.begin());		--> error

	/* std::back_inserter */
	std::copy(sorgente.begin(), sorgente.end(), std::back_inserter(destinazione));		//to copy where destination doesn't exist, it perform container.push_back(val)
	stampVector(std::cout, destinazione);

	v = { 1,2,3,4,5,6,12 };

	//std::vector methods
	std::cout << "VECTOR METHODS" << std::endl;
	v.pop_back();									//pop_back()
	stampVector(std::cout, v);
	v.push_back(120);								//push_back()
	stampVector(std::cout, v);
	v.clear();										//.clear()
	stampVector(std::cout, v);
	v.push_back(v.size());							//v.size
	stampVector(std::cout, v);
													//.front() , .back(), .empty() , .at(i)
													//.begin() .end()		-->return iterator
	v = { 312 , 5 , 54 , 2 , 211 , 9 };

	/* sorting a vector and LAMBDA FUNCTION */
	std::sort(v.begin(), v.end() );					// | 2 | 5 | 9 | 54 | 211 | 312 |
	stampVector(std::cout, v);			

	
	std::vector<Window> minecraft_house = { Window ("wool", false) ,  Window() , Window("paper", true) , Window ("diamond", false)	};

	std::sort(minecraft_house.begin(), minecraft_house.end(), []( Window& a, Window& b) {				//here widget is a generic object without a predefined predicate
		return (a.getMaterial() < b.getMaterial());
		
	});
	stampVector(std::cout, minecraft_house);

	//the lambda dunction admit some paramenters inside [] to catch the variable defined outside the lambda 
	//[=] = every variable by value
	//[&] = every variable by ref
	//[=,&a,&b] = every variable by value except a and b by ref
	//...
	//by value means-> it is NOT editable inside the lambda function

	//continue in next project with these concepts:

		//cast
		//.get
		//raw read/rawwrite
		//bitreader
		//containers


	return EXIT_SUCCESS;
}

