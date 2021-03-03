#ifndef	TESTBENCH_H
#define	TESTBENCH_H

#include <verilated.h>
#include <verilated_vcd_c.h>
#include <functional>


// Current simulation time
// This is a 64-bit integer to reduce wrap over issues and
// allow modulus.  This is in units of the timeprecision
// used in Verilog (or from --timescale-override)
extern uint64_t main_time;

template <class Core>
class TestBench
{
public:
    TestBench()
        : _tickCount{0}, _trace{NULL}
    {
        Verilated::traceEverOn(true);
        _core = new Core;
        _core->setClock(0);
        _core->eval();
    }

    virtual ~TestBench(void) {
        delete _core;
        _core = NULL;
    }

    Core *core(void) {
        return _core;
    }

    virtual void openTrace(const char * filename) {
        if (_trace == NULL) {
            _trace = new VerilatedVcdC;
            // Trace 99 levels of hierarchy
            _core->trace(_trace, 99);
            _trace->open(filename);
            _trace->dump(static_cast<vluint64_t>(10*_tickCount));
        }
    }

    virtual void closeTrace(void) {
        if (_trace != NULL) {
            _trace->close();
            delete _trace;
            _trace = NULL;
        }
    }

    uint64_t tickCount(void) {
        return _tickCount;
    }

    virtual void eval(void) {
        _core->eval();
    }

    virtual void tick(void) {
        incrementTick();
        _core->eval();
        if (_trace != NULL) {
            _trace->dump(static_cast<vluint64_t>(10*_tickCount-2));
        }

        // Set clock high
        _core->setClock(1);
        _core->eval();
        if (_trace != NULL) {
            _trace->dump(static_cast<vluint64_t>(10*_tickCount));
        }

        // Set clock low
        _core->setClock(0);
        _core->eval();
        if (_trace != NULL) {
            _trace->dump(static_cast<vluint64_t>(10*_tickCount+5));
            _trace->flush();
        }
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
    Core * _core;
    uint64_t _tickCount;
    VerilatedVcdC *_trace;

    void incrementTick(void) {
        _tickCount++;
        main_time++;
    }
};

#endif