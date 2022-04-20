#pragma once
#include <gum-maths.h>
#include <string>
#include <typeinfo>

template<typename T, unsigned int S, typename T2, unsigned int S2, typename T3, unsigned int S3>
void printEquation(const tvec<T, S>& leftSide, const tvec<T2, S2>& rightSide1, const std::string& operation, 
                        const tvec<T3, S3>& rightSide2, const std::string& suffix = "")
{
    unsigned int largest = Gum::Maths::max<unsigned int>({S, S2, S3});

    unsigned int Soffset  = (largest - S)  / 2;
    unsigned int S2offset = (largest - S2) / 2;
    unsigned int S3offset = (largest - S3) / 2;

    for(unsigned int i = 0; i < largest; i++)
    {
        std::string line = "";
        unsigned int index = 0;
        if(i >= Soffset && i < S + Soffset)
        {
            index = i - Soffset;
            line += std::to_string(leftSide[index]) + "\t";
        }
        else 
        {
            line += "\t\t";
        }

        if(i == (unsigned int)(largest / 2))
            line += "=\t";
        else
            line += " \t";

        if(i >= S2offset && i < S2 + S2offset)
        {
            index = i - S2offset;
            line += std::to_string(rightSide1[index]) + "\t";
        }
        else 
        {  
            line += "\t\t";
        }

        if(i == (unsigned int)(largest / 2))
            line += operation + "\t";
        else
            line += "\t";

        if(i >= S3offset && i < S3 + S3offset)
        {
            index = i - S3offset;
            line += std::to_string(rightSide2[index]) + "\t";
        }

        if(i == (unsigned int)(largest / 2))
            line += suffix;

        std::cout << line << std::endl;
    }
}