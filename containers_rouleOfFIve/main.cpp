#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm> // per std::swap

//ROULE OF FIVE:
/*if you define one of these 5 methods, you probably should implement all of these*/
/*
* distructor		-->		~Class()
* copy constructor	-->		Class(const Class&)
* copy assignment operator	-->		operator=(const CLass&)
* move constructor	-->		Class(class&&)
* move assignment operator	-->		operator(Class&&)
* 
* ->if the distructor HAS TO PERFORM a "delete" , "free" or "close" you need the roule of five
* 
* !is the compiler that decide if to use copy or move constructo:
* lvalue -> copy constructor
* rvalue -> move constructor
* 
* lvalue :
*   - has a name
*   - has a stable memory location
*   - could stay at the left og "="
* rvalue :
*  - doesn't has a name
*  - is temporary
*  - usually stayes at right of "="
* 
* std::move(a);             //"see a as a rvalue"
* MyClass b(std::move(a)); // usa il move constructor   ->!!! a will be empty

*/

#include <iostream>
#include <vector>
#include <algorithm> // for std::copy
#include <utility>   // for std::move

class ImageBuffer {
    int* pixels;
    size_t size;

public:
    // Standard Constructor
    ImageBuffer(size_t s) : size(s), pixels(new int[s]) {
        std::cout << "  [CTOR] Created buffer of " << size << " pixels (Allocation)\n";
    }

    // 1. DESTRUCTOR
    ~ImageBuffer() {
        if (pixels != nullptr) {
            std::cout << "  [DTOR] Freed memory of " << size << " pixels\n";
            delete[] pixels;
        }
        else {
            std::cout << "  [DTOR] Empty object destruction (already moved)\n";
        }
    }

    // 2. COPY CONSTRUCTOR (Deep Copy) - SLOW
    ImageBuffer(const ImageBuffer& other) : size(other.size), pixels(new int[other.size]) {      // allocating a new dynamic array (new int pointer)
        std::copy(other.pixels, other.pixels + other.size, pixels);
        std::cout << "  [COPY CTOR] DEEP COPY performed (New Allocation)\n";
    }

    // 3. COPY ASSIGNMENT (Copy Assignment) - SLOW
    ImageBuffer& operator=(const ImageBuffer& other) {
        if (this == &other) return *this;

        delete[] pixels; // Clean up the old buffer

        size = other.size;
        pixels = new int[other.size]; // Allocate the new one
        std::copy(other.pixels, other.pixels + other.size, pixels);

        std::cout << "  [COPY ASSIGN] Copy Assignment (New Allocation)\n";
        return *this;
    }

    // 4. MOVE CONSTRUCTOR (Move Constructor) - FAST                                            // noexcept means "this function will not throw exceptions" (can't fail)
    ImageBuffer(ImageBuffer&& other) noexcept : pixels(other.pixels), size(other.size) {        // size(other.size) copies the size value, pixels(other.pixels) copies the pointer address
        other.pixels = nullptr; // Make the other one "empty"
        other.size = 0;
        std::cout << "  [MOVE CTOR] Resources STOLEN (Zero Allocations!)\n";
    }

    // 5. MOVE ASSIGNMENT (Move Assignment) - FAST
    ImageBuffer& operator=(ImageBuffer&& other) noexcept {
        if (this == &other) return *this;

        delete[] pixels; // Clean up my old buffer

        pixels = other.pixels; // Steal the pointer
        size = other.size;

        other.pixels = nullptr; // Reset the other one
        other.size = 0;

        std::cout << "  [MOVE ASSIGN] Move Assignment (Zero Allocations!)\n";
        return *this;
    }
};


// (qui va la tua classe ImageBuffer)

ImageBuffer createTemporaryBuffer() {
    ImageBuffer temp(5);
    std::cout << "  [FUNC] Returning temporary ImageBuffer\n";
    return temp; // RVO o MOVE
}

int main() {

    std::cout << "\n=== 1. Standard construction ===\n";
    ImageBuffer img1(10);
    // [CTOR] Allocazione

    std::cout << "\n=== 2. Copy constructor (lvalue) ===\n";
    ImageBuffer img2(img1);
    // img1 è un lvalue -> COPY CTOR

    std::cout << "\n=== 3. Move constructor (rvalue temporary) ===\n";
    ImageBuffer img3(ImageBuffer(20));
    // oggetto temporaneo -> MOVE CTOR

    std::cout << "\n=== 4. Copy assignment (lvalue) ===\n";
    ImageBuffer img4(3);
    img4 = img1;
    // img1 è lvalue -> COPY ASSIGN

    std::cout << "\n=== 5. Move assignment (std::move) ===\n";
    ImageBuffer img5(4);
    img5 = std::move(img1);
    // std::move forza img1 a rvalue -> MOVE ASSIGN

    std::cout << "\n=== 6. Move constructor via std::move ===\n";
    ImageBuffer img6(std::move(img2));
    // img2 era lvalue -> forzato a rvalue -> MOVE CTOR

    std::cout << "\n=== 7. Return by value (RVO or MOVE) ===\n";
    ImageBuffer img7 = createTemporaryBuffer();
    // Tipicamente: NO COPY
    // - RVO (Return Value Optimization)
    // - oppure MOVE CTOR

    std::cout << "\n=== 8. Self assignment safety ===\n";
    img7 = img7;
    // Non succede nulla (protetto da if(this == &other))

    std::cout << "\n=== END OF MAIN ===\n";
    return 0;
}


//ROULE OF ZERO
/*is the opposit of the roule of five, if my class doesn't manage "raw resources manually" and uses built-in object instead 
 (such as std::vector, std::string, std::unique_pointer) , you don't have to write the 5 methods
  
 */

