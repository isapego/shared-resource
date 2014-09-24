#ifndef SHARED_RESOURCE_H
#define SHARED_RESOURCE_H

#include <mutex>

template<typename T>
class SharedResource
{
public:
    template<typename ...Args>
    SharedResource(Args&& ...args) : m_resource(std::forward<Args>(args)...) { }

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
            release();
        }

        Accessor(const Accessor&) = delete;
        Accessor& operator=(const Accessor&) = delete;

        Accessor(Accessor&& a) :
            m_shared_resource(a.m_shared_resource)
        {
            a.m_shared_resource = nullptr;
        }

        Accessor& operator=(Accessor&& a)
        {
            if (&a != this)
            {
                release();
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
            return &m_shared_resource->m_resource;
        }

        T& operator*()
        {
            return m_shared_resource->m_resource;
        }

    private:
        Accessor(SharedResource<T> *resource) : m_shared_resource(resource)
        {
            m_shared_resource->m_mutex.lock();
        }

        void release()
        {
            if (m_shared_resource != nullptr)
            {
                m_shared_resource->m_mutex.unlock();
            }
        }

        SharedResource<T> *m_shared_resource;
    };


    class ConstAccessor
    {
        friend class SharedResource<T>;
    public:
        ~ConstAccessor()
        {
            release();
        }

        ConstAccessor(const ConstAccessor&) = delete;
        ConstAccessor& operator=(const ConstAccessor&) = delete;

        ConstAccessor(ConstAccessor&& a) :
            m_shared_resource(a.m_shared_resource)
        {
            a.m_shared_resource = nullptr;
        }

        ConstAccessor(Accessor&& a) :
            m_shared_resource(a.m_shared_resource)
        {
            a.m_shared_resource = nullptr;
        }

        ConstAccessor& operator=(ConstAccessor&& a)
        {
            if (&a != this)
            {
                release();
                m_shared_resource = a.m_shared_resource;
                a.m_shared_resource = nullptr;
            }
            return *this;
        }

        ConstAccessor& operator=(Accessor&& a)
        {
            if (&a != this)
            {
                release();
                m_shared_resource = a.m_shared_resource;
                a.m_shared_resource = nullptr;
            }
            return *this;
        }

        bool isValid() const noexcept
        {
            return m_shared_resource != nullptr;
        }

        const T* operator->()
        {
            return &m_shared_resource->m_resource;
        }

        const T& operator*()
        {
            return m_shared_resource->m_resource;
        }

    private:
        ConstAccessor(const SharedResource<T> *resource) : m_shared_resource(resource)
        {
            m_shared_resource->m_mutex.lock();
        }

        void release() const
        {
            if (m_shared_resource != nullptr)
            {
                m_shared_resource->m_mutex.unlock();
            }
        }

        const SharedResource<T> *m_shared_resource;
    };

    Accessor lock()
    {
        return Accessor(this);
    }


    ConstAccessor lockConst() const
    {
        return ConstAccessor(this);
    }

private:
    T                   m_resource;
    mutable std::mutex  m_mutex;
};

#endif //SHARED_RESOURCE_H
