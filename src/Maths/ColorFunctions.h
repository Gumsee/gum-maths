#pragma once
#include "vec.h"

namespace Gum {
namespace Maths
{
    extern vec3 HSVToRGB(vec3 hsv);
    extern vec3 RGBToHSV(vec3 rgb);
    extern vec3 RGBToHSB(vec3 rgb);
    extern vec3 HEXToRGB(std::string hex);
    extern vec3 HEXToRGB(int hex);
    extern std::string RGBToHEX(vec3 rgb);
}}