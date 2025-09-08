#ifndef SHADER_H
#define SHADER_H

#define LOG_SIZE 1024

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader{
    public:
        // shader program id
        GLuint ID;

        /// @brief Constructs the shader from the paths
        /// @param vertexPath Path to the vertex shader
        /// @param fragmentPath Path to the fragment shader
        Shader(const char* vertexPath, const char* fragmentPath){
            // get vertex and fragment shader source code
            std::string vertexCode, fragmentCode;
            std::ifstream vertexShaderFile, fragmentShaderFile;
            vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

            try {
                // open files
                vertexShaderFile.open(vertexPath);
                fragmentShaderFile.open(fragmentPath);
                std::stringstream vertexShaderStream, fragmentShaderStream;

                // read files into the shader buffer
                vertexShaderStream << vertexShaderFile.rdbuf();
                fragmentShaderStream << fragmentShaderFile.rdbuf();

                // close file handlers
                vertexShaderFile.close();
                fragmentShaderFile.close();

                // convert stream into string
                vertexCode = vertexShaderStream.str();
                fragmentCode = fragmentShaderStream.str();
            }
            catch(std::ifstream::failure& e){
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
            }

            const char* vertexShader = vertexCode.c_str();
            const char* fragmentShader = fragmentCode.c_str();

            // Compile the shaders
            unsigned int vertexID, fragmentID;
            int success;
            char infoLog[LOG_SIZE];

            // vertex shader
            vertexID = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexID, 1, &vertexShader, NULL);
            glCompileShader(vertexID);

            // check for errors
            CheckCompileErrors(vertexID, "VERTEX");

            // fragment shader
            fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentID, 1, &fragmentShader, NULL);
            glCompileShader(fragmentID);

            //check for errors
            CheckCompileErrors(fragmentID, "FRAGMENT");

            // link into shader Programm
            ID = glCreateProgram();
            glAttachShader(ID, vertexID);
            glAttachShader(ID, fragmentID);
            glLinkProgram(ID);
            // print linking errors
            CheckCompileErrors(ID, "PROGRAM");

            //delete vertex- and fragmentshader, as they are now inside our program
            glDeleteShader(vertexID);
            glDeleteShader(fragmentID);
        }

        /// @brief Deletes the shader in OpenGL
        void destroy(){
            glDeleteProgram(ID);
            ID = 0;
        }

        /// @brief Set this shader to be used dor rendering
        void use() 
        { 
            glUseProgram(ID); 
        }
        
        /// @brief Sets a Uniform Bool in the shader
        /// @param name Name of the Uniform
        /// @param value Value to be set
        void setBool(const std::string &name, bool value) const
        {         
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
        }

        /// @brief Sets a Uniform Int in the shader
        /// @param name Name of the Uniform
        /// @param value Value to be set
        void setInt(const std::string &name, int value) const
        { 
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
        }

        /// @brief Sets a Uniform Float in the shader
        /// @param name Name of the Uniform
        /// @param value Value to be set
        void setFloat(const std::string &name, float value) const
        { 
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
        }

        /// @brief Sets a Uniform Matrix4 Float in the shader
        /// @param name Name of the Uniform
        /// @param value Value to be set
        void setMat4Float(const std::string &name, glm::mat4 &value) const{
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
        }

    private:
        /// @brief Checks for compile errors and prints them to the console
        /// @param shader Shader to check
        /// @param type Type of program to check
        void CheckCompileErrors(unsigned int shader, std::string type){
            int success;
            char infoLog[LOG_SIZE];
            if(type != "PROGRAM"){
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if(!success){
                    glGetShaderInfoLog(shader, LOG_SIZE, NULL, infoLog);
                    std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- ------------------------------------------ -- " << std::endl;
                }
            } else {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if(!success){
                    glGetShaderInfoLog(shader, LOG_SIZE, NULL, infoLog);
                    std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- ------------------------------------------ -- " << std::endl;
                }
            }
        }
};

#endif
