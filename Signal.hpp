#ifndef SIGNAL_H
#define SIGNAL_H

#include <functional>

template<class Core>
class SignalInput
{
public:
    virtual std::function<void (uint64_t, Core& core)> inputHook() = 0;
};

template<class Core>
class SignalOutput
{
public:
    virtual std::function<void (uint64_t, Core& core)> outputHook() = 0;
};

#endif // SIGNAL_H