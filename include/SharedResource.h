#ifndef SHARED_RESOURCE_H
#define SHARED_RESOURCE_H

#include <mutex>
#include <condition_variable>

template<typename T>
class SharedResource
{
    class AccessorBase
    {
    public:

        std::unique_lock<std::mutex>& get_lock() noexcept
        {
            return m_lock;
        }

    protected:
        AccessorBase(const SharedResource<T> *resource) : m_lock(resource->m_mutex) { }
        ~AccessorBase() = default;

        AccessorBase(AccessorBase&& a) : m_lock(std::move(a.m_lock)) { }

        AccessorBase& operator=(AccessorBase&& a)
        {
            if (&a != this)
            {
                m_lock = std::move(a.m_lock);
            }
            return *this;
        }

    private:
        std::unique_lock<std::mutex> m_lock;
    };

public:
    template<typename ...Args>
    SharedResource(Args&& ...args) : m_resource(std::forward<Args>(args)...) { }

    ~SharedResource() = default;
    SharedResource(SharedResource&&) = delete;
    SharedResource(const SharedResource&) = delete;
    SharedResource& operator=(SharedResource&&) = delete;
    SharedResource& operator=(const SharedResource&) = delete;

    class Accessor : public AccessorBase
    {
        friend class SharedResource<T>;
    public:
        ~Accessor() = default;

        Accessor(const Accessor&) = delete;
        Accessor& operator=(const Accessor&) = delete;

        Accessor(Accessor&& a) :
            AccessorBase(std::move(a)),
            m_shared_resource(a.m_shared_resource)
        {
            a.m_shared_resource = nullptr;
        }

        Accessor& operator=(Accessor&& a)
        {
            if (&a != this)
            {
                AccessorBase::operator=(std::move(a));
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

    private:
        Accessor(SharedResource<T> *resource) :
            AccessorBase(resource),
            m_shared_resource(&resource->m_resource) { }

        T   *m_shared_resource;
    };


    class ConstAccessor : public AccessorBase
    {
        friend class SharedResource<T>;
    public:
        ~ConstAccessor() = default;

        ConstAccessor(const ConstAccessor&) = delete;
        ConstAccessor& operator=(const ConstAccessor&) = delete;

        ConstAccessor(ConstAccessor&& a) :
            AccessorBase(std::move(a)),
            m_shared_resource(a.m_shared_resource)
        {
            a.m_shared_resource = nullptr;
        }

        ConstAccessor(Accessor&& a) :
            AccessorBase(std::move(a)),
            m_shared_resource(a.m_shared_resource)
        {
            a.m_shared_resource = nullptr;
        }

        ConstAccessor& operator=(ConstAccessor&& a)
        {
            if (&a != this)
            {
                AccessorBase::operator=(std::move(a));
                m_shared_resource = a.m_shared_resource;
                a.m_shared_resource = nullptr;
            }
            return *this;
        }

        ConstAccessor& operator=(Accessor&& a)
        {
            if (&a != this)
            {
                AccessorBase::operator=(std::move(a));
                m_shared_resource = a.m_shared_resource;
                a.m_shared_resource = nullptr;
            }
            return *this;
        }

        bool isValid() const noexcept
        {
            return m_shared_resource != nullptr;
        }

        const T* operator->() const
        {
            return m_shared_resource;
        }

        const T& operator*() const
        {
            return *m_shared_resource;
        }

    private:
        ConstAccessor(const SharedResource<T> *resource) :
            AccessorBase(resource),
            m_shared_resource(&resource->m_resource) { }

        const T *m_shared_resource;
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
