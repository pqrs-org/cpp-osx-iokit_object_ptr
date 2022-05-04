#include <iostream>
#include <pqrs/osx/iokit_object_ptr.hpp>

int main(void) {
  {
    io_iterator_t it;
    IORegistryCreateIterator(kIOMainPortDefault,
                             kIOServicePlane,
                             0,
                             &it);

    std::cout << IOObjectGetUserRetainCount(it) << std::endl; // CFGetRetainCount(*p) == 1

    {
      pqrs::osx::iokit_object_ptr p(it);
      std::cout << "user_retain_count: " << p.user_retain_count() << std::endl; // CFGetRetainCount(*p) == 2
      std::cout << "kernel_retain_count: " << p.kernel_retain_count() << std::endl;
      std::cout << "conforms_to: " << p.conforms_to("IOUserIterator") << std::endl;       // true
      std::cout << "conforms_to: " << p.conforms_to("RootDomainUserClient") << std::endl; // false
      if (auto name = p.class_name()) {
        std::cout << "class_name: " << *name << std::endl;
      }
    }

    std::cout << IOObjectGetUserRetainCount(it) << std::endl; // CFGetRetainCount(*p) == 1

    IOObjectRelease(it);

    std::cout << IOObjectGetUserRetainCount(it) << std::endl; // CFGetRetainCount(*p) == 0
  }

  return 0;
}
