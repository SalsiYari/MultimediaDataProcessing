This project is done to:
-	take familiarity with visual sutdio code,
-	try different aspect/roule/basic know ledge of CPP


windows-keyboard shortcut:

CTRL + Z,Y					(back/forward in history)
CTRL + X,V,C
CTRL + L					(to canc a entire line)
CTRL + D					(duplicate a row )
SHIFT + ALT + freccia		(add a cursor in culomn)
				
Here some theoretical aspect important to recap :		

###############
bit		=		1/0 bit
nibble	=		4 bit			|2^4		=	16	 --> 0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F
byte	=		8 bit			|2^8		=	256	 --> different representations (continue below...)
uint8_t =		8 bit WHITHOUT sign  (0->255)
nsigned char =	8 bit WHITHOUT sign  (0->255)
uint16_t=		16bit WHITHOUT sign  (0->65.534)
char	=		8 bit whit	   Signed(-128->127)		--> two_complement!	(1) i swap bit value (2) i add +1 to it 
uint32_t=
int		=		32 bit with	   Signed(-2.147.483.647 -> +2.147.483.646 )
long	=
..........and more

size_t	=		depends, if 32 bit system -> 4 Byte / if 64 bit syestem -> 8 Byte
		=		whithout sign. it si able to contain the dimention of the biggest object in memory so
		=		is used as indices:
									--> is the type returned by sizeof() , and size() of std::vector


###############
unsigned char VS unint8_t:
Even if the second is a typedef of the first, temantically CPP sayes:
--> unsigned char = has to be "at least" 8 bit 
--> unint8_t	  = is exactly 8 bit (if the platform doesn't support it, it doesn't compile)
moreover:
--> unsigned char = you tell "here there is a char of thext (ASCII) or little string"
--> unint8_t	  = you tell "here there is a raw number"


###############
C++ sees uint8_t as char !!
uint8_t val = 65									 //as DEC is --> 0100 0001 BIN , 0x41 in hex

std::cout << "value read:"  <<val <<  std::endl;	 //it prints: Value read A
std::cout << "value read:"  <<(int)val <<  std::endl;//it prints: Value read 65


###############
different representation of BYTEs, for instance 'a' letter could be seen as :

DEC: uint8_t b = 97;		 (human)
HEX: uint8_t b = 0x61;		 (prefix 0x)
BIN: uint8_t b = 0b01100001; (prefix 0b, useful in Multimedia!)
OCT: uint8_t b = 0141;		 (prefix 0)
CHAR: uint8_t b = 'a';

std::cout << std::dec << (int)b; -> Outout: 97
std::cout << std::hex << (int)b; -> Outout: 61


###############
LITTLE ENDIAN vs BIG ENDIAN
Those "orders" are applied only when the CPU has to interpret a sequention od Byte as a Single Numerical Data (i.e. 32-bit or 64-bit ).
if the data is a single Byte(char, uint8_t)		->8 bit nothing happens
if the data is 2		Bytes(uint16_t, short)  -> the two byte are swapped
if the data is 4		Byres(uint32_t, float)	-> the four byte are inverted as "the last significattive at first".

##############
REFERENCE &
are returned by a function in these 3  cases:
(1) to CONCATENATE operations (CHAINING)
	std::cout << "ciao" << "mondo" ;
	(std::cout << "ciao").operator<<("mondo");

	std::ostream& function( std::ostream& os){		//i return a reference to the same object i gave as aroument
		...
		return os;									//so in the main i can do--> function(std::cout) << " <-- Fatto!" << std::endl;
	}

(2) when an object couldn't be copied as -> std::ostream {std::cout , std::ofstream}
(3)to admint internal modification to object
	class MyVector{
		int datas[10]
		public: 
			int& operator[](int indices){....}
	}

##############
an object doesn't have to be instantiate with new


##############
//declaration			//initialization			//assignments
type name;				tipo nome = valore;			name = value;





