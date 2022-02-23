# uSTL

**This library is obsolete. Please use the official gcc C++ standard library.**

uSTL is a partial implementation of the C++ standard library that focuses on
decreasing the memory footprint of user executables. While some projects can
use it to replace the standard library without any code changes, there exist
important differences that you should be aware of. Please read documentation
in [docs/index.html](https://msharov.github.io/ustl) before using this
library in your project.

Then, after you've read it, consider whether you really want to make your
project depend on a nonstandard standard library to get almost nothing for
the trouble. uSTL is no longer significantly smaller than the standard
library, but continues to have numerous incompatibilities that were required
to get those space savings twenty years ago. So when I ask you to just use
the standard library, I really do mean it!

If you are using uSTL for its non-standard features or because you find it
better fits your way of thinking, consider converting to
[cwiclo](https://github.com/msharov/cwiclo), which is what I am currently
developing for this purpose. It has no documentation at present, but if you
like uSTL, cwiclo lets you do things more or less the same way. And I use
it myself, so it will actually continue being supported.

## Installation

The only dependency is a C++ compiler, gcc 5 or clang 3.6.
Build configuration is in Config.mk and config.h, generated by configure.
`configure --help` will list available configuration options.

```sh
./configure --prefix=/usr
make
make check
make install
```

## Use

Here's a simple hello world application:
```c++
#include <ustl.h>
using namespace ustl;

int main (void)
{
    cout << "Hello world!\n";
    return EXIT_SUCCESS;
}
```
Compile the sources with g++, but link the executable with *gcc*.
g++ will link with `-lstdc++` by default, but to use uSTL you need to
link with `-lustl -lsupc++` instead. The right way to get this list of
libraries is to use `pkg-config --libs ustl`. The pkg-config description
file for uSTL is installed if you have pkg-config on your system.

## Bugs

Report bugs on the [github issue tracker](https://github.com/msharov/ustl/issues)
