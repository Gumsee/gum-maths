#include "ColorFunctions.h"
#include "Maths.h"
#include <iostream>

namespace Gum {
namespace Maths
{
    rgb HSVToRGB(hsv val)
    {
        float h = val.h;
        float s = val.s / 100.0f;
        float v = val.v / 100.0f;

        if(h >= 360.0f) 
            h -= 360.0f;
        
        h /= 60.0f;
        float ff = h - (int)h;
        float p = v * (1.0f - s);
        float q = v * (1.0f - (s * ff));
        float t = v * (1.0f - (s * (1.0f - ff)));

        std::cout << val.toString() << std::endl;

        switch((int)h) 
        { 
            case 0:  return rgb(ceil(v * 255.0f), ceil(t * 255.0f), ceil(p * 255.0f));
            case 1:  return rgb(ceil(q * 255.0f), ceil(v * 255.0f), ceil(p * 255.0f));
            case 2:  return rgb(ceil(p * 255.0f), ceil(v * 255.0f), ceil(t * 255.0f));
            case 3:  return rgb(ceil(p * 255.0f), ceil(q * 255.0f), ceil(v * 255.0f));
            case 4:  return rgb(ceil(t * 255.0f), ceil(p * 255.0f), ceil(v * 255.0f));
            default: return rgb(ceil(v * 255.0f), ceil(p * 255.0f), ceil(q * 255.0f));
        }
    }

    hsv RGBToHSV(rgb val)
    {
        float redc   = (float)val.r / 255.0f;
        float greenc = (float)val.g / 255.0f;
        float bluec  = (float)val.b / 255.0f;

        float cmax = std::max(std::max(redc, greenc), bluec);
        float cmin = std::min(std::min(redc, greenc), bluec);
        float delta = cmax - cmin;


        float hue = 0;
        if      (cmax == redc)   { hue = std::fmod((greenc - bluec)/delta, 6.0f); }
        else if (cmax == greenc) { hue = (bluec - redc)/delta + 2.0f; }
        else                     { hue = (redc - greenc)/delta + 4.0f; }
        hue *= 60;
        if(hue < 0)
            hue += 360;

        unsigned short saturation = (cmax == 0 ? 0 : delta / cmax) * 100;
        unsigned short brightness = (int)(cmax * 100.0f);
        return hsv((unsigned short)ceil(hue), saturation, brightness);
    }
    hsv RGBToHSB(rgb val) { return RGBToHSV(val); }

    rgb HEXToRGB(std::string hex)
    {
        rgb ret;
        sscanf(hex.c_str(), "#%2hx%2hx%2hx", &ret.r, &ret.g, &ret.b);
        return ret;
    }

    rgba HEXToRGBA(std::string hex)
    {
        rgba ret;
        sscanf(hex.c_str(), "#%2hx%2hx%2hx%2hx", &ret.r, &ret.g, &ret.b, &ret.a);
        return ret;
    }

    rgba HEXToRGBA(int hex)
    {
        rgba ret;
        ret.r = (hex >> 16) & 0xFF;  // Extract the RR byte
        ret.g = (hex >> 8)  & 0xFF;  // Extract the GG byte
        ret.b = (hex)       & 0xFF;  // Extract the BB byte
        return ret;
    }

    std::string RGBToHEX(rgb val)
    {
        char r[4], g[4], b[4];
        sprintf(r, "%.2hX", val.r);
        sprintf(g, "%.2hX", val.g);
        sprintf(b, "%.2hX", val.b);

        return std::string("#") + r + g + b + "FF";
    }

    std::string RGBAToHEX(rgba val)
    {
        char r[4], g[4], b[4], a[4];
        sprintf(r, "%.2hX", val.r);
        sprintf(g, "%.2hX", val.g);
        sprintf(b, "%.2hX", val.b);
        sprintf(a, "%.2hX", val.a);

        return std::string("#") + r + g + b + a;
    }

}}