//==========================================================================================================
/*
THIS FILE IS NOT EXECUTABLE, IT ONLY CONTAINS A LIST OF CONCEPTS IN ITALIAN ADDRESSED DURING THE COURSE.
THE EXERCISES CAN BE FOUND IN THE OTHER FOLDERS
*/
//==========================================================================================================


//questo progetto fa riferimento alla cartella esperimenti nel desktop

#include <algorithm>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>



//funzione generica per leggere qualsiasi stream
std::vector<int> read(std::istream& is) {

	std::vector<int> v;
	int num;
	while (is >> num) {									//legge finchè incontra un int
		v.push_back(num);
	}
	return v;
}
//funzione generica per scrivere qualsiasi stream
void print(std::vector<int>& v, std::ostream& os) {
	for (int n : v) {									//scorre una struttura ma per copia: l'elemento non modificabile, non cancellabile
		os << n << " ";
	}
	os << std::endl;
}




int main(int argc, char* argv[]) {

	if (argc != 3) {
		return 0;
	}
	//	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	tipi di stream:#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	
	// 
	// //---------------------------------------------------------------------std::ostream----------------------------------------------------------------------------(scrittura generica)

	std::cerr << "errore";									//stampo su standard error					//#include <iostream>
	std::cout << "\n inserisci un numero \n >";				//stampo su standard input					//#include <iostream>

	std::ofstream outputf("pippo.txt");						//crea file , *SOVRASCRIVE					//include <fstream>
	outputf << "linea 1 su file";							//stampa su file la stringa 

	std::ofstream o1;																					//include <fstream>
	o1.open("pippo.txt", std::ios_base::app);				//crea il file, *APPEND
	o1 << "append1\n append123 \append4 linea 3";

	std::stringstream ss;									//memorizza dati come stringa; utilizza come file					//#include <iostream>	#include <sstream>
	ss << "123 456 789";

	//---------------------------------------------------------------------std::istream----------------------------------------------------------------------------(lettura generica)
	int x;
	std::cin >> x;											//lettura da tastiera, memorizza in x

	std::ifstream inputf(argv[1], std::ios::binary);		//apro fileInput, modalità binary ->inputf	//#include <fstream>	//il file viene chiuso in automatico
	if (!inputf) {
		return 0;
	}

	int a, b, c;
	ss >> a >> b >> c;										//123,456,789														//#include <iostream>	#include <sstream>

	//------------------------------------------------------------------------------------------------------------------------------------------
	std::vector<int> v_input = read(std::cin);				//funzione generica per leggere da qualsiasi stream(da input)
	std::vector<int> v_file = read(inputf);					//funzione generica per leggere da qualsiasi stream(da file)

	print(v_input, std::cout);								//funzione generica per scrivere su qualsiasi stream(su terminale)
	print(v_file, outputf);									//funzione generica per scrivere su qualsiasi stream(su outputfile)

	//	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	vector: #	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	
	std::vector<int> numeri;								//struttura di dati dinamica ridimensionabile						//#include <vector> 
	numeri = { 1,2,3,4,5,6,7,8 };
	print(numeri, std::cout);

	numeri.push_back(100);									//inserisce un valore
	numeri.pop_back();										//rimuove l'ultimo elemento
	std::cout << numeri.size() << "\n";						//ritorna la dimensione del vettore ("8")
	std::cout << numeri.at(4) << "\n";						//ritorna l'elemento in posizione 4 ("5")
	std::cout << numeri.front() << "\n";					//ritorna e non rimuove il primo elemento 
	std::cout << numeri.back() << "\n";						//ritorna e non rimuove l'ultimo elemento
	numeri.clear();											//svuota il vettore
	if (numeri.empty())										//controlla se vettore vuoto
		std::cout << "vuoto";

	numeri.begin();											//restittuisce un iteratore al primo elemento
	numeri.end();											//restittuisce un itertore all'elelemento dopo l'ultimo
	numeri.rbegin();										//restituisce un iteratore inverso all'ultimo elemento
	numeri.rend();											//restituisce un iteratore inverso prima del primo elemento

	numeri = { 8,7,6,5,4,3,2,1,0 };

	//	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	ITERATOR: #	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	
	std::vector<int>::iterator it;							//dichiarazione iterator

	for (const auto& x : numeri){							//ciclo const auto --> permette di leggere e modificare i valori
		std::cout << x << " -\n ";
	}

	for (it = numeri.begin(); it != numeri.end(); ++it) {	//ciclo iterator ---> permette di leggere, modificare e CANCELLARE CON ERASE()
		std::cout << *it << "  ";							//it è un iteratore, ovvero un "puntatore" al contenuto del vettore. Usando *it accedi al valore puntato
	}

	int i;
	for (it = numeri.begin(), i = 0; it != numeri.end(); ++i) {
		if (*it % 2 == 0) {
			it = numeri.erase(it);
		}
		else {
			++it;
		}
	}
	print(numeri, std::cout);
	//	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	algorithm: #	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	
	std::sort(begin(numeri), end(numeri));					//ordine crescente default											//#include <algorithm>
	print(numeri, std::cout);

	//	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	lambda function: #	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	

	std::sort(begin(numeri), end(numeri), [](const int& a, const int& b) {
		return a > b;															//decrescente (6 4 2 1)
		});
	print(numeri, std::cout);



	
	
	
	
	
	
	//
	//continua con i vettori (da riprendere pagine prima), algorithm, lambda function(da riprendere pagine prima), template generico(da riprendere pagine prima)+ classi (dopo)
 

	return 1;
}

