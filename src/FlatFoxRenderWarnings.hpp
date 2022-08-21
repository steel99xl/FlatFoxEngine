//
// Created by steel99xl on 3/25/22.
//

#ifndef FLATFOXENGINE_FLATFOXRENDERWARNINGS_HPP
#define FLATFOXENGINE_FLATFOXRENDERWARNINGS_HPP
#include <GL/glew.h>
#include <iostream>
#include <functional>

#define GLCall(function); GLClearError(); function; GLLogCall(#function, __FILE__, __LINE__);
void GLClearError();

void GLLogCall(const char *function, const char *file, int line);

#endif //FLATFOXENGINE_FLATFOXRENDERWARNINGS_HPP
