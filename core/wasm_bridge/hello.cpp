#include <emscripten/bind.h>
#include <string>

int addNumbers(int a, int b) {
    return a + b;
}

std::string greet(std::string name) {
    return "Hello, " + name + "!";
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("addNumbers", &addNumbers);
    emscripten::function("greet", &greet);
}
