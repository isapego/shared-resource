#ifndef SHARED_RESOURCE_H
#define SHARED_RESOURCE_H

#include <mutex>

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

    class Accessor
    {
        friend class SharedResource<T>;
    public:
        ~Accessor()
        {
            m_shared_resource.m_mutex.unlock();
        }

    private:
        Accessor(SharedResource<T> &resource) : m_shared_resource(resource)
        {
            m_shared_resource.m_mutex.lock();
        }

        SharedResource<T> &m_shared_resource;
    };

    Accessor lock()
    {
        return Accessor(*this);
    }

private:
    T           m_resource;
    std::mutex  m_mutex;
};

#endif //SHARED_RESOURCE_H
