#ifndef VERILATEST_SIGNAL_H
#define VERILATEST_SIGNAL_H

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
    virtual void setInitialValue(Core& core) = 0;
    virtual std::function<void (uint64_t, Core& core)> outputHook() = 0;
};

template<class Core>
class Component
{
    public:
    virtual void addInput(SignalInput<Core>& input) = 0;
    virtual void addOutput(SignalOutput<Core>& output) = 0;
};

#endif // VERILATEST_SIGNAL_H
