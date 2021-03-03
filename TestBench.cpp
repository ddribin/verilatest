#include "TestBench.hpp"

uint64_t main_time = 0; 

// Called by $time in Verilog
// converts to double, to match
// what SystemC does
double sc_time_stamp (void)
{
    return main_time;
}
