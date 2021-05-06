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
};

#endif
