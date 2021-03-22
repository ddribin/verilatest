#ifndef	TESTBENCH_H
#define	TESTBENCH_H

#include <verilated.h>
#include <verilated_vcd_c.h>
#include <functional>
#include <vector>


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
        _core.clock = 0;
        _core.eval();
    }

    virtual ~TestBench(void) { }

    Core& core(void)
    {
        return _core;
    }

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
    void addPreHook(TickHook hook)
    {
        _preHooks.push_back(hook);
    }

    void addPostHook(TickHook hook)
    {
        _postHooks.push_back(hook);
    }

    void tick_n(uint64_t count, std::function<void (uint64_t, Core *)>func)
    {
        while (count > 0) {
            tick();
            func(_tickCount, _core);
            count--;
        }
    }

private:
    Core _core;
    uint64_t _tickCount;
    VerilatedVcdC *_trace;
    std::vector<TickHook> _preHooks;
    std::vector<TickHook> _postHooks;

    void callPreHooks()
    {
        for (auto h : _preHooks) {
            h(_tickCount, _core);
        }
    }

    void callPostHooks()
    {
        for (auto h : _postHooks) {
            h(_tickCount, _core);
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
        // Call pre hooks after incrementTick() so the _tickCount is the same as the post hooks.
        callPreHooks();
        _core.eval();
        if (_trace != NULL) {
            _trace->dump(static_cast<vluint64_t>(10*_tickCount-2));
        }

        // Set clock high
        _core.clock = 1;
        _core.eval();
        if (_trace != NULL) {
            _trace->dump(static_cast<vluint64_t>(10*_tickCount));
        }

        // Set clock low
        _core.clock = 0;
        _core.eval();
        if (_trace != NULL) {
            _trace->dump(static_cast<vluint64_t>(10*_tickCount+5));
            _trace->flush();
        }
        callPostHooks();
    }
};

#endif