#include "SharedResource.h"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(Basic_construction)
{
    SharedResource<int> shared_int;
}


BOOST_AUTO_TEST_CASE(Construction_with_argument)
{
    SharedResource<int> shared_int(5);
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
    SharedResource<int> shared_int(0);
    shared_int.lock();
}


BOOST_AUTO_TEST_CASE(Locking_with_accessor)
{
    SharedResource<int> shared_int(0);
    auto shared_int_accessor = shared_int.lock();
}


BOOST_AUTO_TEST_CASE(Accessor_isValid)
{
    SharedResource<int> shared_int(0);
    auto shared_int_accessor = shared_int.lock();
    BOOST_CHECK(shared_int_accessor.isValid());
    auto shared_int_accessor_new(std::move(shared_int_accessor));
    BOOST_CHECK(!shared_int_accessor.isValid());
    BOOST_CHECK(shared_int_accessor_new.isValid());
}

