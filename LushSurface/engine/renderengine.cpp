#include "renderengine.hpp"

static bool up = false, down = false, right = false, left = false;

bool RenderEngine::initialize(GLuint width, GLuint height) {
    windowWidth = width;
    windowHeight = height;

    view = new View();

    if(!initializeGL()) {
        std::cout << "Could not initialize OpenGL window" << std::endl;
        return false;
    }

    if(!initializeScene()) {
        std::cout << "Could not initialize OpenGL scene" << std::endl;
        return false;
    }

    return true;
}

bool RenderEngine::initializeGL() {
    if (SDL_Init (SDL_INIT_VIDEO) < 0) {
        std::cout << "Error while initializing SDL: "<< SDL_GetError() << std::endl;
        return false;
    }
    atexit(SDL_Quit);

    GLint majorVersionError = SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    GLint minorVersionError = SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 2);
    GLint profileMaskError = SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    if (majorVersionError < 0 || minorVersionError < 0 || profileMaskError < 0) {
        std::cout << "\033[1;31mCould not enable OpenGL 3.3: " << majorVersionError << ":" << minorVersionError << ":" << profileMaskError << "\033[0m" << std::endl;
        return false;
    }

    GLint bufferError = SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
    GLint bufferDepthError = SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 24);
    if (bufferError < 0 || bufferDepthError < 0) {
        std::cout << "Could not enable double buffering: " << bufferError << ":" << bufferDepthError << std::endl;
    }

    screen = SDL_CreateWindow ("LushSurface Milestone 1 dev",
                          SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED,
                          (int)windowWidth, (int)windowHeight,
                          // SDL_WINDOW_FULLSCREEN |
                          SDL_WINDOW_OPENGL);
    if (screen == NULL) {
        std::cout << "Unable to create OpenGL 3.3 window: "<< SDL_GetError () << std::endl;
        return false;
    }

    glcontext = SDL_GL_CreateContext(screen);
    if (glcontext == NULL) {
        std::cout << "Unable to create OpenGL context: " << SDL_GetError () << std::endl;
        return false;
    }

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError > 0) {
        std::cout << "Unable to initialize GLEW: " << glewError << std::endl;
        return false;
    }

    glEnable(GL_DEPTH_TEST);

    SDL_GL_SetSwapInterval(1);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    return true;
}

bool RenderEngine::initializeScene() {
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);
    glm::vec3 cameraPos = view->getCameraPos();
    world = new World(cameraPos.x - chunkSize / 2, cameraPos.z + chunkSize / 2);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    //projection = glm::perspective(45.0f, (GLfloat)windowWidth / windowHeight, 0.1f, 1000.0f);
    projection = glm::ortho(0.0f, 24.0f, 0.0f, 13.5f, 0.1f, 100.0f);


    /*textures = new GLuint[2];
    glGenTextures(2, textures);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_Surface* loadedSurface = IMG_Load("container2.png");
    if(loadedSurface == NULL) {
        std::cout << "Unable to load image container2.png. SDL_Image error: " << IMG_GetError() << std::endl;
        return false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, loadedSurface->w, loadedSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, loadedSurface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    SDL_FreeSurface(loadedSurface);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    loadedSurface = IMG_Load("container2_specular.png");
    if(loadedSurface == NULL) {
        std::cout << "Unable to load image container2_specular.png. SDL_Image error: " << IMG_GetError() << std::endl;
        return false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, loadedSurface->w, loadedSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, loadedSurface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    SDL_FreeSurface(loadedSurface);*/

    //defaultShader = new Shader("default.vert", "default.frag");
    lightingShader = new Shader("shaders/lighting.vert", "shaders/lighting.frag");
    lampShader = new Shader("shaders/lamp.vert", "shaders/lamp.frag");

    lightingShader->activate();
    glUniform1i(glGetUniformLocation(lightingShader->programID, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader->programID, "material.specular"), 1);

    return true;
}

void RenderEngine::mouseInput(GLint relX, GLint relY) {
    //For testing purposes
    /*
    if(!(SDL_GetWindowFlags(screen) & SDL_WINDOW_MOUSE_FOCUS)) return;

    yaw += relX / 10.0f;
    pitch += -relY / 10.0f;

    if(pitch > 89.0f) {pitch =  89.0f;}
    else if(pitch < -89.0f) {pitch = -89.0f;}

    glm::vec3 front;
    front.x = float(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
    front.y = float(sin(glm::radians(pitch)));
    front.z = float(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
    cameraFront = glm::normalize(front);
    */
}

