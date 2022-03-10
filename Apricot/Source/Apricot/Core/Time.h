// Part of Apricot Engine. 2022-2022.
// Module: Core

#pragma once

#include "Base.h"

namespace Apricot {

	class APRICOT_API Time
	{
	public:
		Time() = default;

		Time(uint64 time)
			: m_Time(time) {}

		operator uint64() const { return m_Time; }

		float Microseconds() const { return (float)((float64)m_Time * 0.001); }
		float Miliseconds()  const { return (float)((float64)m_Time * 0.000001); }
		float Seconds()      const { return (float)((float64)m_Time * 0.000000001); }

		void SetMicroseconds(uint64 time) { m_Time = time * 1000u; }
		void SetMiliseconds(uint64 time)  { m_Time = time * 1000000u; }
		void SetSeconds(uint64 time)      { m_Time = time * 1000000000u; }

	private:
		uint64 m_Time = 0;
	};

	class APRICOT_API Timestep
	{
	public:
		Timestep()
			: m_DeltaTime(0.0f) {}

		Timestep(float DeltaTime)
			: m_DeltaTime(DeltaTime) {}

		operator float() const { return m_DeltaTime; }

		float DeltaTime() const { return m_DeltaTime; }

		float Miliseconds() const { return m_DeltaTime * 1000.0f; }

	private:
		float m_DeltaTime;
	};

}