#ifndef TYPELIST_H
#define TYPELIST_H

static int error_code = -1;

struct NullTypeList {};

template <typename H, typename T>
struct typelist {
    typedef H head;
    typedef T tail;
};


template <typename Type, typename Head, typename List>
struct _index {
    int get() {
        int result = _index<Type, typename List::head, typename List::tail> ().get();
        if (result == error_code) {
            return error_code;
        }

        return result + 1;
    }
};

// Here we proceed last element
template <typename Type, typename Head>
struct _index<Type, Head, NullTypeList> {
    int get() {
        return error_code;
    }
};

// Here Type = Head
template <class Type>
struct _index<Type, Type, NullTypeList> {
    int get() {
        return 0;
    }
};

template <typename Type, typename List>
int getIndexOf() {
    return _index<Type, typename List::head, typename List::tail> ().get();
};


#endif
