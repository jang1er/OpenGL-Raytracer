#ifndef INPUT_H
#define INPUT_H

#define NOTIFY_ON_ALL       7
#define NOTIFY_ON_REPEAT    4
#define NOTIFY_ON_PRESS     2
#define NOTIFY_ON_RELEASE   1
#define NOTIFY_ON_NONE      0

#include <GLFW/glfw3.h>
#include <vector>

#include <common/InputControl/IKeyboardListener.hpp>
#include <common/InputControl/IMousePosListener.hpp>
#include <common/InputControl/IMouseClickListener.hpp>
#include <common/InputControl/IScrollListener.hpp>

template <typename T> class Entry {
    public:
    /// @brief Listener for this input entry
    T listener;
    /// @brief Flags for the input entry
    unsigned short notifyFlags;

    Entry(T object, unsigned short flags) : listener(object), notifyFlags(flags) {}
};

class InputController {
    public:
        static void init(GLFWwindow* window);
        static void addKeyBoardListener(IKeyboardListener* listener, unsigned short flags = NOTIFY_ON_ALL);
        static void addMousePosListener(IMousePosListener* listener, unsigned short flags = NOTIFY_ON_ALL);
        static void addMouseClickListener(IMouseClickListener* listener, unsigned short flags = NOTIFY_ON_ALL);
        static void addScrollListener(IScrollListener* listener, unsigned short flags = NOTIFY_ON_ALL);

        static bool removeKeyBoardListener(IKeyboardListener* listener, unsigned short flags = NOTIFY_ON_ALL);
        static bool removeMousePosListener(IMousePosListener* listener, unsigned short flags = NOTIFY_ON_ALL);
        static bool removeMouseClickListener(IMouseClickListener* listener, unsigned short flags = NOTIFY_ON_ALL);
        static bool removeScrollListener(IScrollListener* listener, unsigned short flags = NOTIFY_ON_ALL);


    private:
        static std::vector<Entry<IKeyboardListener*>> keyboardListeners;
        static std::vector<Entry<IMousePosListener*>> mousePosListeners;
        static std::vector<Entry<IMouseClickListener*>> mouseClickListeners;
        static std::vector<Entry<IScrollListener*>> scrollListeners;

        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

};


#endif