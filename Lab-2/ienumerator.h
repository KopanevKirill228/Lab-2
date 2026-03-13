#ifndef IENUMERATOR_H
#define IENUMERATOR_H

template <class T>
class IEnumerator {
public:
    virtual ~IEnumerator() = default;
    virtual bool move_next() = 0;
    virtual const T& get_current() const = 0;
    virtual void reset() = 0;
};

#endif
