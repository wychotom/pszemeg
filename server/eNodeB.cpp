#include "eNodeB.h"

eNodeB::eNodeB() : ch1(6742)
{
    ch1.handle_connections();
}