##################################################################################################################################################################################
##################################################################################################################################################################################
##################################################################################################################################################################################

#include <cstdint>		//-->per usare int32_t
#include <fstream>		//-->per leggere /scrivere file
#include <vector>		//--> vector
#include <ostream>		//--> per output generico 
#include <iostream>		//--> per std::cin , cout, cerr ,clog
#include <format>		//--> println
#include <iomanip>		//-->setfill , setw ...
#include <set>
#include <iterator>



//struttura per modificare ordinamento SET in decrescente -->funge da comparatore personalizzato --> functor
struct Descending {
	bool operator()(float a , float b) const{
		return a > b;			
	}
};
// Standard Template Library (STL) support custom operators:
//															std::map , std::multimap , std::set , std::multiset , std::priority_queue , 
// types of comparators:
//						function pointers, functors, lambda expression



int main(int argc, char* argv[]) {

	std::vector<float> vettoreFloat = { 21.2f , 122.1f , 2.121222115f , 43.34f , 43.34f ,34 , 654.4f };
	int v[12] = { 1,2,3,4,5,6,7,8,9,10,11,12 };
	
	//	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	formattazione: #	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	

	int numero  =  255;
	std::cerr << "Numero in hex: 0x" << std::hex << numero << ", in decimale: " << std::dec << numero << std::endl;				// formattazione con << 
			//manipolatore di stream	(1)											(2)						(3 aggiunge \n e flush del buffer "forza il salvataggio soprattutto su file" )
	std::cerr << std::setw(3) << std::setfill('0') << v[0] << std::endl;														//stampa 001
	std::cerr << std::setw(3) << std::setfill('0') << v[11] << std::endl;														//stampa 012

	
	std::println(std::cerr, "numero in hex minuscolo: 0x{:x} , in decimale: {} ", numero, numero);
	std::println(std::cerr, "numero in hex Maiuscolo : 0x{:x} ,in ottale: {},  da c++23 binario:{:b}", numero, numero, numero); //formattazione con #include <format> da c20++


	//	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	SET e MULTISET :#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	
	std::set<float> setFloat(vettoreFloat.begin(), vettoreFloat.end());															//NO elementi duplicati, ORDINE CRESCENTE 
	setFloat.insert(34);																										//			(usano operator < di default)
	setFloat.insert(34);
	std::cout <<"di 34 ce ne   sempre solo (1) ->" << setFloat.count(34) << std::endl;
	
	//scorrere
	for (std::set<float>::iterator start = setFloat.begin(); start != setFloat.end(); ++start) {
		std::cout << "--originalmente -->" << *start <<std::endl; 
	}
	
	std::vector<float>::iterator start = vettoreFloat.begin();

	std::set<float, Descending> setFloatDec(start, vettoreFloat.end());															//Durante l'inserimento degli elementi, il set chiama internamente:
	//scorrere classico 
	for (const auto& n : setFloatDec) {
		std::cout << "\n->" << n << std::endl;
	}

	//	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	apertura file binario input :#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	

	std::ifstream is("file_binario", std::ios::binary);

	if (!is) {
		return EXIT_FAILURE;
	}
	//	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	 LETTTURA FILE:#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	   da finire + bitreader bitwriter
	//contenuto file : 48 55 46 46 4D 41 4E 31 05 61 26 B1 0A 63 1F 64 26 32 84 00 00 00 1A 05 5F E6 80
	std::vector<char> contenitore = {};
	char c;																
	while (is.get(c)) {										//-->'4'+'8'+' '+'5'+'5'+' '+...
		contenitore.push_back(c);							// contenitore =48 55 46 46 4D 41 4E 31 05 61 26... 
	}
	is.clear();					  //  Ripristina lo stato valido (is.get(c) pu  impostare eofbit o failbit quando non ci sono pi  dati da leggere, seekg non lo fa )
	is.seekg(0, std::ios::beg);   //  Riporta il puntatore all'inizio

	while (is >> c) {
		contenitore.push_back(c);
	}
	
	vector<int> v{
	istream_iterator<int>(is),
	istream_iterator<int>()
	};

	//std::map<std::set<char>, int> mappa(set_no_duplicati.begin(), set_no_duplicati.end());		-->   da capire


	//	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	 CAST:#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#					da finire

	double d = 3.14;
	double e = static_cast<int>(d);  // e = 3								//3.14 -> int 3 -> double 3.0

	const unsigned char arr[4] = { 0xaa, 0xbb, 0xcc, 0xdd };
	const int* p = reinterpret_cast<const int*>(arr);						//forza il puntatore ad array ad essere interpretatoc ome puntatore ad int
	int* m = const_cast<int*>(p);											//rimuove il const permettendo di modificare il contenuto
	*m = 0;																	//scrive 4 byte sull'array
	
	
	//STATIC CAST
	//			~conversioni tra tipi primitivi 
		int i = 42;
		double d = static_cast<double>(i);  // int → double

	//DYNAMIC CAST
	//			~utilizzato solo con tipi polimorfi (cioè almeno con una funzione virtuale)
		Base* b = new Derived();
		Derived* d = dynamic_cast<Derived*>(b);  // Sicuro: restituisce nullptr se fallisce

	//CONST CAST
	//			~usato per rimuovere o aggiungere qualificatori const o volatile
		const int* x = new int(5);
		int* y = const_cast<int*>(x);  // Rimuove il const

		*y = 10;  // !! Comportamento indefinito se x puntava a oggetto const originale

	//REINTERPRET CAST
	//			~cambia come vengono reinterpretati i bit
		int i = 65;
		char* c = reinterpret_cast<char*>(&i);  // Interpreta i bit dell'int come char*
		//-
		int a = 65;
		char* p = reinterpret_cast<char*>(&a);
		std::cout << *p << std::endl;  			// Stampa 'A' su architetture little-endian


	//C STYLE --> static_cast o reinterpret_cast
		int x = (int)3.14;  // sconsigliato qui


	return EXIT_SUCCESS;
}
##################################################################################################################################################################################
##################################################################################################################################################################################
##################################################################################################################################################################################

