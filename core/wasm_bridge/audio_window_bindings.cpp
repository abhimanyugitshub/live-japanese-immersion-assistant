#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <vector>
#include "../audio_window.hpp"

using namespace emscripten;

// JS Float32Array -> C++ window
void pushFromJs(ds::AudioWindow& w, const val& jsArray)
{
    std::vector<float> v = convertJSArrayToNumberVector<float>(jsArray);
    w.push(v.data(), v.size());
}

// C++ window -> a NEW JS Float32Array (a copy, so it stays valid)
val snapshotToJs(const ds::AudioWindow& w)
{
    std::vector<float> v = w.snapshot();
    val view(typed_memory_view(v.size(), v.data()));
    return val::global("Float32Array").new_(view);   // new_ makes a copy
}

EMSCRIPTEN_BINDINGS(audio_window_module)
{
    class_<ds::AudioWindow>("AudioWindow")
        .constructor<size_t>()
        .function("push", &pushFromJs)
        .function("snapshot", &snapshotToJs)
        .function("size", &ds::AudioWindow::size)
        .function("capacity", &ds::AudioWindow::capacity)
        .function("totalPushed", &ds::AudioWindow::totalPushed);
}