#pragma once
#include "Common.h"
#include <string.h>
using namespace std;
struct CustomException : public std::exception
{
   std::string s;
   CustomException(std::string ss) : s(ss) {}
   const char* what() const throw() { return s.c_str(); }
};