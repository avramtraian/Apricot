// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Base.h"
#include "Apricot/DataStructures/String.h"

namespace Apricot {

	struct APRICOT_API HTime
	{
	public:
		HTime() = default;
		HTime(uint64 time)
			: Time(time) {}

		static HTime Sec  (uint64 seconds)      { return HTime(seconds      * 1000 * 1000 * 1000); }
		static HTime Mili (uint64 miliseconds)  { return HTime(miliseconds  * 1000 * 1000); }
		static HTime Micro(uint64 microseconds) { return HTime(microseconds * 1000); }
		static HTime Nano (uint64 nanoseconds)  { return HTime(nanoseconds); }

		operator uint64() const { return Time; }

		bool operator==(const HTime& other) const { return (Time == other.Time); }
		bool operator!=(const HTime& other) const { return (Time != other.Time); }
		bool operator> (const HTime& other) const { return (Time > other.Time); }
		bool operator>=(const HTime& other) const { return (Time >= other.Time); }
		bool operator< (const HTime& other) const { return (Time < other.Time); }
		bool operator<=(const HTime& other) const { return (Time <= other.Time); }

		HTime  operator+ (const HTime& other) const { return HTime(Time + other.Time); }
		HTime  operator- (const HTime& other) const { return HTime(Time - other.Time); }
		HTime& operator+=(const HTime& other) { Time += other.Time; return *this; }
		HTime& operator-=(const HTime& other) { Time -= other.Time; return *this; }

	public:
		uint64 Microseconds() const { return Time / 1000; }
		uint64 Miliseconds()  const { return Time / 1000 / 1000; }
		uint64 Seconds()      const { return Time / 1000 / 1000 / 1000; }

		float64 FMicroseconds() const { return (float64)Time * 0.001; }
		float64 FMiliseconds()  const { return (float64)Time * 0.000001; }
		float64 FSeconds()      const { return (float64)Time * 0.000000001; }

	public:
		/*
		* The time in nanoseconds.
		*/
		uint64 Time = 0;
	};

	struct APRICOT_API HTimestep
	{
	public:
		HTimestep() = default;

		HTimestep(float32 deltaTime)
			: DeltaTime(deltaTime) {}

		HTimestep(HTime time)
			: DeltaTime((float32)time.FSeconds()) {}

		operator float32() const { return DeltaTime; }

	public:
		float32 DeltaTime = 0.0f;
	};

}