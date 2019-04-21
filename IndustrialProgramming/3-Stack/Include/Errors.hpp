#include <csignal>
#include <iostream>

enum class HardStackError {
    FullStack           = 1,
    EmptyStack          = 2,
    NodeInvalidate      = 3,
    HashInvalidate      = 4,
    CuckooInvalidate    = 5,
    MemoryAllocateFails = 6
};

namespace std {
    template <> struct is_error_code_enum<HardStackError> : std::true_type {};
}

namespace detail {
    class HardStackError_category : public std::error_category {
    public:
        const char *name() const noexcept final {
            return "HardStackError";
        }
        std::string message(int c) const final {
            switch (static_cast<HardStackError>(c)) {
                case HardStackError::FullStack:
                    return "Stack is full!";
                case HardStackError::EmptyStack:
                    return "Pop of empty stack!";
                case HardStackError::NodeInvalidate:
                    return "One node is invalidate!";
                case HardStackError::HashInvalidate:
                    return "Hash is invalidate!";
                case HardStackError::CuckooInvalidate:
                    return "One of the Cuckoo is Invalidate!";
                case HardStackError ::MemoryAllocateFails:
                    return "Memory allocation fails";
                default:
                    return "Unknown error";
            }
        }
    };
}


#define THIS_MODULE_API_DECL extern inline

THIS_MODULE_API_DECL const detail::HardStackError_category &HardStackError_category() {
    static detail::HardStackError_category c;
    return c;
}

inline std::error_code make_error_code(HardStackError e) {
    return {static_cast<int>(e), HardStackError_category()};
}

namespace {
    volatile std::sig_atomic_t gSignalStatus;
}

void signal_handler(int signal) {
    gSignalStatus = signal;
}
