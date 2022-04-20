#include <gum-maths.h>
#include "equationprinter.h"

int main(int argc, char** argv)
{
	std::cout << "#" << std::endl;
	std::cout << "# Vector Testing:" << std::endl;
	std::cout << "#" << std::endl;

	tvec<double, 2> vec2_1(2.0, 3.0);
	std::cout << "vec2_1: (2, 3) double" << std::endl;
	std::cout << vec2_1.toString() << std::endl << std::endl;

	tvec<float, 5> vec5_1(vec2_1, 4.0f, 6.0f, 6.0f);
	std::cout << "vec5_1: (vec2_1, 4, 6, 6) float" << std::endl;
	std::cout << vec5_1.toString() << std::endl << std::endl;

	vec3 vec3_1(1.0f, 2.0f, 3.0f);
	std::cout << "vec3_1: (1.0f, 2.0f, 3.0f) float" << std::endl;
	std::cout << vec5_1.toString() << std::endl << std::endl;

	vec3 vec3_2(6.0f, 4.0f, 7.0f);
	std::cout << "vec3_2: (6.0f, 4.0f, 7.0f) float" << std::endl;
	std::cout << vec5_1.toString() << std::endl << std::endl;

	printEquation(vec3_1 * vec3_2, vec3_1, "*", vec3_2);

	return 0;
};
