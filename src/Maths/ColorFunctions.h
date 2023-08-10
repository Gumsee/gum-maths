#pragma once
#include "vec.h"

namespace Gum {
namespace Maths
{
    extern rgb HSVToRGB(hsv hsv);
    extern hsv RGBToHSV(rgb rgb);
    extern hsv RGBToHSB(rgb rgb);
    extern rgb HEXToRGB(std::string hex);
    extern rgba HEXToRGBA(std::string hex);
    extern rgba HEXToRGBA(int hex);
    extern std::string RGBToHEX(rgb val);
    extern std::string RGBAToHEX(rgba val);
}}