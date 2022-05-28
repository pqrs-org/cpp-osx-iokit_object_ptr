#include <boost/ut.hpp>
#include <pqrs/osx/iokit_object_ptr.hpp>
#include <pqrs/osx/iokit_types.hpp>

namespace {
pqrs::osx::iokit_object_ptr copy_constructor_test(void) {
  pqrs::osx::iokit_object_ptr result;
  return result;
}
} // namespace

int main(void) {
  using namespace boost::ut;
  using namespace boost::ut::literals;

  "iokit_object_ptr"_test = [] {
    io_iterator_t it1;
    io_iterator_t it2;
    IORegistryCreateIterator(type_safe::get(pqrs::osx::iokit_mach_port::null),
                             kIOServicePlane,
                             0,
                             &it1);
    IORegistryCreateIterator(type_safe::get(pqrs::osx::iokit_mach_port::null),
                             kIOServicePlane,
                             0,
                             &it2);

    expect(IOObjectGetUserRetainCount(it1) == 1);
    expect(IOObjectGetUserRetainCount(it2) == 1);

    {
      pqrs::osx::iokit_object_ptr ptr1(it1);
      expect(IOObjectGetUserRetainCount(it1) == 2);
      expect(ptr1 == true);

      expect(IOObjectGetUserRetainCount(ptr1.get()) == 2);
      expect(IOObjectGetUserRetainCount(*ptr1) == 2);

      ptr1.reset();
      expect(IOObjectGetUserRetainCount(it1) == 1);
      expect(ptr1 == false);
    }

    expect(IOObjectGetUserRetainCount(it1) == 1);

    {
      pqrs::osx::iokit_object_ptr ptr1(it1);
      expect(IOObjectGetUserRetainCount(it1) == 2);

      ptr1 = pqrs::osx::iokit_object_ptr();
      expect(IOObjectGetUserRetainCount(it1) == 1);
    }

    {
      pqrs::osx::iokit_object_ptr ptr1(it1);
      expect(IOObjectGetUserRetainCount(it1) == 2);
      pqrs::osx::iokit_object_ptr ptr2(it2);
      expect(IOObjectGetUserRetainCount(it2) == 2);

      ptr1 = ptr2;
      expect(IOObjectGetUserRetainCount(it1) == 1);
      expect(IOObjectGetUserRetainCount(it2) == 3);

      ptr1 = pqrs::osx::iokit_object_ptr();
      expect(IOObjectGetUserRetainCount(it1) == 1);
      expect(IOObjectGetUserRetainCount(it2) == 2);

      ptr2 = pqrs::osx::iokit_object_ptr();
      expect(IOObjectGetUserRetainCount(it1) == 1);
      expect(IOObjectGetUserRetainCount(it2) == 1);
    }

    {
      pqrs::osx::iokit_object_ptr ptr1(it1);
      expect(IOObjectGetUserRetainCount(it1) == 2);

      pqrs::osx::iokit_object_ptr ptr1_1(ptr1);
      expect(IOObjectGetUserRetainCount(it1) == 3);

      ptr1 = pqrs::osx::iokit_object_ptr();
      expect(IOObjectGetUserRetainCount(it1) == 2);

      ptr1_1 = pqrs::osx::iokit_object_ptr();
      expect(IOObjectGetUserRetainCount(it1) == 1);
    }
  };

  "iokit_object_ptr::conforms_to, class_name"_test = [] {
    {
      pqrs::osx::iokit_object_ptr ptr;
      expect(ptr.conforms_to("IOUserIterator") == false);
      expect(ptr.class_name() == std::nullopt);
    }

    {
      io_iterator_t it;
      IORegistryCreateIterator(type_safe::get(pqrs::osx::iokit_mach_port::null),
                               kIOServicePlane,
                               0,
                               &it);
      pqrs::osx::iokit_object_ptr ptr(it);
      expect(ptr.conforms_to("IOUserIterator") == true);
      expect(ptr.conforms_to("IORegistryEntry") == false);
      expect(ptr.class_name() == "IOUserIterator");

      ptr.reset();
      expect(ptr.conforms_to("IOUserIterator") == false);
      expect(ptr.class_name() == std::nullopt);
    }
  };

  "iokit_object_ptr::copy_constructor_test"_test = [] {
    copy_constructor_test();
  };

  return 0;
}
