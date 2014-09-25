#include <thread>
#include <chrono>
#include <condition_variable>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "SharedResource.h"


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


BOOST_AUTO_TEST_CASE(Accessor_dereferencing_1)
{
    SharedResource<int> shared_int(0);
    auto shared_int_accessor = shared_int.lock();
    BOOST_CHECK_EQUAL(0, *shared_int_accessor);
    *shared_int_accessor = 5;
    BOOST_CHECK_EQUAL(5, *shared_int_accessor);
}


BOOST_AUTO_TEST_CASE(Accessor_dereferencing_2)
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


BOOST_AUTO_TEST_CASE(Concurrency_basic)
{
    SharedResource<int> shared_int(0);

    std::thread test_thread([&shared_int]()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        auto shared_int_accessor = shared_int.lock();
        *shared_int_accessor = 7;
    });

    {
        auto shared_int_accessor = shared_int.lock();
        BOOST_CHECK_EQUAL(0, *shared_int_accessor);
        *shared_int_accessor = 5;
        BOOST_CHECK_EQUAL(5, *shared_int_accessor);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        BOOST_CHECK_EQUAL(5, *shared_int_accessor);
    }
    test_thread.join();
    auto shared_int_accessor = shared_int.lock();
    BOOST_CHECK_EQUAL(7, *shared_int_accessor);
}


BOOST_AUTO_TEST_CASE(Acessor_move_construction)
{
    SharedResource<int> shared_int(42);
    auto shared_int_accessor = shared_int.lock();
    BOOST_REQUIRE(shared_int_accessor.isValid());
    BOOST_CHECK_EQUAL(42, *shared_int_accessor);
    auto shared_int_accessor_2(std::move(shared_int_accessor));
    BOOST_CHECK(!shared_int_accessor.isValid());
    BOOST_REQUIRE(shared_int_accessor_2.isValid());
    BOOST_CHECK_EQUAL(42, *shared_int_accessor_2);
}


BOOST_AUTO_TEST_CASE(Acessor_move_assigment)
{
    SharedResource<int> shared_int(42);
    auto shared_int_accessor = shared_int.lock();
    BOOST_REQUIRE(shared_int_accessor.isValid());
    BOOST_CHECK_EQUAL(42, *shared_int_accessor);

    auto shared_int_accessor_2(std::move(shared_int_accessor));
    BOOST_CHECK(!shared_int_accessor.isValid());
    BOOST_REQUIRE(shared_int_accessor_2.isValid());
    BOOST_CHECK_EQUAL(42, *shared_int_accessor_2);

    shared_int_accessor = std::move(shared_int_accessor_2);
    BOOST_CHECK(!shared_int_accessor_2.isValid());
    BOOST_REQUIRE(shared_int_accessor.isValid());
    BOOST_CHECK_EQUAL(42, *shared_int_accessor);
}


BOOST_AUTO_TEST_CASE(Acessor_move_assigment_self)
{
    SharedResource<int> shared_int(42);
    auto shared_int_accessor = shared_int.lock();
    BOOST_REQUIRE(shared_int_accessor.isValid());
    BOOST_CHECK_EQUAL(42, *shared_int_accessor);

    shared_int_accessor = std::move(shared_int_accessor);
    BOOST_REQUIRE(shared_int_accessor.isValid());
    BOOST_CHECK_EQUAL(42, *shared_int_accessor);
}


BOOST_AUTO_TEST_CASE(Acessor_release_on_assigment)
{
    SharedResource<int> shared_int_1(42);
    SharedResource<int> shared_int_2(69);

    auto shared_int_1_accessor = shared_int_1.lock();
    auto some_shared_int_accessor = shared_int_2.lock();

    some_shared_int_accessor = std::move(shared_int_1_accessor);
    auto shared_int_2_accessor = shared_int_2.lock();
}


BOOST_AUTO_TEST_CASE(Accessor_move_construction)
{
    SharedResource<int> shared_int(42);
    auto shared_int_accessor = shared_int.lock();
    BOOST_REQUIRE(shared_int_accessor.isValid());
    BOOST_CHECK_EQUAL(42, *shared_int_accessor);
    auto shared_int_accessor_2(std::move(shared_int_accessor));
    BOOST_CHECK(!shared_int_accessor.isValid());
    BOOST_REQUIRE(shared_int_accessor_2.isValid());
    BOOST_CHECK_EQUAL(42, *shared_int_accessor_2);
}


BOOST_AUTO_TEST_CASE(Accessor_move_assigment)
{
    SharedResource<int> shared_int(42);
    auto shared_int_accessor = shared_int.lock();
    BOOST_REQUIRE(shared_int_accessor.isValid());
    BOOST_CHECK_EQUAL(42, *shared_int_accessor);

    auto shared_int_accessor_2(std::move(shared_int_accessor));
    BOOST_CHECK(!shared_int_accessor.isValid());
    BOOST_REQUIRE(shared_int_accessor_2.isValid());
    BOOST_CHECK_EQUAL(42, *shared_int_accessor_2);

    shared_int_accessor = std::move(shared_int_accessor_2);
    BOOST_CHECK(!shared_int_accessor_2.isValid());
    BOOST_REQUIRE(shared_int_accessor.isValid());
    BOOST_CHECK_EQUAL(42, *shared_int_accessor);
}


