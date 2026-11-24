#ifndef __SIMPLE_AUX_HEADER__
#define __SIMPLE_AUX_HEADER__
// namespace SimpleAux
// {

#define GET_MAX(A, B) ( (A) > (B) ? (A) : (B) )
#define GET_MIN(A, B) ( (A) < (B) ? (A) : (B) )
#define IN_RANGE(X, A, B) ( ((A) <= (X)) && ((X) <= (B)) )
#define CLAMP(X, A, B) ( GET_MIN( (B), GET_MAX( (X), (A)) ) )

bool IsAlmostZero(float Val);

// };

#endif
