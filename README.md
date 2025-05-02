# WTF newlib vs newlib

## On a host with gcc arm toolchain installed...
The commandline is from godbolt, but it serves to compile and test the file, without worrying about any linking...
```
arm-none-eabi-gcc -v -g -o output.s -fno-verbose-asm -S -fdiagnostics-color=always -Wall -Wextra -Os hate.c
```

It should compile just fine.  inttypes is meant to work...

## Now, with ubuntu's newlib package...

```
podman build -t ubu-arm:24.04 --build-arg VERSION_UBUNTU=24.04 -f Dockerfile
podman run --rm -v .:/project:Z -w /project localhost/ubu-arm:24.04 arm-none-eabi-gcc -g -o output.s -fno-verbose-asm -S -fdiagnostics-color=always -Wall -Wextra -Os hate.c
```

Witness it fails to know about the 64bit types, but it even tries to tell you to include inttypes.h!

```
hate.c: In function 'lol':
hate.c:7:20: error: expected ')' before 'PRIu64'
    7 |     printf("lol: %" PRIu64 "\n", input);
      |           ~        ^~~~~~~
      |                    )
hate.c:4:1: note: 'PRIu64' is defined in header '<inttypes.h>'; did you forget to '#include <inttypes.h>'?
    3 | #include <inttypes.h>
  +++ |+#include <inttypes.h>
    4 | 
hate.c:7:18: warning: spurious trailing '%' in format [-Wformat=]
    7 |     printf("lol: %" PRIu64 "\n", input);
      |                  ^
hate.c:8:20: error: expected ')' before 'PRIX64'
    8 |     printf("lol: %" PRIX64 "\n", input);
      |           ~        ^~~~~~~
      |                    )
hate.c:8:21: note: 'PRIX64' is defined in header '<inttypes.h>'; did you forget to '#include <inttypes.h>'?
    8 |     printf("lol: %" PRIX64 "\n", input);
      |                     ^~~~~~
hate.c:8:18: warning: spurious trailing '%' in format [-Wformat=]
    8 |     printf("lol: %" PRIX64 "\n", input);
      |                  ^
hate.c:5:19: warning: unused parameter 'input' [-Wunused-parameter]
    5 | void lol(uint64_t input)
      |          ~~~~~~~~~^~~~~
```

wth?!

## Now what?!
I don't know.  the inttypes.h files in the container and in my arm gcc tree are identical.
I've found some comment that this is related to gcc's stdint not doing things in the right order, and that the "fix"
is to include ```<sys/types.h>``` first, but.... wth. I mean, it works, but... that can't be right...


## I hate your "build" command
Want to see it with a "main" instead?

```
arm-none-eabi-gcc --specs=nosys.specs -Wall -Wextra -Os hate-main.c
```
vs 
```
podman run --rm -v .:/project:Z -w /project localhost/ubu-arm:24.04 arm-none-eabi-gcc --specs=nosys.specs -Wall -Wextra -Os hate-main.c
```

(it's the same thing)

### Distro check...
Debian 12.10: works fine (uses old newlib 3.3, yay, debian stability!)
Ubuntu 24.04 LTS: fails
Debian testing-20250428: fails (newlib 4.5.0)
