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
        ~Accessor() = default;

        Accessor(const Accessor&) = delete;
        Accessor& operator=(const Accessor&) = delete;

        Accessor(Accessor&& a) :
            m_lock(std::move(a.m_lock)),
            m_shared_resource(a.m_shared_resource)
        {
            a.m_shared_resource = nullptr;
        }

        Accessor& operator=(Accessor&& a)
        {
            if (&a != this)
            {
                m_lock = std::move(a.m_lock);
                m_shared_resource = a.m_shared_resource;
                a.m_shared_resource = nullptr;
            }
            return *this;
        }

        bool isValid() const noexcept
        {
            return m_shared_resource != nullptr;
        }

        T* operator->()
        {
            return m_shared_resource;
        }

        T& operator*()
        {
            return *m_shared_resource;
        }

        std::unique_lock<std::mutex>& get_lock() noexcept
        {
            return m_lock;
        }

    private:
        Accessor(SharedResource<T> *resource) :
            m_lock(resource->m_mutex),
            m_shared_resource(&resource->m_resource)
        {
        }

        std::unique_lock<std::mutex>    m_lock;
        T                              *m_shared_resource;
    };

    Accessor lock()
    {
        return Accessor(this);
    }

private:
    T           m_resource;
    std::mutex  m_mutex;
};

#endif //SHARED_RESOURCE_H
