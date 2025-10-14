#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <chrono>
#include <thread>

#include <algorithm>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include "common/Shaders/shader.hpp"
#include "common/Camera/camera.hpp"
#include "common/Textures/TextureManager.hpp"

#include "common/Objects/Model.hpp"

using namespace glm;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Simple Raytracer", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    if (!GLAD_GL_ARB_bindless_texture) {
        std::cerr << "Bindless textures not supported!\n";
    }

    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

    // init InputControl
    InputController::init(window);


    Shader programShader = Shader("ressources/Shaders/texture.vert", "ressources/Shaders/texture.frag");

    glEnableVertexAttribArray(2);

    auto textureManager = std::make_shared<TextureManager>();

    Model ourModel(textureManager, "ressources/Models/backpack/backpack.obj");


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // create default camera
    Camera camera = Camera(window);
    camera.enable();

    
    // view matrix
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection;
    glm::mat4 model;
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);

    double previousTime = 0.0;
    double currentTime = 0.0;
    double timeDiff;
    unsigned int counter = 0;
    
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        currentTime = glfwGetTime();
        timeDiff = currentTime - previousTime;
        counter++;
        if(timeDiff >= 1.0 / 30){
            std::string fps = std::to_string((1.0 / timeDiff) * counter);
            std::string ms = std::to_string((timeDiff / counter) * 1000);
            std::string windowTitle = "OpenGL Render Engine - " + fps + " fps / " + ms + "ms";
            glfwSetWindowTitle(window, windowTitle.c_str());
            previousTime = currentTime;
            counter = 0;
        }

        // input
        processInput(window);
        
        // projection matrix
        projection = glm::perspective(glm::radians(45.0f), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
        // render
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        // compute MVP Matrix
        view = camera.GetViewMatrix();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));


        // draw our first triangle
        programShader.use();

        //send MVP to shader
        programShader.setMat4Float("model", model);
        programShader.setMat4Float("view", view);
        programShader.setMat4Float("projection", projection);

        ourModel.Draw(programShader);

        //glBindTexture(GL_TEXTURE_2D, textureID);
        //glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        ////glDrawArrays(GL_TRIANGLES, 0, 6);
        //glDrawElements(GL_TRIANGLES, sizeof(indices) / 3, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    programShader.destroy();
    textureManager->Clear();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);

    SCR_HEIGHT = height;
    SCR_WIDTH = width;
}