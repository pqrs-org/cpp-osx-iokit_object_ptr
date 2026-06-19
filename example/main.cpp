#include <iostream>
#include <pqrs/osx/iokit_object_ptr.hpp>
#include <pqrs/osx/iokit_types.hpp>

int main() {
  {
    io_iterator_t it = IO_OBJECT_NULL;
    IORegistryCreateIterator(type_safe::get(pqrs::osx::iokit_mach_port::null),
                             kIOServicePlane,
                             0,
                             &it);

    auto p = pqrs::osx::adopt_iokit_object_ptr(it);
    std::cout << "user_retain_count: " << p.user_retain_count() << std::endl; // CFGetRetainCount(*p) == 1
    std::cout << "kernel_retain_count: " << p.kernel_retain_count() << std::endl;
    std::cout << "conforms_to: " << p.conforms_to("IOUserIterator") << std::endl;       // true
    std::cout << "conforms_to: " << p.conforms_to("RootDomainUserClient") << std::endl; // false
    if (auto name = p.class_name()) {
      std::cout << "class_name: " << *name << std::endl;
    }
  }

  return 0;
}
