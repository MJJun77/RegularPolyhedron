#include <cmath>
#include <cfloat>
#include "simple_aux.h"

using namespace std;

// namespace SimpleAux
// {

#define EPSILON_FACTOR (5.0f)

bool IsAlmostZero(float fVal)
{
	return IN_RANGE(fVal, -FLT_EPSILON * EPSILON_FACTOR, FLT_EPSILON * EPSILON_FACTOR);
}

// };
