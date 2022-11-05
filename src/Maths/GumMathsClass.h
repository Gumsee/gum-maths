#pragma once
#include <string>
#include "Constants.h"

struct GumMathsClass
{
    //GumMathsClass() {};
    virtual std::string toString(bool oneline = true, std::string prefix = "", std::string suffix = "", std::string delimiter = ", ");
};