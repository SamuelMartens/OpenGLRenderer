#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

namespace Utils
{
	template<typename T>
	inline float DegToRad(T degreeVal )
	{
		return degreeVal * M_PI / 180;
	}
}