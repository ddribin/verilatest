#ifndef SIGNAL_PUBLISHER_H
#define SIGNAL_PUBLISHER_H

#include <vector>
#include <tuple>
#include <queue>
#include <iostream>
#include <algorithm>
#include <functional>

template <typename T, class Core>
class SignalPublisher
{
public:
    typedef std::tuple<uint64_t, T> ChangeTuple;
    typedef std::vector<ChangeTuple> ChangeVector;

    SignalPublisher(T Core::* signal) : _signal(signal) { }
    virtual ~SignalPublisher() { }

    void addInput(const ChangeTuple &input) {
        _changes.push(input);
    }

    void addInputs(const ChangeVector &inputs) {
        for (auto i : inputs) {
            _changes.push(i);
        }
    }

    std::function<void (uint64_t, Core& core)> hook() {
        auto hook = [=](uint64_t tickCount, Core& core) {
            this->updateSignal(tickCount, core);
        };
        return hook;
    }

private:
    T Core::* _signal;

    struct EventCompare {
        bool operator() (ChangeTuple left, ChangeTuple right) {
            return std::get<0>(left) > std::get<0>(right);
         }
    };
    std::priority_queue<ChangeTuple, ChangeVector, EventCompare> _changes;

    void updateSignal(uint64_t time, Core& core) {
        if (_changes.size() == 0) {
            return;
        }

        ChangeTuple t = _changes.top();
        if (std::get<0>(t) == time) {
            core.*_signal = std::get<1>(t);
            _changes.pop();
        }
    }
};

template <typename T, class Core>
SignalPublisher<T, Core> makePublisher(T Core:: *signal) {
    SignalPublisher<T, Core> observer(signal);
    return observer;
}

template <typename T, class Core>
SignalPublisher<T, Core> makePublisher(T Core:: *signal, const std::vector<std::tuple<uint64_t, T>> &inputs) {
    SignalPublisher<T, Core> observer(signal);
    observer.addInputs(inputs);
    return observer;
}

#endif