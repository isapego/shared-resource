#ifndef SHARED_RESOURCE_H
#define SHARED_RESOURCE_H

template<typename T>
class SharedResource
{
public:
	SharedResource() = default;
	~SharedResource() = default;
	SharedResource(SharedResource&&) = delete;
	SharedResource(const SharedResource&) = delete;
	SharedResource& operator=(SharedResource&&) = delete;
	SharedResource& operator=(const SharedResource&) = delete;
private:
};

#endif //SHARED_RESOURCE_H