//counting the occurrences of each byte
	std::array<size_t, 256> count{};							//	#import <array>
	
	char c;													//la variabile c è di tipo char (può essere signed o unsigned a seconda del compilatore) -->unsigned=[-128, +127] --> no indice array
	while (is.get(c)) {										//legge un byte e ritorna il byte o EOF (-1)	//legge anche \0 ,\t , \n mentre >> li salta
		++count[(uint8_t)c];								//nella posizione "byte" incremento il contatore
	}
	//un byte può rappresentare 2^8=256 valori distinti  (da 0 a 255)
	//convertiamo c in uint8_t -->sempre 0-255
	
//bitwise operation
	int a = 0b10011001;										//	   1001 1001			a=153
	a = a << 1;												//0001 0011 0010			->su cout è convertito in ascii=306
	a = a << 2;												//0100 1100 1000			=1224
	a = a >> 5;												//	   0010 0110			=38
	a = a <<= 1;											//0100 1100					=76		-->ok, ma non necessario il a = ...
	a <<= 2;												//0001 0011 0000			=304

	
/* Manual version (machine independent) */
	for (const auto& x : v) {
		os.put((x >> 0) & 0xff);							//.put --> Scrive un singolo carattere (1 byte) nello stream.
		os.put((x >> 8) & 0xff);									//Il parametro è un singolo char (1 byte).
		os.put((x >> 16) & 0xff);									//
		os.put((x >> 24) & 0xff);
	}
	