void RenderEngine::keyInput(SDL_KeyboardEvent key) {
    SDL_Keycode k = key.keysym.sym;
    if(k == SDLK_DOWN || k == SDLK_s) {
        if(key.type == SDL_KEYDOWN) {down = true;} else {down = false;}
    } else if(k == SDLK_UP || k == SDLK_w) {
        if(key.type == SDL_KEYDOWN) {up = true;} else {up = false;}
    } else if(k == SDLK_RIGHT || k == SDLK_d) {
        if(key.type == SDL_KEYDOWN) {right = true;} else {right = false;}
    } else if(k == SDLK_LEFT || k == SDLK_a) {
        if(key.type == SDL_KEYDOWN) {left = true;} else {left = false;}
    }
}

void RenderEngine::processInput(GLuint deltaTime) {
    GLfloat cameraSpeed = 0.03f * deltaTime;

    if(down) {
        view->moveCamera(0, cameraSpeed);
    }
    if(up) {
        view->moveCamera(1, cameraSpeed);
    }
    if(right) {
        view->moveCamera(2, cameraSpeed);
    }
    if(left) {
        view->moveCamera(3, cameraSpeed);
    }
    glm::vec3 cameraPos = view->getCameraPos();
    world->loadTerrain(cameraPos.x, cameraPos.z + chunkSize / 2);
}

