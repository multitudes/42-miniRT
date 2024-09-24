
# Extras
## Create Random function in C

Since we are not allowed to use the rand() function in the mandatory part, we have to create our own random function.
One of the simplest and most commonly used algorithms is the "Linear Congruential Generator (LCG).

1. Define constants `a`, `c`, and `m`.
2. Define a variable `seed` to store the current state.
3. Implement a function to set the seed.
4. Implement a function to generate the next random number using the formula: `seed = (a * seed + c) % m`.

Like:

```c
#include <stdio.h>
#include <time.h>

// Define constants for the LCG
#define A 1664525
#define C 1013904223
#define M 4294967296 // 2^32

// Function to generate the next random number
unsigned int rand_custom() {
	static __thread unsigned int seed = 1;
    seed = (A * seed + C) % M;
    return (seed);
}
```

## the __thread attribute
This makes the variable thread safe. 

> The __thread attribute in C is used to declare thread-local storage (TLS) variables. TLS variables are variables that are specific to each thread and are not shared between threads. This ensures that each thread has its own copy of the variable, preventing race conditions and data corruption.

## comparing the results

A good random function does make a difference...



## OPTIMISATION in the Makefile for production

The CFLAGS:
```makefile
#standard flags
CFLAGS 			= 	-Wextra -Wall -Werror
# for production
CFLAGS			+=	-03 -Ofast -march=native -funroll-loops -Wunreachable-code -finline-functions -fno-rtti -fno-exceptions -fno-stack-protector
```

- O3: Enables aggressive optimization level 3, which applies a wide range of optimizations.
- Ofast: Enables even more aggressive optimizations, including some that might violate strict C standards. Use with caution, as it can potentially introduce unexpected behavior.
- march=native: Targets the specific CPU architecture of your machine for optimal performance.
- funroll-loops: Unrolls loops, which can improve performance by reducing loop overhead.
- Wunreachable-code: Warns about unreachable code, which can help identify potential errors or inefficiencies.
- finline-functions: Inlines small functions, which can reduce function call overhead.
- fno-rtti: Disables runtime type information (RTTI), which can reduce code size and improve performance.
- fno-exceptions: Disables exception handling, which can also reduce code size and improve performance.
- fno-stack-protector: Disables stack protection, which can slightly improve performance but also reduces security.

The LDFLAGS:
```makefile
# for production
LDFLAGS			= 	-Wl,--gc-sections -Wl,-O2 -Wl,--as-needed -Wl,--strip-all
```
- Wl,--gc-sections: Removes unused sections from the executable, reducing its size.
- Wl,-O2: Enables optimization level 2 for the linker, which can improve linking performance.
- Wl,--as-needed: Only includes libraries and symbols that are actually used by the executable, reducing its size and potential dependencies.
- Wl,--strip-all: Removes all debug symbols from the executable, reducing its size and improving performance for production builds.