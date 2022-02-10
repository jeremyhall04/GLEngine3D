#ifndef TIMER_H
#define TIMER_H

#include <Windows.h>

class Timer
{
private:
	LARGE_INTEGER m_Start;
	double m_Frequency;

public:
	Timer()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		m_Frequency = 1.0 / frequency.QuadPart;
		QueryPerformanceCounter(&m_Start);
	}

	inline void reset()
	{
		QueryPerformanceCounter(&m_Start);
	};

	// returns time in seconds
	inline float elapsed()
	{
		LARGE_INTEGER current;
		QueryPerformanceCounter(&current);
		unsigned _int64 cycles = current.QuadPart - m_Start.QuadPart;
		return (float)(cycles * m_Frequency);
	};
};

#endif // !TIMER_H
