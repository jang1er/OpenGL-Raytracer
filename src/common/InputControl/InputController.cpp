#include <common/InputControl/InputController.hpp>

#include <vector>
#include <algorithm>

std::vector<Entry<IKeyboardListener*>> InputController::keyboardListeners;
std::vector<Entry<IMousePosListener*>> InputController::mousePosListeners;
std::vector<Entry<IMouseClickListener*>> InputController::mouseClickListeners;
std::vector<Entry<IScrollListener*>> InputController::scrollListeners;

void InputController::init(GLFWwindow* window){
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);
}

// ------- Listener Add Functions --------

/// @brief 
/// @param listener 
/// @param flags 
void InputController::addKeyBoardListener(IKeyboardListener* listener, unsigned short flags){
    Entry<IKeyboardListener*> keyboard(listener, flags);
    keyboardListeners.push_back(keyboard);
}

/// @brief 
/// @param listener 
/// @param flags 
void InputController::addMousePosListener(IMousePosListener* listener, unsigned short flags){
    Entry<IMousePosListener*> mousePos(listener, flags);
    mousePosListeners.push_back(mousePos);
}


void InputController::addMouseClickListener(IMouseClickListener* listener, unsigned short flags){
    Entry<IMouseClickListener*> mouseClick(listener, flags);
    mouseClickListeners.push_back(mouseClick);
}

/// @brief 
/// @param listener 
/// @param flags 
void InputController::addScrollListener(IScrollListener* listener, unsigned short flags){
    Entry<IScrollListener*> scroll(listener, flags);
    scrollListeners.push_back(scroll);
}


// -------- Listener Remove Function
bool InputController::removeKeyBoardListener(IKeyboardListener* listener, unsigned short flags){
    const auto removed = std::erase_if(keyboardListeners, [listener, flags](const auto& entry){
        return listener == entry.listener && flags == entry.notifyFlags;
    });
    return removed > 0;
}

bool InputController::removeMousePosListener(IMousePosListener* listener, unsigned short flags){
    const auto removed = std::erase_if(mousePosListeners, [listener, flags](const auto& entry){
        return listener == entry.listener && flags == entry.notifyFlags;
    });
    return removed > 0;
}

bool InputController::removeMouseClickListener(IMouseClickListener* listener, unsigned short flags){
    const auto removed = std::erase_if(mouseClickListeners, [listener, flags](const auto& entry){
        return listener == entry.listener && flags == entry.notifyFlags;
    });
    return removed > 0;
}

bool InputController::removeScrollListener(IScrollListener* listener, unsigned short flags){
    const auto removed = std::erase_if(scrollListeners, [listener, flags](const auto& entry){
        return listener == entry.listener && flags == entry.notifyFlags;
    });
    return removed > 0;
}


// -------- GLFW Call Back Functions

/// @brief 
/// @param window 
/// @param key 
/// @param scancode 
/// @param action 
/// @param mods 
void InputController::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    unsigned short flag = 1 << action;
    for(auto entry : keyboardListeners){
        if(flag & entry.notifyFlags)entry.listener->HandleKey(key, scancode, action, mods);
    }
}

/// @brief 
/// @param window 
/// @param button 
/// @param action 
/// @param mods 
void InputController::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
    unsigned short flag = 1 << action;
    for(auto entry : mouseClickListeners){
        if(flag & entry.notifyFlags)entry.listener->HandleMouseClick(button, action, mods);
    }
}

/// @brief 
/// @param window 
/// @param xpos 
/// @param ypos 
void InputController::cursorPosCallback(GLFWwindow* window, double xpos, double ypos){
    for(auto entry : mousePosListeners){
        entry.listener->HandleMouseMove(xpos, ypos);
    }
}

/// @brief 
/// @param window
/// @param xoffset 
/// @param yoffset 
void InputController::scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
    for(auto entry : scrollListeners){
        entry.listener->HandleScrollCallback(xoffset, yoffset);
    }
}