#include "SharedResource.h"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(Basic_construction)
{
    SharedResource<int> shared_test_class;
}


BOOST_AUTO_TEST_CASE(Construction_with_argument)
{
    SharedResource<int> shared_test_class(5);
}


BOOST_AUTO_TEST_CASE(Construction_no_default_constructor)
{
    struct TestClass
    {
        TestClass() = delete;
        TestClass(int) { }
    };

    SharedResource<TestClass> stc2(5);
}


BOOST_AUTO_TEST_CASE(Basic_Locking)
{
    SharedResource<int> shared_test_class(0);
    shared_int.lock();
}


BOOST_AUTO_TEST_CASE(Locking_with_accessor)
{
    SharedResource<int> shared_test_class(0);
    auto shared_int_accessor = shared_test_class.lock();
}


BOOST_AUTO_TEST_CASE(Accessor_isValid)
{
    SharedResource<int> shared_test_class(0);
    auto shared_int_accessor = shared_test_class.lock();
    BOOST_CHECK(shared_int_accessor.isValid());
    auto shared_int_accessor_new(std::move(shared_int_accessor));
    BOOST_CHECK(!shared_int_accessor.isValid());
    BOOST_CHECK(shared_int_accessor_new.isValid());
}


BOOST_AUTO_TEST_CASE(Acessor_dereferencing_1)
{
    SharedResource<int> shared_test_class(0);
    auto shared_int_accessor = shared_test_class.lock();
    BOOST_CHECK_EQUAL(0, *shared_int_accessor);
    *shared_int_accessor = 5;
    BOOST_CHECK_EQUAL(5, *shared_int_accessor);
}


BOOST_AUTO_TEST_CASE(Acessor_dereferencing_2)
{
    struct TestClass
    {
        TestClass(int a) : m_a(a) { }
        void set(int a) noexcept { m_a = a; }
        int get() const noexcept { return m_a; }
    private:
        int m_a;
    };

    SharedResource<TestClass> shared_test_class(0);
    auto shared_int_accessor = shared_test_class.lock();
    BOOST_CHECK_EQUAL(0, shared_int_accessor->get());
    shared_int_accessor->set(5);
    BOOST_CHECK_EQUAL(5, shared_int_accessor->get());
}

