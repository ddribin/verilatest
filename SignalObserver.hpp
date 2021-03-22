#ifndef	SIGNAL_OBSERVER_H
#define	SIGNAL_OBSERVER_H

#include <vector>
#include <tuple>
#include <iostream>
#include <algorithm>

template <typename T, class Core>
class SignalObserver
{
public:
    typedef std::tuple<uint64_t, T> ChangeTuple;
    typedef std::vector<ChangeTuple> ChangeVector;

    SignalObserver(T Core:: *signal, T initialValue = 0) : 
        _signal(signal),
        _value(initialValue) { }
    virtual ~SignalObserver() { }

    void updateSignal(uint64_t time, T newValue)
    {
        if (_value != newValue) {
            _changes.push_back({time, newValue});
            _value = newValue;
        }
    }

    const ChangeVector& changes() const
    {
        return _changes;
    }

    ChangeVector changes(size_t limit)
    {
        auto first = _changes.begin();
        auto last = _changes.begin() + std::min(limit, _changes.size());
        ChangeVector changes(first, last);
        return changes;
    }

    std::function<void (uint64_t, Core * core)> hook(T Core::*signal) {
        auto hook = [=](uint64_t tickCount, Core *core) {
            this->updateSignal(tickCount, core->*signal);
        };
        return hook;
    }

    std::function<void (uint64_t, Core * core)> hook() {
        auto hook = [=](uint64_t tickCount, Core *core) {
            this->updateSignal(tickCount, core->*_signal);
        };
        return hook;
    }

private:
    T _value;
    T Core::* _signal;
    ChangeVector _changes;
};


template <typename T, class Core>
SignalObserver<T, Core> makeObserver(T Core:: *signal) {
    SignalObserver<T, Core> observer(signal);
    return observer;
}

template<typename T>
using SignalEvent = std::tuple<uint64_t,T>;
template<typename T>
using ChangeVector = std::vector<SignalEvent<T>>;

using SignalEvent8 = SignalEvent<uint8_t>;
using ChangeVector8 = std::vector<SignalEvent8>;

std::ostream& operator << ( std::ostream& os, std::tuple<uint64_t, uint8_t> const& value );

#endif // SIGNAL_OBSERVER_H