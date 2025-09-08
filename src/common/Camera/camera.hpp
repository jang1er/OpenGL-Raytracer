#ifndef CAMERA_H
#define CAMERA_H

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <common/InputControl/InputController.hpp>
#include <common/InputControl/IKeyboardListener.hpp>
#include <common/InputControl/IMousePosListener.hpp>
#include <common/InputControl/IScrollListener.hpp>
#include <glm/gtx/string_cast.hpp>



const float YAW = 0.0f;
const float PITCH = 0.0f;
const float SPEED = 1.5f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 45.0f;

class Camera : public IKeyboardListener, public IMousePosListener {
    public:
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;
        bool ConstraintPitch;

        Camera(GLFWwindow* window, std::string name = "Default Camera", glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f), float Yaw = YAW, float Pitch = PITCH) : m_Front(glm::vec3(0.0f, 0.0f, 1-.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
        {
            m_ContextWindow = window;
            m_name = name;
            m_cameraPos = cameraPos;
            m_WorldUp = Up;
            m_Yaw = Yaw;
            m_Pitch = Pitch;
            m_Changed = true;

            m_LastTime = glfwGetTime();

            // set first mouse input
            glfwGetCursorPos(m_ContextWindow, &m_lastX, &m_lastY);

            // compute all matrices for first time
            UpdateCameraVectors();
        }

        ~Camera(){
            // disable camera by unsubcribing from input

        }

        void enable(){
            InputController::addKeyBoardListener(this, NOTIFY_ON_PRESS | NOTIFY_ON_RELEASE);
            InputController::addMousePosListener(this);
            glfwSetInputMode(m_ContextWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        void disable(){
            InputController::removeKeyBoardListener(this, NOTIFY_ON_PRESS | NOTIFY_ON_RELEASE);
            InputController::removeMousePosListener(this);
            glfwSetInputMode(m_ContextWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        glm::mat4 GetViewMatrix(){
            double time = glfwGetTime();
            float delta = static_cast<float>(time - m_LastTime);
            m_LastTime = time;

            //normalize movement vector only if it is not zero
            glm::vec3 moveDirection = glm::vec3(0.0f);
            if(!glm::all(glm::equal(glm::vec3(0.0f),m_movementVec))){
                moveDirection = glm::normalize(m_movementVec.x * m_Front + m_movementVec.y * m_Up + m_movementVec.z * m_Right);
                m_cameraPos += moveDirection * MovementSpeed * delta;
                std::cout << "Front:" << glm::to_string(m_Front) << std::endl;
                std::cout << "Pos:" << glm::to_string(m_cameraPos) << std::endl;
            }


            m_ViewMatrix = glm::lookAt(m_cameraPos, m_cameraPos + m_Front, m_Up);
            return m_ViewMatrix;
        }

        void HandleKey(int key, int scancode, int action, int mods) override {
            std::cout << key << std::endl;
            switch(key){
                case GLFW_KEY_W:
                    m_movementVec.x += action ? 1 : -1;
                    break;
                case GLFW_KEY_S:
                    m_movementVec.x += action ? -1 : 1;
                    break;
                case GLFW_KEY_A:
                    m_movementVec.z += action ? -1 : 1;
                    break;
                case GLFW_KEY_D:
                    m_movementVec.z += action ? 1 : -1;
                    break;
                case GLFW_KEY_SPACE:
                    m_movementVec.y += action ? 1 : -1;
                    break;
                case GLFW_KEY_LEFT_SHIFT:
                    m_movementVec.y += action ? -1 : 1;
                    break;
                default:
                    std::cout << "Pressed Key: " << key << " | " << action << std::endl;
            }
        }

        void HandleMouseMove(double xpos, double ypos){
            m_Changed = true;
            
            // get mouse delta
            float xoffset = static_cast<float>(xpos - m_lastX);
            float yoffset = static_cast<float>(m_lastY - ypos);

            m_lastX = xpos;
            m_lastY = ypos;
            
            xoffset *= MouseSensitivity;
            yoffset *= MouseSensitivity;

            m_Yaw += xoffset;
            m_Pitch += yoffset;
            
            if(ConstraintPitch){
                if(m_Pitch > 89.0f)m_Pitch = 89.0f;
                if(m_Pitch < -89.0f)m_Pitch = -89.0f;
            }
            UpdateCameraVectors();
        }

        void handleScroll(double xoffset, double yoffset){

        }

    private:

    double m_LastTime;

    double m_lastX;

    double m_lastY;

    std::string m_name;

    /// @brief Window context this camera lives in
    GLFWwindow* m_ContextWindow;

    /// @brief View Matrix of this camera
    glm::mat4 m_ViewMatrix;


    glm::vec3 m_movementVec = glm::vec3{0.0f, 0.0f, 0.0f};

    /// @brief Represents the camera Position
    glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

    /// @brief camera front facing vector
    glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);

    /// @brief camera up facing vector
    glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);

    /// @brief camera right facing vectr
    glm::vec3 m_Right;

    /// @brief up facing vector of the world
    glm::vec3 m_WorldUp;

    /// @brief Yaw of camera
    float m_Yaw;

    /// @brief Pitch of camera
    float m_Pitch;

    bool m_Changed = false;

    /// @brief 
    void UpdateCameraVectors(){
        glm::vec3 front;
        front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front.y = sin(glm::radians(m_Pitch));
        front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        m_Front = glm::normalize(front);
        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));
    }
};

#endif