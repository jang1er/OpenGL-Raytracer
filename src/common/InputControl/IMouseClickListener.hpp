#ifndef IMOUSE_CLICK_H
#define IMOUSE_CLICK_H

class IMouseClickListener{
    public:
        virtual void HandleMouseClick(int button, int action, int mods) = 0;
        virtual ~IMouseClickListener() = default;
};

#endif