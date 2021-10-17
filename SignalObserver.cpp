#include "SignalObserver.hpp"

using namespace std;

ostream& operator << (ostream& os, tuple<uint64_t, uint8_t> const& value )
{
    os << "{" << get<0>(value) << ", " << static_cast<int>(get<1>(value)) << "}";
    return os;
}

ostream& operator << (ostream& os, tuple<uint64_t, uint16_t> const& value )
{
    os << "{" << get<0>(value) << ", " << static_cast<int>(get<1>(value)) << "}";
    return os;
}

ostream& operator << (ostream& os, tuple<uint64_t, uint32_t> const& value )
{
    os << "{" << get<0>(value) << ", " << static_cast<int>(get<1>(value)) << "}";
    return os;
}