//os.write //Scrive un blocco di count byte a partire dall’indirizzo ptr. Scrive dati binari grezzi, senza interpretazione. Nessuna codifica
/*RAW_WRITE*/
/*Funzione generica (template): (riferimento stream di output os ,un valore qualsiasi val una dimensione da scrivere (opzionale, di default sizeof(val))*/
	template<typename T>
	std::ostream& raw_write(std::ostream& os,const T& val , size_t size = sizeof(val)) {
		return os.write(reinterpret_cast<const char*>(&val), size);									//Converte il valore in sequenza di byte (cast a const char*)->Scrive byte grezzi(raw) nello stream binario
	}
	raw_write(os, v[0], v.size() * sizeof(int));


/*RAW_READ*/
/*Funzione generica (template): (stream di input os ,un valore qualsiasi val una dimensione da scrivere (opzionale, di default sizeof(val))*/
	template<typename T>
	std::istream& raw_read(std::istream& is, T& val, size_t size = sizeof(T))
	{
		return is.read(reinterpret_cast<char*>(&val), size);						//&val -> indirizzo di memoria, converte quel int32_t* in un char* (ovvero array di byte)
	}																				//is.read legge size byte e li copia direttamente dentro val

		vector<int> v;
		int32_t val;
		while (raw_read(is, val)) {
			v.push_back(val);
		}
	//[ D2 02 00 00  → little endian]		-> is.read(4Byte)		-> val = [ 0x000002D2 = 722 in decimale]
	// sta leggendo questi 4 byte e convertendoli direttamente in int32_t
	// vettore v contiene interi veri, non caratteri: ogni val letto è un numero a 32 bit		-->sto interpretando gruppi di bytr come interi 
		std::cout << //stampa int in base decimale di default
	//	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	 RANGES:(cpp20)#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#	#					
	namespace rng = std::ranges;
	rng::copy(v, ostream_iterator<int>(os, "\n"));									//itera sugli elementi della sorgente "src" e li inserisce nella destinazione "dst" usando l'assegnazione .
			//   ostream_iterator<int>(os, "\n")									-->iteratore "contenitore di destinazione" che scrive un int su os(output stream) seguito da \n


/*BITWRITER*/
// si chiama bitwriter(bit) ogni volta che si vuole scrivere un bit, messitemporanemamente in un buffer fino ad 8, quando pieno viene trascritto , bit residui vengono trascritti alla fine complementando con 0 

#include <fstream>
#include <vector>
#include <iomanip>
#include <cstdint>
#include <iterator>

template<typename T>
std::ostream& raw_write(std::ostream& os, const T& val,	size_t size = sizeof(T)){
	return os.write(reinterpret_cast<const char*>(&val), size);
}

class bitwriter {
	std::ostream& os_;
	uint8_t buffer_ = 0;							//8 bit = 1 BYTE
	size_t n_ = 0;									//numero di bit attualmente nel bufffer

	void writebit(uint32_t bit) {					//writebit() aggiunge un singolo bit al buffer 
		buffer_ = (buffer_ << 1) | (bit & 1);			//(buffer_ << 1) scorre a sinistra i bit per fare spazio
		++n_;											// |(bit & 1) aggiunge in coda il bit ricevuto (meno significativo)	--> bitwise end con 00001
		if (n_ == 8) {									//(n_ == 8 ) buffer pieno
			raw_write(os_, buffer_);
			n_ = 0;
		}
	}

public:
	bitwriter(std::ostream& os) : os_(os) {}		//COSTRUTTORTE
	~bitwriter() {									//DISTRUTTORE chiama flush()
		flush();
	}

													// Write the n least significant bits of u from MSB to LSB
	std::ostream& operator()(uint32_t u, size_t n) {
		while (n --> 0) {							//(n-- >0)	
			writebit(u >> n);						//(u >> n) prende il bit n-esimo da sinistra, scrive bit per bit partendo dal più significativo
		}
		return os_;
	}

	std::ostream& flush(uint32_t bit = 0) {			//flush() scrive eventuali bit rimasti nel buffer
		while (n_ > 0) {
			writebit(bit);
		}
		return os_;
	}
};

int main(int argc, char* argv[])
{
	using namespace std;

	if (argc != 3) {
		return EXIT_FAILURE;
	}
	ifstream is(argv[1]/*, std::ios::binary*/);
	if (!is) {
		return EXIT_FAILURE;
	}

	vector<int> v{
		istream_iterator<int>(is),
		istream_iterator<int>()
	};

	ofstream os(argv[2], std::ios::binary);
	if (!os) {
		return EXIT_FAILURE;
	}

	bitwriter bw(os);
	for (const auto& x : v) {
		bw(x, 11);
	}

	return EXIT_SUCCESS;

}
