#include <print>
#include <iostream>
#include <fstream>


class String {

    size_t size;
    char* ptr;

    union {
        size_t reserved;
        char buffer[16];
    } data;

public:

    String() {
        ptr = data.buffer;
    }

    const char* c_str() {
        return ptr;
    }

};


int main() {
    
    std::ios::sync_with_stdio(false);

    std::println("{}", sizeof(std::string));

    void printOutput(std::ofstream& os);


    std::ifstream is{"abcd.txt" };
    std::ofstream os{"defg.txt" };

    printOutput(os);
}

void printOutput(std::ostream& os) {
    os << "output";
}


void grep(FILE* infile, FILE* outfile, const char* pattern) {
}


void grep(const char* infile,
          const char* outfile,
          const char* pattern) {

    FILE* is = NULL;
    if (*infile != 0) {
        is = fopen(infile, "r");
    }

    while (true) {

        if (is == NULL) {
            fgets(stdin);
        }
        else {
            fgets(is);
        }
        
    }

        
}
