#ifndef	VERILATEST_TEST_FIXTURE_BASE_H
#define	VERILATEST_TEST_FIXTURE_BASE_H

#include "Signal.hpp"

template<class Core>
struct TestFixtureBase {
    using Input8 = SignalPublisher<uint8_t, Core>;
    using Output8 = SignalObserver<uint8_t, Core>;
    using Output16 = SignalObserver<uint16_t, Core>;
    TestBench<Core> bench;
    Core& core;
    TestFixtureBase() :
        core(bench.core())
    {
    }

    virtual ~TestFixtureBase() {}

    Input8 makeInput(uint8_t Core:: *signal)
    {
        return Input8(signal, bench);
    };

    Output8 makeOutput(uint8_t Core:: *signal, uint8_t initialValue = 0)
    {
        return Output8(signal, bench);
    };

    Output16 makeOutput(uint16_t Core:: *signal, uint16_t initialValue = 0)
    {
        return Output16(signal, bench);
    };
};

#endif
