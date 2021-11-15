#include "TestBench.hpp"

// Called by $time in Verilog
// converts to double, to match
// what SystemC does
double sc_time_stamp (void)
{
    return 0;
}
