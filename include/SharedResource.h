#ifndef SHARED_RESOURCE_H
#define SHARED_RESOURCE_H

#include <iostream>

template<typename T>
class SharedResource
{
public:
    template<typename ...Args>
    SharedResource(Args ...args) : m_resource(args...) { }

    ~SharedResource() = default;
    SharedResource(SharedResource&&) = delete;
    SharedResource(const SharedResource&) = delete;
    SharedResource& operator=(SharedResource&&) = delete;
    SharedResource& operator=(const SharedResource&) = delete;
private:
    T   m_resource;
};

#endif //SHARED_RESOURCE_H
