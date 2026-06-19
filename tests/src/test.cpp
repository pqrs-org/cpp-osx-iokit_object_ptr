#include <boost/ut.hpp>
#include <pqrs/osx/iokit_object_ptr.hpp>
#include <pqrs/osx/iokit_types.hpp>
#include <utility>

namespace {
pqrs::osx::iokit_object_ptr copy_constructor_test() {
  pqrs::osx::iokit_object_ptr result;
  return result;
}
} // namespace

int main() {
  using namespace boost::ut;
  using namespace boost::ut::literals;

  "iokit_object_ptr"_test = [] {
    io_iterator_t it1 = IO_OBJECT_NULL;
    io_iterator_t it2 = IO_OBJECT_NULL;
    expect(fatal(IORegistryCreateIterator(type_safe::get(pqrs::osx::iokit_mach_port::null),
                                          kIOServicePlane,
                                          0,
                                          &it1) == KERN_SUCCESS));
    expect(fatal(IORegistryCreateIterator(type_safe::get(pqrs::osx::iokit_mach_port::null),
                                          kIOServicePlane,
                                          0,
                                          &it2) == KERN_SUCCESS));

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

    {
      pqrs::osx::iokit_object_ptr ptr1(it1);
      expect(IOObjectGetUserRetainCount(it1) == 2);

      pqrs::osx::iokit_object_ptr ptr2(std::move(ptr1));
      expect(ptr1 == false);
      expect(ptr2 == true);
      expect(IOObjectGetUserRetainCount(it1) == 2);

      pqrs::osx::iokit_object_ptr ptr3(it2);
      expect(IOObjectGetUserRetainCount(it2) == 2);

      ptr3 = std::move(ptr2);
      expect(ptr2 == false);
      expect(ptr3 == true);
      expect(IOObjectGetUserRetainCount(it1) == 2);
      expect(IOObjectGetUserRetainCount(it2) == 1);

      ptr3.reset();
      expect(IOObjectGetUserRetainCount(it1) == 1);
    }

    IOObjectRelease(it1);
    IOObjectRelease(it2);
  };

  "adopt_iokit_object_ptr"_test = [] {
    io_iterator_t it = IO_OBJECT_NULL;
    expect(fatal(IORegistryCreateIterator(type_safe::get(pqrs::osx::iokit_mach_port::null),
                                          kIOServicePlane,
                                          0,
                                          &it) == KERN_SUCCESS));

    {
      auto ptr = pqrs::osx::adopt_iokit_object_ptr(it);
      expect(IOObjectGetUserRetainCount(it) == 1);
      expect(ptr == true);

      {
        pqrs::osx::iokit_object_ptr copied(ptr);
        expect(IOObjectGetUserRetainCount(it) == 2);
      }

      expect(IOObjectGetUserRetainCount(it) == 1);
    }
  };

  "iokit_object_ptr::conforms_to, class_name"_test = [] {
    {
      pqrs::osx::iokit_object_ptr ptr;
      expect(ptr.conforms_to("IOUserIterator") == false);
      expect(ptr.class_name() == std::nullopt);
    }

    {
      io_iterator_t it = IO_OBJECT_NULL;
      expect(fatal(IORegistryCreateIterator(type_safe::get(pqrs::osx::iokit_mach_port::null),
                                            kIOServicePlane,
                                            0,
                                            &it) == KERN_SUCCESS));
      pqrs::osx::iokit_object_ptr ptr(it);
      expect(ptr.conforms_to("IOUserIterator") == true);
      expect(ptr.conforms_to("IORegistryEntry") == false);
      expect(ptr.class_name() == "IOUserIterator");

      ptr.reset();
      expect(ptr.conforms_to("IOUserIterator") == false);
      expect(ptr.class_name() == std::nullopt);

      IOObjectRelease(it);
    }
  };

  "iokit_object_ptr::copy_constructor_test"_test = [] {
    copy_constructor_test();
  };

  return 0;
}
