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


BOOST_AUTO_TEST_CASE(Construction_no_default)
{
    struct TestClass
    {
        TestClass() = delete;
        TestClass(int) { }
    };

    SharedResource<TestClass> stc2(5);
}
