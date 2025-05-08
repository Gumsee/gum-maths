#pragma once
#include "vec.h"
#include "ColorFunctions.h"

class color : public rgba
{
public:
    using rgba::rgba;
    color(rgba col) : rgba(col) {}
    color(hsva col)             { setHSVA(col); }
    color(std::string hex)      { setHEX(hex); }

    using rgba::operator-;
    using rgba::operator-=;
    using rgba::operator+;
    using rgba::operator+=;
    using rgba::operator/;
    using rgba::operator/=;
    using rgba::operator*;
    using rgba::operator*=;

    //Setter
    void setHSVA(const hsva& val)       { rgba::operator=(rgba(Gum::Maths::HSVToRGB(val), val.a)); }
    void setRGBA(const rgba& val)       { rgba::operator=(val); }
    void setHEX(const std::string& val) { rgba::operator=(rgba(Gum::Maths::HEXToRGBA(val))); }

    //Getter
    hsva getHSVA() const         { return hsva(Gum::Maths::RGBToHSV(*this), a); }
    rgba getRGBA() const         { return *this; }
    vec4 getGLColor() const      { return vec4(r,g,b,a) / 255.0f; }
    std::string getHEX() const   { return Gum::Maths::RGBAToHEX(*this); }
};