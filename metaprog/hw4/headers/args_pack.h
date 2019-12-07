template <class ...T>
class ArgsPack;

template <>
class ArgsPack<> {
public:
    ArgsPack() = default;

    template <class Function, class...Args>
    auto call(Function function, Args... args) -> decltype(function(args...)) {
        return function(args...);
    }
};

template<class Head, class... Tail>
class ArgsPack<Head, Tail...> {

typedef ArgsPack<Tail...> Base;

private:
    Head param_;
    Base base_;

    Head* _toPointer(Head &param) {
        return &param_;
    }

    Head* _toPointer(Head* param) {
        return param_;
    }

    Head& _toReference(Head &param) {
        return param_;
    }

    Head& _toReference(Head* param) {
        return *param_;
    }

public:
    explicit ArgsPack(Head param, Tail... params) : base_(params...), param_(param) {}

    template <class Function, class...Args>
    auto call(const Function &function, Args... args) ->
        decltype(base_.call(function, args..., _toPointer(this->param_))) {
            return base_.call(function, args..., _toPointer(param_));
    }

    template <class Function, class...Args>
    auto call(const Function &function, Args... args) ->
        decltype(base_.call(function, args..., _toReference(this->param_))) {
            return base_.call(function, args..., _toReference(param_));
    }
};