void RenderEngine::render(GLuint deltaTime, GLuint ticks) {
    processInput(deltaTime);

    glClearColor(0.8f, 0.85f, 0.95f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lightingShader->activate();

    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, textures[0]);

    //glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, fmod(ticks / 100.0f, 50.0f) - 40.0f);
    glm::vec3 *lightPositions = new glm::vec3[4];
    lightPositions[0] = glm::vec3(cos(ticks / 500.0f) * 15 + 5, 1.0f, sin(ticks / 500.0f) * 15 - 15);
    lightPositions[1] = glm::vec3(cos(ticks / 300.0f) * 15 + 5, 1.0f, sin(ticks / 500.0f) * 15 - 15);
    lightPositions[2] = glm::vec3(cos(ticks / 500.0f) * 15 + 5, 1.0f, sin(ticks / 300.0f) * 15 - 15);
    lightPositions[3] = glm::vec3(cos(ticks / 200.0f) * 15 + 5, 1.0f, sin(ticks / 200.0f) * 15 - 15);

    GLint matDiffuseLoc = glGetUniformLocation(lightingShader->programID, "material.diffuse");
    GLint matSpecularLoc = glGetUniformLocation(lightingShader->programID, "material.specular");
    GLint matShineLoc = glGetUniformLocation(lightingShader->programID, "material.shininess");

    glUniform3f(matDiffuseLoc, 1.0f, 0.8f, 0.8f);
    glUniform3f(matSpecularLoc, 0.5f, 0.5f, 0.5f);
    glUniform1f(matShineLoc, 32.0f);

    //Directional light
    GLint dirLightDirectionLoc = glGetUniformLocation(lightingShader->programID, "dirLight.direction");
    GLint dirLightAmbientLoc = glGetUniformLocation(lightingShader->programID, "dirLight.ambient");
    GLint dirLightDiffuseLoc = glGetUniformLocation(lightingShader->programID, "dirLight.diffuse");
    GLint dirLightSpecularLoc = glGetUniformLocation(lightingShader->programID, "dirLight.specular");

    glUniform3f(dirLightDirectionLoc, -50.0f, -50.0f, -200.0f);
    glUniform3f(dirLightAmbientLoc, 0.4f, 0.4f, 0.4f);
    glUniform3f(dirLightDiffuseLoc, 0.2f, 0.2f, 0.2f);
    glUniform3f(dirLightSpecularLoc, 0.5f, 0.5f, 0.5f);

    //Pointlight
    for(int i = 0; i < 4; i++) {
        std::string num = "pointLights[" + std::to_string(i);

        GLint pointLightPositionLoc = glGetUniformLocation(lightingShader->programID, (num + "].position").c_str());
        GLint pointLightAmbientLoc = glGetUniformLocation(lightingShader->programID, (num + "].ambient").c_str());
        GLint pointLightDiffuseLoc = glGetUniformLocation(lightingShader->programID, (num + "].diffuse").c_str());
        GLint pointLightSpecularLoc = glGetUniformLocation(lightingShader->programID, (num + "].specular").c_str());
        GLint pointLightConstantLoc = glGetUniformLocation(lightingShader->programID, (num + "].constant").c_str());
        GLint pointLightLinearLoc = glGetUniformLocation(lightingShader->programID, (num + "].linear").c_str());
        GLint pointLightQuadraticLoc = glGetUniformLocation(lightingShader->programID, (num + "].quadratic").c_str());

        glUniform3f(pointLightPositionLoc, lightPositions[i].x, lightPositions[i].y, lightPositions[i].z);
        glUniform3f(pointLightAmbientLoc, 0.2f, 0.2f, 0.2f);
        glUniform3f(pointLightDiffuseLoc, 0.5f, 0.5f, 0.5f);
        glUniform3f(pointLightSpecularLoc, 1.0f, 1.0f, 1.0f);
        glUniform1f(pointLightConstantLoc, 1.0f);
        glUniform1f(pointLightLinearLoc, 0.022f);
        glUniform1f(pointLightQuadraticLoc, 0.0019f);
    }

    GLint spotLightPosLoc = glGetUniformLocation(lightingShader->programID, "spot.position");
    GLint spotLightSpotdirLoc = glGetUniformLocation(lightingShader->programID, "spot.direction");
    GLint spotLightSpotCutOffLoc = glGetUniformLocation(lightingShader->programID, "spot.cutOff");
    GLint spotLightSpotOuterCutOffLoc = glGetUniformLocation(lightingShader->programID, "spot.outerCutOff");
    GLint spotLightAmbientLoc = glGetUniformLocation(lightingShader->programID, "spot.ambient");
    GLint spotLightDiffuseLoc = glGetUniformLocation(lightingShader->programID, "spot.diffuse");
    GLint spotLightSpecularLoc = glGetUniformLocation(lightingShader->programID, "spot.specular");
    GLint spotLightConstantLoc = glGetUniformLocation(lightingShader->programID, "spot.constant");
    GLint spotLightLinearLoc = glGetUniformLocation(lightingShader->programID, "spot.linear");
    GLint spotLightQuadraticLoc = glGetUniformLocation(lightingShader->programID, "spot.quadratic");

    glm::vec3 cameraPos = view->getCameraPos();
    glm::vec3 cameraFront = view->getCameraFront();
    glUniform3f(spotLightPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
    glUniform3f(spotLightSpotdirLoc, cameraFront.x, cameraFront.y, cameraFront.z);
    glUniform1f(spotLightSpotCutOffLoc, glm::cos(glm::radians(12.5f)));
    glUniform1f(spotLightSpotOuterCutOffLoc, glm::cos(glm::radians(14.5f)));
    glUniform3f(spotLightAmbientLoc, 0.5f, 0.5f, 0.5f);
    glUniform3f(spotLightDiffuseLoc, 0.5f, 0.5f, 0.5f);
    glUniform3f(spotLightSpecularLoc, 1.0f, 1.0f, 1.0f);
    glUniform1f(spotLightConstantLoc, 1.0f);
    glUniform1f(spotLightLinearLoc, 0.045f);
    glUniform1f(spotLightQuadraticLoc, 0.0075f);

    glUniform1f(glGetUniformLocation(lightingShader->programID, "light.constant"),  1.0f);
    glUniform1f(glGetUniformLocation(lightingShader->programID, "light.linear"),    0.045f);
    glUniform1f(glGetUniformLocation(lightingShader->programID, "light.quadratic"), 0.0075f);

    GLint viewPosLoc = glGetUniformLocation(lightingShader->programID, "viewPosition");

    glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);

    GLint modelLocation = glGetUniformLocation(lightingShader->programID, "model");
    GLint viewLocation = glGetUniformLocation(lightingShader->programID, "view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view->getViewMatrix()));
    GLint projectionLocation = glGetUniformLocation(lightingShader->programID, "projection");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);

    std::vector<Coordinate> coords = world->getChunkCoordinates();

    for(unsigned long chunk = 0; chunk < coords.size(); chunk++) {
        model = glm::translate(glm::mat4(), glm::vec3(coords[chunk].x * chunkSize, 0.0f, coords[chunk].y * chunkSize));
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
        Chunk *c = world->getChunk(coords[chunk].x, coords[chunk].y);
        c->bindVBO();
        glDrawArrays(GL_TRIANGLES, 0, c->getTriangleCount());
    }


    glBindVertexArray(0);

    lampShader->activate();
    viewLocation = glGetUniformLocation(lampShader->programID, "view");
    projectionLocation = glGetUniformLocation(lampShader->programID, "projection");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view->getViewMatrix()));
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBindVertexArray(lightVAO);
    for(int i = 0; i < 4; i++) {
        modelLocation = glGetUniformLocation(lampShader->programID, "model");

        model = glm::mat4();
        model = glm::translate(model, glm::vec3(lightPositions[i].x, lightPositions[i].y, lightPositions[i].z));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

        glDrawArrays(GL_TRIANGLES, 0, 36);

    }
    glBindVertexArray(0);


    delete[] lightPositions;

    SDL_GL_SwapWindow(screen);
}

RenderEngine::RenderEngine() {
}

RenderEngine::~RenderEngine() {
    delete[] textures;
    delete world;
    delete view;
    delete lightingShader;
    delete lampShader;
}
