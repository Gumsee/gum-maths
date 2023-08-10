#pragma once
#include "vec.h"
#include "ColorFunctions.h"

class color : public rgba
{
public:
    using rgba::rgba;

    //Setter
    void setHSVA(hsva val)       { rgba::operator=(rgba(Gum::Maths::HSVToRGB(val), val.a)); }
    void setRGBA(rgba val)       { rgba::operator=(val); }
    void setHEX(std::string val) { rgba::operator=(rgba(Gum::Maths::HEXToRGBA(val))); }

    //Getter
    hsva getHSVA()               { return hsva(Gum::Maths::RGBToHSV(*this), a); }
    rgba getRGBA()               { return *this; }
    vec4 getGLColor()            { return vec4(r,g,b,a) / 255.0f; }
    std::string getHEX()         { return Gum::Maths::RGBAToHEX(*this); }
};