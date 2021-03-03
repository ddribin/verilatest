#include "SignalObserver.hpp"

using namespace std;

ostream& operator << (ostream& os, SignalObserver8::change_tuple const& value )
{
    os << "{" << std::get<0>(value) << ", " << static_cast<int>(std::get<1>(value)) << "}";
    return os;
}