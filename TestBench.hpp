#ifndef	TESTBENCH_H
#define	TESTBENCH_H

#include <verilated.h>
#include <verilated_vcd_c.h>
#include <functional>
#include <vector>

#include "Signal.hpp"

#ifndef VERILATEST_TRACE
#define VERILATEST_TRACE 1
#endif

// Current simulation time
// This is a 64-bit integer to reduce wrap over issues and
// allow modulus.  This is in units of the timeprecision
// used in Verilog (or from --timescale-override)
extern uint64_t main_time;

template <class Core>
class TestBench
{
public:
    typedef std::function<void (uint64_t, Core&)> TickHook;

    TestBench()
        : _tickCount{0}, _trace{NULL}
    {
        Verilated::traceEverOn(true);
        _core.setClock(0);
        _core.eval();
    }

    virtual ~TestBench(void) { }

    Core& core(void)
    {
        return _core;
    }

#if VERILATEST_TRACE
    virtual void openTrace(const char * filename)
    {
        if (_trace == NULL) {
            _trace = new VerilatedVcdC;
            // Trace 99 levels of hierarchy
            _core.trace(_trace, 99);
            _trace->open(filename);
            _trace->dump(static_cast<vluint64_t>(10*_tickCount));
        }
    }

    virtual void closeTrace(void)
    {
        if (_trace != NULL) {
            _trace->close();
            delete _trace;
            _trace = NULL;
        }
    }
#endif

    uint64_t tickCount(void)
    {
        return _tickCount;
    }

    virtual void eval(void)
    {
        _core.eval();
    }

    void tick(uint64_t count = 0)
    {
        while (count > 0) {
            oneTick();
            count--;
        }
    }

    void addInput(SignalInput<Core>& input)
    {
        addInputHook(input.inputHook());
    }

    void addOutput(SignalOutput<Core>& output)
    {
        addOutputHook(output.outputHook());
    }

    void addInputHook(TickHook hook)
    {
        _inputHooks.push_back(hook);
    }

    void addOutputHook(TickHook hook)
    {
        _outputHooks.push_back(hook);
    }

private:
    Core _core;
    uint64_t _tickCount;
    VerilatedVcdC *_trace;
    std::vector<TickHook> _inputHooks;
    std::vector<TickHook> _outputHooks;

    void callInputHooks()
    {
        for (auto hook : _inputHooks) {
            hook(_tickCount, _core);
        }
    }

    void callOutputHooks()
    {
        for (auto hook : _outputHooks) {
            hook(_tickCount, _core);
        }
    }

    void incrementTick()
    {
        _tickCount++;
        main_time++;
    }

    void oneTick(void)
    {
        incrementTick();
        // Call input hooks after incrementTick() so the _tickCount is the same as the output hooks.
        callInputHooks();
        _core.eval();
#if VERILATEST_TRACE
        if (_trace != NULL) {
            _trace->dump(static_cast<vluint64_t>(10*_tickCount-2));
        }
#endif

        // Set clock high
        _core.setClock(1);
        _core.eval();
#if VERILATEST_TRACE
        if (_trace != NULL) {
            _trace->dump(static_cast<vluint64_t>(10*_tickCount));
        }
#endif

        // Set clock low
        _core.setClock(0);
        _core.eval();
#if VERILATEST_TRACE
        if (_trace != NULL) {
            _trace->dump(static_cast<vluint64_t>(10*_tickCount+5));
            _trace->flush();
        }
#endif
        callOutputHooks();
    }
};

#endif