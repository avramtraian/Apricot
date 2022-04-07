#pragma once

#include "Base.h"

constexpr uint64 Micro2Nano = 1000u;
constexpr uint64 Mili2Nano  = 1000u * Micro2Nano;
constexpr uint64 Sec2Nano   = 1000u * Mili2Nano;

constexpr double Nano2Micro = 0.001;
constexpr double Nano2Mili  = 0.001 * Nano2Micro;
constexpr double Nano2Sec   = 0.001 * Nano2Mili;

namespace Apricot {

	struct UTCTime
	{
		uint64 Year   = 0;
		uint64 Month  = 0;
		uint64 Day 	  = 0;
		uint64 Hour   = 0;
		uint64 Minute = 0;
		uint64 Second = 0;
	};

	struct Time
	{
		Time() = default;

		Time(uint64 nanoseconds)
			: Nanoseconds(nanoseconds) {}

		float Seconds()      const { return (float)(Nanoseconds * Nano2Sec); }
		float Miliseconds()  const { return (float)(Nanoseconds * Nano2Mili); }
		float Microseconds() const { return (float)(Nanoseconds * Nano2Micro); }

		double dSeconds()      const { return Nanoseconds * Nano2Sec; }
		double dMiliseconds()  const { return Nanoseconds * Nano2Mili; }
		double dMicroseconds() const { return Nanoseconds * Nano2Micro; }

		uint64 Nanoseconds = 0;
	};

	class Timestep
	{
	public:
		Timestep() = default;

		Timestep(uint64 nanoseconds)
			: m_Time(nanoseconds) {}

		Timestep(Time time)
			: m_Time(time) {}

		Timestep(Time t0, Time t1)
			: m_Time(t1.Nanoseconds - t0.Nanoseconds) {}

		operator float() const
		{
			return m_Time.Seconds();
		}

		operator double() const
		{
			return m_Time.dSeconds();
		}

	private:
		Time m_Time;
	};

}