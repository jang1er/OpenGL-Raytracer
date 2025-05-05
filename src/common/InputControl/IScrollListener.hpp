#ifndef ISCROLL_H
#define ISCROLL_H

class IScrollListener {
    public:
        virtual void HandleScrollCallback(double xoffset, double yoffset) = 0;
        virtual ~IScrollListener() = default;
};

#endif