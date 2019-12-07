#include "Typelist.h"

template <class T>
struct Unit {
    T value;
};

template <class TList>
class GenScatterHierarchy;

template <class T1, class T2>
class GenScatterHierarchy<Typelist<T1, T2> > :
    public GenScatterHierarchy<T1>,
    public GenScatterHierarchy<T2> {
public:
    using TList = Typelist<T1, T2>;
    using Left = GenScatterHierarchy<T1>;
    using Right = GenScatterHierarchy<T2>;
};

template <class AtomicType>
class GenScatterHierarchy :
    public Unit<AtomicType> {
public:
    using Left = AtomicType;
};

template <int i, class H> struct Helper;

template <class H>
struct Helper<0, H> {
    typedef typename H::TList::Head ElementType;
    typedef typename H::Left LeftBase;

    static ElementType& Do(H& obj) {
        LeftBase& leftBase = obj;
        return leftBase.value;
    }
};

template <int i, class H>
struct Helper {
    typedef typename TypeAt<i, typename H::TList>::Result  ElementType;
    typedef typename H::Right RightBase;

    static ElementType& Do(H& obj) {
        RightBase& rightBase = obj;
        return Helper<i - 1, RightBase >::Do(rightBase);
    }
};

template <int i, class H>
typename Helper<i, H>::ElementType& Field(H& obj) {
    return Helper<i, H>::Do(obj);
}
