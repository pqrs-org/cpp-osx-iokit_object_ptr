#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <pqrs/osx/iokit_object_ptr.hpp>

TEST_CASE("iokit_object_ptr") {
  io_iterator_t it1;
  io_iterator_t it2;
  IORegistryCreateIterator(kIOMainPortDefault,
                           kIOServicePlane,
                           0,
                           &it1);
  IORegistryCreateIterator(kIOMainPortDefault,
                           kIOServicePlane,
                           0,
                           &it2);

  REQUIRE(IOObjectGetUserRetainCount(it1) == 1);
  REQUIRE(IOObjectGetUserRetainCount(it2) == 1);

  {
    pqrs::osx::iokit_object_ptr ptr1(it1);
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
    pqrs::osx::iokit_object_ptr ptr1(it1);
    REQUIRE(IOObjectGetUserRetainCount(it1) == 2);

    ptr1 = pqrs::osx::iokit_object_ptr();
    REQUIRE(IOObjectGetUserRetainCount(it1) == 1);
  }

  {
    pqrs::osx::iokit_object_ptr ptr1(it1);
    REQUIRE(IOObjectGetUserRetainCount(it1) == 2);
    pqrs::osx::iokit_object_ptr ptr2(it2);
    REQUIRE(IOObjectGetUserRetainCount(it2) == 2);

    ptr1 = ptr2;
    REQUIRE(IOObjectGetUserRetainCount(it1) == 1);
    REQUIRE(IOObjectGetUserRetainCount(it2) == 3);

    ptr1 = pqrs::osx::iokit_object_ptr();
    REQUIRE(IOObjectGetUserRetainCount(it1) == 1);
    REQUIRE(IOObjectGetUserRetainCount(it2) == 2);

    ptr2 = pqrs::osx::iokit_object_ptr();
    REQUIRE(IOObjectGetUserRetainCount(it1) == 1);
    REQUIRE(IOObjectGetUserRetainCount(it2) == 1);
  }

  {
    pqrs::osx::iokit_object_ptr ptr1(it1);
    REQUIRE(IOObjectGetUserRetainCount(it1) == 2);

    pqrs::osx::iokit_object_ptr ptr1_1(ptr1);
    REQUIRE(IOObjectGetUserRetainCount(it1) == 3);

    ptr1 = pqrs::osx::iokit_object_ptr();
    REQUIRE(IOObjectGetUserRetainCount(it1) == 2);

    ptr1_1 = pqrs::osx::iokit_object_ptr();
    REQUIRE(IOObjectGetUserRetainCount(it1) == 1);
  }
}

TEST_CASE("iokit_object_ptr::conforms_to, class_name") {
  {
    pqrs::osx::iokit_object_ptr ptr;
    REQUIRE(ptr.conforms_to("IOUserIterator") == false);
    REQUIRE(ptr.class_name() == std::nullopt);
  }

  {
    io_iterator_t it;
    IORegistryCreateIterator(kIOMainPortDefault,
                             kIOServicePlane,
                             0,
                             &it);
    pqrs::osx::iokit_object_ptr ptr(it);
    REQUIRE(ptr.conforms_to("IOUserIterator") == true);
    REQUIRE(ptr.conforms_to("IORegistryEntry") == false);
    REQUIRE(ptr.class_name() == "IOUserIterator");

    ptr.reset();
    REQUIRE(ptr.conforms_to("IOUserIterator") == false);
    REQUIRE(ptr.class_name() == std::nullopt);
  }
}

namespace {
pqrs::osx::iokit_object_ptr copy_constructor_test(void) {
  pqrs::osx::iokit_object_ptr result;
  return result;
}
} // namespace

TEST_CASE("iokit_object_ptr::copy_constructor_test") {
  copy_constructor_test();
}
