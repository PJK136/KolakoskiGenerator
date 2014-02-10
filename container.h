#ifndef CONTAINER_H
#define CONTAINER_H

//#define INHERIT_CONTAINER // À désactiver si + de perf

#ifdef INHERIT_CONTAINER
class Container
{
public:
    virtual ~Container() {}
    virtual void push(unsigned char) = 0;
    virtual unsigned char front() = 0;
    virtual void pop() = 0;
    virtual void clear() = 0;
    virtual std::size_t size() = 0;
    virtual std::size_t approximateSize() = 0;
    virtual bool empty() = 0;
    virtual bool hasNext() = 0;
};
#endif

#endif // CONTAINER_H
