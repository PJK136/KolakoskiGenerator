#ifndef CONTAINER_H
#define CONTAINER_H

class Container
{
public:
    virtual ~Container() {}
    virtual void push(unsigned char) = 0;
    virtual unsigned char first() = 0;
    virtual unsigned char pop() = 0;
    virtual void clear() = 0;
    virtual unsigned long long size() = 0;
    virtual unsigned long long approximateSize() = 0;
    virtual bool isEmpty() = 0;
};

#endif // CONTAINER_H
