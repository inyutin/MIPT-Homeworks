struct NullTypeList {};

template <typename H, typename T>
struct Typelist {
    typedef H Head;
    typedef T Tail;
};

template <int i, class TList> struct TypeAt;

template <class H, class T>
struct TypeAt<0, Typelist<H, T>> {
    typedef H Result;
};

template <int i, class H, class T>
struct TypeAt<i, Typelist<H, T>> {
    typedef typename TypeAt<i - 1, T>::Result Result;
};