#include <Maths/ColorFunctions.h>
#include <Maths/color.h>
#include <gum-maths.h>

int main(int argc, char** argv)
{
	std::cout << "#" << std::endl;
	std::cout << "# Color Conversion Testing:" << std::endl;
	std::cout << "#" << std::endl;

	color cColor(255, 45, 77, 255);
	std::cout << "rgba(255, 45, 77, 255):" << std::endl;
	std::cout << "  RGB: " << cColor.getRGBA().toString() << std::endl;
	std::cout << "  HSV: " << cColor.getHSVA().toString() << std::endl;
	std::cout << "  HEX: " << cColor.getHEX() << std::endl;
	std::cout << "  OGL: " << cColor.getGLColor().toString() << std::endl;
	std::cout << std::endl;

	color cColor2;
    cColor2.setHSVA(hsva(360, 100, 50, 255));
	std::cout << "hsva(360, 100, 50, 255):" << std::endl;
	std::cout << "  RGB: " << cColor2.getRGBA().toString() << std::endl;
	std::cout << "  HSV: " << cColor2.getHSVA().toString() << std::endl;
	std::cout << "  HEX: " << cColor2.getHEX() << std::endl;
	std::cout << "  OGL: " << cColor2.getGLColor().toString() << std::endl;
	std::cout << std::endl;

	color cColor3;
    cColor3.setHEX("#FF0010FF");
	std::cout << "#FF0010FF:" << std::endl;
	std::cout << "  RGB: "    << cColor3.getRGBA().toString() << std::endl;
	std::cout << "  HSV: "    << cColor3.getHSVA().toString() << std::endl;
	std::cout << "  HEX: "    << cColor3.getHEX() << std::endl;
	std::cout << "  OGL: "    << cColor3.getGLColor().toString() << std::endl;
	std::cout << std::endl;


	color cColor4;
    cColor4.setHSVA(hsva(360, 0, 100, 255));
	std::cout << "hsva(360, 0, 100, 255):" << std::endl;
	std::cout << "  RGB: " << cColor4.getRGBA().toString() << std::endl;
	std::cout << "  HSV: " << cColor4.getHSVA().toString() << std::endl;
	std::cout << "  HEX: " << cColor4.getHEX() << std::endl;
	std::cout << "  OGL: " << cColor4.getGLColor().toString() << std::endl;
	std::cout << std::endl;


	std::cout << "Precision loss test:" << std::endl;
	rgba cRGBA(255, 45, 77, 255);
    hsva cHSVA;

	std::cout << "RGBA:        " << cRGBA.toString() << std::endl;
    for(int i = 0; i < 5; i++)
    {
        cHSVA = hsva(Gum::Maths::RGBToHSV(cRGBA), cRGBA.a);
        std::cout << "RGBA -> HSVA:" << cHSVA.toString() << std::endl;

        cRGBA = rgba(Gum::Maths::HSVToRGB(cHSVA), cHSVA.a);
        std::cout << "HSVA -> RGBA:" << cRGBA.toString() << std::endl;
    }


	return 0;
};
