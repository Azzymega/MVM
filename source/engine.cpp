#include "engine.hpp"
#include "memory.hpp"

Engine *Engine::Resolve(Class *Object) {
    for (auto &&methods : Object->methods.methods) {
        if (methods.name == "main") {
            Frame initFrame;
            initFrame.ClassPoolReference = Object;
            initFrame.MethodReference  = &methods;
            initFrame.ReturnValue = nullptr;
            initFrame.CodeReference = &methods.attributes.attributes[0].code;
        }
    }
    return nullptr;
}

Engine *Engine::Resolve(Frame *Object) {
    // interpreter here и доработать идею о нативных вызовах библиотек! стандартную функцию загрузки засунуть в стороннюю либу
    return nullptr;
}