BOOST_AUTO_TEST_CASE(Accessor_move_assigment_self)
{
    SharedResource<int> shared_int(42);
    auto shared_int_accessor = shared_int.lock();
    BOOST_REQUIRE(shared_int_accessor.isValid());
    BOOST_CHECK_EQUAL(42, *shared_int_accessor);

    shared_int_accessor = std::move(shared_int_accessor);
    BOOST_REQUIRE(shared_int_accessor.isValid());
    BOOST_CHECK_EQUAL(42, *shared_int_accessor);
}


BOOST_AUTO_TEST_CASE(Accessor_release_on_assigment)
{
    SharedResource<int> shared_int_1(42);
    SharedResource<int> shared_int_2(69);

    auto shared_int_1_accessor = shared_int_1.lock();
    auto some_shared_int_accessor = shared_int_2.lock();

    some_shared_int_accessor = std::move(shared_int_1_accessor);
    auto shared_int_2_accessor = shared_int_2.lock();
}


BOOST_AUTO_TEST_CASE(Const_Locking)
{
    const SharedResource<int> shared_int(42);
    shared_int.lockConst();
}


BOOST_AUTO_TEST_CASE(Locking_with_ConstAccessor)
{
    const SharedResource<int> shared_int(42);
    auto shared_int_accessor = shared_int.lockConst();
}


BOOST_AUTO_TEST_CASE(ConstAccessor_isValid)
{
    const SharedResource<int> shared_int(42);
    auto shared_int_accessor = shared_int.lockConst();
    BOOST_CHECK(shared_int_accessor.isValid());
    auto shared_int_accessor_new(std::move(shared_int_accessor));
    BOOST_CHECK(!shared_int_accessor.isValid());
    BOOST_CHECK(shared_int_accessor_new.isValid());
}


BOOST_AUTO_TEST_CASE(ConstAccessor_dereferencing_1)
{
    SharedResource<int> shared_int(42);
    auto shared_int_accessor = shared_int.lockConst();
    BOOST_REQUIRE(shared_int_accessor.isValid());
    BOOST_CHECK_EQUAL(42, *shared_int_accessor);
}


BOOST_AUTO_TEST_CASE(ConstAccessor_dereferencing_2)
{
    struct TestClass
    {
        TestClass(int a) : m_a(a) { }
        void set(int a) noexcept { m_a = a; }
        int get() const noexcept { return m_a; }
    private:
        int m_a;
    };

    const SharedResource<TestClass> shared_test_class(42);
    auto shared_int_accessor = shared_test_class.lockConst();
    BOOST_CHECK_EQUAL(42, shared_int_accessor->get());
}


BOOST_AUTO_TEST_CASE(ConstAccessor_move_construction)
{
    SharedResource<int> shared_int(42);
    auto shared_int_accessor = shared_int.lockConst();
    BOOST_REQUIRE(shared_int_accessor.isValid());
    BOOST_CHECK_EQUAL(42, *shared_int_accessor);
    auto shared_int_accessor_2(std::move(shared_int_accessor));
    BOOST_CHECK(!shared_int_accessor.isValid());
    BOOST_REQUIRE(shared_int_accessor_2.isValid());
    BOOST_CHECK_EQUAL(42, *shared_int_accessor_2);
}


BOOST_AUTO_TEST_CASE(ConstAccessor_move_assigment)
{
    const SharedResource<int> shared_int(42);
    auto shared_int_accessor = shared_int.lockConst();
    BOOST_REQUIRE(shared_int_accessor.isValid());
    BOOST_CHECK_EQUAL(42, *shared_int_accessor);

    auto shared_int_accessor_2(std::move(shared_int_accessor));
    BOOST_CHECK(!shared_int_accessor.isValid());
    BOOST_REQUIRE(shared_int_accessor_2.isValid());
    BOOST_CHECK_EQUAL(42, *shared_int_accessor_2);

    shared_int_accessor = std::move(shared_int_accessor_2);
    BOOST_CHECK(!shared_int_accessor_2.isValid());
    BOOST_REQUIRE(shared_int_accessor.isValid());
    BOOST_CHECK_EQUAL(42, *shared_int_accessor);
}


BOOST_AUTO_TEST_CASE(ConstAccessor_move_assigment_self)
{
    const SharedResource<int> shared_int(42);
    auto shared_int_accessor = shared_int.lockConst();
    BOOST_REQUIRE(shared_int_accessor.isValid());
    BOOST_CHECK_EQUAL(42, *shared_int_accessor);

    shared_int_accessor = std::move(shared_int_accessor);
    BOOST_REQUIRE(shared_int_accessor.isValid());
    BOOST_CHECK_EQUAL(42, *shared_int_accessor);
}


