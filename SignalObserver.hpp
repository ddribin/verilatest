#ifndef	VERILATEST_SIGNAL_OBSERVER_H
#define	VERILATEST_SIGNAL_OBSERVER_H

#include <vector>
#include <tuple>
#include <iostream>
#include <algorithm>

#include "Signal.hpp"

template <typename T, class Core>
class SignalObserver : public SignalOutput<Core>
{
public:
    typedef std::tuple<uint64_t, T> ChangeTuple;
    typedef std::vector<ChangeTuple> ChangeVector;

    SignalObserver(T Core:: *signal, T initialValue = 0) : 
        _signal(signal),
        _initialValue(initialValue),
        _value(initialValue) { }
    SignalObserver(T Core:: *signal, Component<Core>& component, T initialValue = 0) :
        _signal(signal),
        _initialValue(initialValue)
    {
        component.addOutput(*this);
    }
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

    std::vector<T> timeline(uint64_t endTime) const
    {
        uint64_t time = 0;
        T value = _initialValue;
        std::vector<T> timeline;
        // timeline.push_back(value);

        for (auto change : _changes) {
            uint64_t changeTime = std::get<0>(change);
            T changeValue = std::get<1>(change);
            while (time < changeTime) {
                timeline.push_back(value);
                time++;
            }
            // time == changeTime
            value = changeValue;
            timeline.push_back(value);
            time++;
        }

        while (time <= endTime) {
            timeline.push_back(value);
            time++;
        }
        return timeline;
    }

    ChangeVector changes(size_t limit)
    {
        auto first = _changes.begin();
        auto last = _changes.begin() + std::min(limit, _changes.size());
        ChangeVector changes(first, last);
        return changes;
    }

    void setInitialValue(Core &core)
    {
        _value = core.*_signal;
    }

    std::function<void (uint64_t, Core& core)> outputHook()
    {
        auto hook = [=](uint64_t tickCount, Core& core) {
            this->updateSignal(tickCount, core.*_signal);
        };
        return hook;
    }

private:
    T _initialValue;
    T _value;
    T Core::* _signal;
    ChangeVector _changes;
};


template <typename T, class Core>
SignalObserver<T, Core> makeObserver(T Core:: *signal)
{
    SignalObserver<T, Core> observer(signal);
    return observer;
}

template<typename T>
using SignalEvent = std::tuple<uint64_t,T>;
template<typename T>
using ChangeVector = std::vector<SignalEvent<T>>;

using SignalEvent8 = SignalEvent<uint8_t>;
using ChangeVector8 = std::vector<SignalEvent8>;
using Vector8 = std::vector<uint8_t>;

using SignalEvent16 = SignalEvent<uint16_t>;
using ChangeVector16 = std::vector<SignalEvent16>;
using Vector16 = std::vector<uint16_t>;

using SignalEvent32 = SignalEvent<uint32_t>;
using ChangeVector32 = std::vector<SignalEvent32>;
using Vector32 = std::vector<uint32_t>;

std::ostream& operator << ( std::ostream& os, std::tuple<uint64_t, uint8_t> const& value );
std::ostream& operator << ( std::ostream& os, std::tuple<uint64_t, uint16_t> const& value );
std::ostream& operator << ( std::ostream& os, std::tuple<uint64_t, uint32_t> const& value );

#endif // VERILATEST_SIGNAL_OBSERVER_H
