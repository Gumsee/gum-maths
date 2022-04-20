#include <gum-maths.h>

int main(int argc, char** argv)
{
	std::cout << "#" << std::endl;
	std::cout << "# Matrix Testing:" << std::endl;
	std::cout << "#" << std::endl;

	mat<float, 4, 2> mat4x2_1(1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,8.0f);
	std::cout << "mat4x2_1: float" << std::endl;
	std::cout << mat4x2_1.toString(false) << std::endl << std::endl;

	mat<float, 2, 4> mat2x4_1(1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,8.0f);
	std::cout << "mat2x4_1: float" << std::endl;
	std::cout << mat2x4_1.toString(false) << std::endl << std::endl;

	std::cout << "mat2x4_1 * mat4x2_1" << std::endl;
	std::cout << (mat2x4_1 * mat4x2_1).toString(false) << std::endl << std::endl;

	std::cout << "transpose(mat2x4_1)" << std::endl;
	std::cout << mat2x4_1.transpose(mat2x4_1).toString(false) << std::endl << std::endl;

	mat<float, 4, 4> mat4x4_1(1.0f,0.0f,0.0f,0.0f,0.0f,2.0f,0.0f,0.0f,0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,4.0f);
	std::cout << "mat4x4_1: fl0x4_1) * mat4x4_1" << std::endl;
	std::cout << (mat4x4_1.inverse(mat4x4_1) * mat4x4_1).toString(false) << std::endl << std::endl;

	mat<float, 3, 3> mat3x3_1(1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,8.0f,9.0f);
	std::cout << "mat3x3_1: float" << std::endl;
	std::cout << mat3x3_1.toString(false) << std::endl << std::endl;

	std::cout << "determinate(mat3x3_1)" << std::endl;
	std::cout << mat3x3_1.determinant() << std::endl << std::endl;

	mat<float, 3, 5> mat3x5_1;
	mat3x5_1 *= 5.0f;
	std::cout << "mat3x5_1: float" << std::endl;
	std::cout << mat3x5_1.toString(false) << std::endl << std::endl;

	//std::cout << "mat3x5_1 * vec5_1" << std::endl;
	//(mat3x5_1 * vec5_1).toString(false);
	//std::cout << "" << std::endl;

	ivec2 ttestvec;
	ttestvec.x = 10;

	std::cout << sizeof(tvec<int, 2>) << " " << sizeof(int) * 2 << std::endl;
	std::cout << sizeof(tvec<int, 3>) << " " << sizeof(int) * 3 << std::endl;
	std::cout << sizeof(tvec<int, 4>) << " " << sizeof(int) * 4 << std::endl;

	vec3 xvec(1, 0, 0);
	mat3 Amat(2,0,1,0,1,0,1,0,2);

	for(int i = 1; i <= 50; i++)
	{
		std::cout << "norm(x" << i-1 << ") =\t" << vec3::normalize(xvec).toString() << std::endl;
		xvec = Amat * vec3::normalize(xvec);
		std::cout << "x" << i << " =\t" << xvec.toString() << std::endl;
	}

	return 0;
};
