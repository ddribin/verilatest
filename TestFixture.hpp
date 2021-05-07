#ifndef	TEST_FIXTURE_H
#define	TEST_FIXTURE_H

#include "Signal.hpp"

template<class Core>
struct TestFixture {
    using Input8 = SignalPublisher<uint8_t, Core>;
    using Output8 = SignalObserver<uint8_t, Core>;
    using Output16 = SignalObserver<uint16_t, Core>;
    TestBench<Core> bench;
    Core& core;
    TestFixture() :
        core(bench.core())
    {
    }

    virtual ~TestFixture() {}

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
