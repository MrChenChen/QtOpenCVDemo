#pragma once
#include <Windows.h>


class  Stopwatch
{
private:

	LARGE_INTEGER  nFreq;

	LARGE_INTEGER nBeginTime;

	LARGE_INTEGER nEndTime;

	unsigned long time;

public:

	Stopwatch()
	{
		QueryPerformanceFrequency(&nFreq);
	}

	void Start()
	{
		if (nFreq.QuadPart == NULL)
		{
			QueryPerformanceFrequency(&nFreq);
		}

		QueryPerformanceCounter(&nBeginTime);

	}


	//µ¥Î»ÎªºÁÃë
	double Stop()
	{
		if (nBeginTime.QuadPart == 0)
		{
			return 0;
		}

		QueryPerformanceCounter(&nEndTime);

		auto time = (nEndTime.QuadPart - nBeginTime.QuadPart)*1000.0 / nFreq.QuadPart;

		nBeginTime.QuadPart = 0;

		return time;

	}


	~Stopwatch() = default;

	static unsigned __int64 GetSystemTime()
	{
		return GetTickCount64();
	}


};

