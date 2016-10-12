#pragma once
template <typename T>
T lerp(T p0, T p1, float t)
{
	return (1.0f - t) * p0 + p1 * t;
}