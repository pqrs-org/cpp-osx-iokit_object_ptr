#include <iostream>
#include <pqrs/osx/iokit/ioobject_ptr.hpp>

int main(void) {
  {
    io_iterator_t it;
    IORegistryCreateIterator(kIOMasterPortDefault,
                             kIOServicePlane,
                             0,
                             &it);

    std::cout << IOObjectGetUserRetainCount(it) << std::endl; // CFGetRetainCount(*p) == 1

    {
      pqrs::osx::iokit::ioobject_ptr p(it);
      std::cout << IOObjectGetUserRetainCount(it) << std::endl; // CFGetRetainCount(*p) == 2
    }

    std::cout << IOObjectGetUserRetainCount(it) << std::endl; // CFGetRetainCount(*p) == 1

    IOObjectRelease(it);

    std::cout << IOObjectGetUserRetainCount(it) << std::endl; // CFGetRetainCount(*p) == 0
  }

  return 0;
}
