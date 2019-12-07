#include "args_pack.h"

template <class Function>
class FunctionWrapper {

private:
    Function function_;

public:
    explicit FunctionWrapper(const Function function) : function_(function) {}

    template <class... Args>
    auto operator()(Args... args) -> decltype(function_(args...)) {
        return function_(args...);
    }
};

template <class Class, class Callable>
class ObjectWrapper {

private:
    Class& object_;
    Callable callable_;

public:
    ObjectWrapper(Class& object, const Callable& callable) : object_(object), callable_(callable) {}

    template <class... Args>
    auto operator()(Args... args) -> decltype((object_.*callable_)(args...)) {
        return (object_.*callable_)(args...);
    }
};

template <class Function, class... Args>
class Functor {

private:
    typedef ArgsPack<Args...> base;
    Function function_;
    base base_;

public:
    explicit Functor(Function function, Args...args) : base_(args...), function_(function) {}

    template <class Object>
    auto call(Object& object) -> decltype(base_.call(ObjectWrapper<Object, Function>(object, function_))) {
        return base_.call(ObjectWrapper<Object, Function>(object, function_));
    }

    template <class Callable = Function> auto  call() -> decltype(base_.call(FunctionWrapper<Callable>(function_))) {
        return base_.call(FunctionWrapper<Callable>(function_));
    }
};

template <class Function, class... Args>
Functor<Function, Args...> make_functor(Function function, Args...args) {
    return Functor<Function, Args...>(function, args...);
}