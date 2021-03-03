#ifndef	SIGNAL_OBSERVER_H
#define	SIGNAL_OBSERVER_H

#include <vector>
#include <tuple>
#include <iostream>
#include <algorithm>

template <typename T>
class SignalObserver
{
public:
    typedef std::tuple<uint64_t, T> change_tuple;
    typedef std::vector<change_tuple> change_vector;

    SignalObserver(T initialValue = 0) : _value(initialValue) { }
    virtual ~SignalObserver() { }

    void updateSignal(uint64_t time, T newValue)
    {
        if (_value != newValue) {
            _changes.push_back({time, newValue});
            _value = newValue;
        }
    }

    const change_vector& changes()
    {
        return _changes;
    }

    change_vector changes(size_t limit)
    {
        auto first = _changes.begin();
        auto last = _changes.begin() + std::min(limit, _changes.size());
        change_vector changes(first, last);
        return changes;
    }

private:
    T _value;
    change_vector _changes;
};

typedef SignalObserver<uint8_t> SignalObserver8;
typedef SignalObserver8::change_vector change_vector_8;

std::ostream& operator << ( std::ostream& os, SignalObserver8::change_tuple const& value );

#endif // SIGNAL_OBSERVER_H