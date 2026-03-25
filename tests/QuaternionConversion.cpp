#include <gum-maths.h>
#include <cassert>

template<typename T>
bool unitTest(T given, T expected)
{
  if(given != expected)
  {
    std::cerr << "Unit test failed: expected " << expected.toString("", "", ", ", 10) << ", got " << given.toString("", "", ", ", 10) << std::endl;
    //assert(false);
    return false;
  }

  return true;
}

int main(int argc, char** argv)
{
  for(float i = 0.0f; i < 180; i += 1.0f)
  for(float j = 0.0f; j < 180; j += 1.0f)
  for(float k = 0.0f; k < 180; k += 1.0f)
  {
    vec3 testVal(i,j,k);
    fquat q = fquat::toQuaternion(testVal);
    //if(!unitTest(fquat::toEuler(q), testVal))
      //return 1;
    unitTest(fquat::toEuler(q), testVal);
  }

	return 1;
};
