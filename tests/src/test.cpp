#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <pqrs/osx/iokit/ioobject_ptr.hpp>

TEST_CASE("ioobject_ptr") {
  io_iterator_t it1;
  io_iterator_t it2;
  IORegistryCreateIterator(kIOMasterPortDefault,
                           kIOServicePlane,
                           0,
                           &it1);
  IORegistryCreateIterator(kIOMasterPortDefault,
                           kIOServicePlane,
                           0,
                           &it2);

  REQUIRE(IOObjectGetUserRetainCount(it1) == 1);
  REQUIRE(IOObjectGetUserRetainCount(it2) == 1);

  {
    pqrs::osx::iokit::ioobject_ptr ptr1(it1);
    REQUIRE(IOObjectGetUserRetainCount(it1) == 2);
    REQUIRE(ptr1 == true);

    REQUIRE(IOObjectGetUserRetainCount(ptr1.get()) == 2);
    REQUIRE(IOObjectGetUserRetainCount(*ptr1) == 2);

    ptr1.reset();
    REQUIRE(IOObjectGetUserRetainCount(it1) == 1);
    REQUIRE(ptr1 == false);
  }

  REQUIRE(IOObjectGetUserRetainCount(it1) == 1);

  {
    pqrs::osx::iokit::ioobject_ptr ptr1(it1);
    REQUIRE(IOObjectGetUserRetainCount(it1) == 2);

    ptr1 = IO_OBJECT_NULL;
    REQUIRE(IOObjectGetUserRetainCount(it1) == 1);
  }

  {
    pqrs::osx::iokit::ioobject_ptr ptr1(it1);
    REQUIRE(IOObjectGetUserRetainCount(it1) == 2);
    pqrs::osx::iokit::ioobject_ptr ptr2(it2);
    REQUIRE(IOObjectGetUserRetainCount(it2) == 2);

    ptr1 = ptr2;
    REQUIRE(IOObjectGetUserRetainCount(it1) == 1);
    REQUIRE(IOObjectGetUserRetainCount(it2) == 3);

    ptr1 = IO_OBJECT_NULL;
    REQUIRE(IOObjectGetUserRetainCount(it1) == 1);
    REQUIRE(IOObjectGetUserRetainCount(it2) == 2);

    ptr2 = IO_OBJECT_NULL;
    REQUIRE(IOObjectGetUserRetainCount(it1) == 1);
    REQUIRE(IOObjectGetUserRetainCount(it2) == 1);
  }

  {
    pqrs::osx::iokit::ioobject_ptr ptr1(it1);
    REQUIRE(IOObjectGetUserRetainCount(it1) == 2);

    ptr1 = ptr1;
    REQUIRE(IOObjectGetUserRetainCount(it1) == 2);
  }

  {
    pqrs::osx::iokit::ioobject_ptr ptr1(it1);
    REQUIRE(IOObjectGetUserRetainCount(it1) == 2);

    pqrs::osx::iokit::ioobject_ptr ptr1_1(ptr1);
    REQUIRE(IOObjectGetUserRetainCount(it1) == 3);

    ptr1 = IO_OBJECT_NULL;
    REQUIRE(IOObjectGetUserRetainCount(it1) == 2);

    ptr1_1 = IO_OBJECT_NULL;
    REQUIRE(IOObjectGetUserRetainCount(it1) == 1);
  }
}
