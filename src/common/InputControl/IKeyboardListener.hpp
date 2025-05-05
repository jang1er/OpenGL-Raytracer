#ifndef IKEYBOARD_H
#define IKEYBOARD_H

class IKeyboardListener{
    public:
        virtual void HandleKey(int key, int scancode, int action, int mods) = 0;
        virtual ~IKeyboardListener() = default;
};

#endif