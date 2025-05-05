#ifndef IMOUSE_POS_H
#define IMOUSE_POS_H

class IMousePosListener {
    public:
        virtual void HandleMouseMove(double xpos, double ypos) = 0;
        virtual ~IMousePosListener() = default;
};

#endif