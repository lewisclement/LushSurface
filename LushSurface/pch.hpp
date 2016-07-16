#pragma once

#ifndef PCH_H
#define PCH_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include <unordered_set>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <GL/glew.h>
#include <GL/glu.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#pragma clang diagnostic ignored "-Wundefined-reinterpret-cast"
#pragma clang diagnostic ignored "-Wshadow"
#pragma clang diagnostic ignored "-Wundef"
#pragma clang diagnostic ignored "-Wfloat-equal"

#define GLM_FORCE_RADIANS

#include "../glm/glm/glm.hpp"
#include "../glm/glm/gtc/type_ptr.hpp"
#include "../glm/glm/gtc/matrix_transform.hpp"
#include "../glm/glm/gtc/matrix_inverse.hpp"

#pragma clang diagnostic pop

#endif
