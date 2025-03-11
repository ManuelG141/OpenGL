#include <iostream>
#include <GLFW/glfw3.h>

// To dinamic link your library "GLFW" you have to 
// 1. Change the import dependency in the linker part from "glfw3.lib" to "glfw3ddl.lib" (Static library that has the pointers to the dynamic library)
// 2. Place the dynamic library file in an accesible location (For example, next to the executable)

int main()
{
	int a = glfwInit();

	std::cout << a << std::endl;

	std::cin.get();
}