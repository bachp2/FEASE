#pragma once
#include <string>

#define PRINT2F(X, Y) printf(#X ": %.2f, " #Y ": %.2f\n", X, Y);
#define PRINT3F(X, Y, Z) printf(#X ": %.2f, " #Y ": %.2f, " #Z ": %.2f\n", X, Y, Z); 
#define PRINTBOOL(X) std::cout << #X << ": " << std::boolalpha << X << std::endl;
typedef std::string String;