BOOST_AUTO_TEST_CASE(ConstAccessor_release_on_assigment)
{
    SharedResource<int> shared_int_1(42);
    SharedResource<int> shared_int_2(69);

    auto shared_int_1_accessor = shared_int_1.lockConst();
    auto some_shared_int_accessor = shared_int_2.lockConst();

    some_shared_int_accessor = std::move(shared_int_1_accessor);
    auto shared_int_2_accessor = shared_int_2.lockConst();
}


BOOST_AUTO_TEST_CASE(ConstAccessor_construction_from_Accessor)
{
    SharedResource<int> shared_int(42);

    auto shared_int_accessor = shared_int.lock();
    SharedResource<int>::ConstAccessor shared_int_const_accessor(std::move(shared_int_accessor));
    BOOST_REQUIRE(shared_int_const_accessor.isValid());
    BOOST_CHECK_EQUAL(42, *shared_int_const_accessor);
}


BOOST_AUTO_TEST_CASE(Concurrency_condvars)
{
    SharedResource<int> shared_int(0);
    std::condition_variable condvar;

    std::thread test_thread([&shared_int, &condvar]()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        auto shared_int_accessor = shared_int.lock();
        *shared_int_accessor = 7;
        shared_int_accessor.wait(condvar);
        *shared_int_accessor = 3;
    });

    {
        auto shared_int_accessor = shared_int.lock();
        BOOST_CHECK_EQUAL(0, *shared_int_accessor);
        *shared_int_accessor = 5;
        BOOST_CHECK_EQUAL(5, *shared_int_accessor);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        BOOST_CHECK_EQUAL(5, *shared_int_accessor);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    {
        auto shared_int_accessor = shared_int.lock();
        BOOST_CHECK_EQUAL(7, *shared_int_accessor);
        condvar.notify_one();
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        BOOST_CHECK_EQUAL(7, *shared_int_accessor);
    }

    test_thread.join();
    auto shared_int_accessor = shared_int.lock();
    BOOST_CHECK_EQUAL(3, *shared_int_accessor);
}


BOOST_AUTO_TEST_CASE(Accessor_condvars_basic)
{
    SharedResource<int> shared_int(42);
    std::condition_variable condvar;

    auto shared_int_accessor = shared_int.lock();
    auto wait_res = shared_int_accessor.waitFor(condvar, std::chrono::milliseconds(50));
    BOOST_CHECK(std::cv_status::timeout == wait_res);

    wait_res = shared_int_accessor.waitUntil(condvar, std::chrono::steady_clock::now() +
                                                      std::chrono::milliseconds(50));
    BOOST_CHECK(std::cv_status::timeout == wait_res);
}


BOOST_AUTO_TEST_CASE(ConstAccessor_condvars_basic)
{
    SharedResource<int> shared_int(42);
    std::condition_variable condvar;

    auto shared_int_accessor = shared_int.lockConst();
    auto wait_res = shared_int_accessor.waitFor(condvar, std::chrono::milliseconds(50));
    BOOST_CHECK(std::cv_status::timeout == wait_res);

    wait_res = shared_int_accessor.waitUntil(condvar, std::chrono::steady_clock::now() +
                                                      std::chrono::milliseconds(50));
    BOOST_CHECK(std::cv_status::timeout == wait_res);
}


BOOST_AUTO_TEST_CASE(Accessor_condvars_pred_basic)
{
    SharedResource<int> shared_int(42);
    std::condition_variable condvar;

    auto shared_int_accessor = shared_int.lock();
    auto wait_res = shared_int_accessor.waitFor(condvar, std::chrono::milliseconds(50), []{ return false; });
    BOOST_CHECK(!wait_res);
    wait_res = shared_int_accessor.waitFor(condvar, std::chrono::milliseconds(50), []{ return true; });
    BOOST_CHECK(wait_res);

    wait_res = shared_int_accessor.waitUntil(condvar, std::chrono::steady_clock::now() +
                                                      std::chrono::milliseconds(50), [] { return false; });
    BOOST_CHECK(!wait_res);
    wait_res = shared_int_accessor.waitUntil(condvar, std::chrono::steady_clock::now() +
                                                      std::chrono::milliseconds(50), [] { return true; });
    BOOST_CHECK(wait_res);
}


BOOST_AUTO_TEST_CASE(ConstAccessor_condvars_pred_basic)
{
    SharedResource<int> shared_int(42);
    std::condition_variable condvar;

    auto shared_int_accessor = shared_int.lockConst();
    auto wait_res = shared_int_accessor.waitFor(condvar, std::chrono::milliseconds(50), []{ return false; });
    BOOST_CHECK(!wait_res);
    wait_res = shared_int_accessor.waitFor(condvar, std::chrono::milliseconds(50), []{ return true; });
    BOOST_CHECK(wait_res);

    wait_res = shared_int_accessor.waitUntil(condvar, std::chrono::steady_clock::now() +
                                                      std::chrono::milliseconds(50), [] { return false; });
    BOOST_CHECK(!wait_res);
    wait_res = shared_int_accessor.waitUntil(condvar, std::chrono::steady_clock::now() +
                                                      std::chrono::milliseconds(50), [] { return true; });
    BOOST_CHECK(wait_res);
}

