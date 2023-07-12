# sfalib
A header library for symbolic automata written in C++

# Setup
Since sfalib is a header-only library, you can download it as is and include it in your project. You can then compile your project and everything should work out of the box.

# Testing

To run the tests, use the CMake file:

```
mkdir build && cd build
cmake ..
make && ./tests/tests
```

To run the tests with coverage just run:

```
make coverage
```

This should produce an html report under: `coverage_report/index.html`
