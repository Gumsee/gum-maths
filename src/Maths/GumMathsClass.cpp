#include "GumMathsClass.h"
#include <iostream>

std::string GumMathsClass::toString([[maybe_unused]] bool oneline, std::string prefix, std::string suffix, std::string delimiter)
{
    return prefix + "mathobj()" + suffix;
}