// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Base.h"

namespace Apricot {

	class APRICOT_API ACrashReporter
	{
	public:
		static void Init();
		static void Destroy();

	public:
		void PreCheckFailed(const TChar* File, const TChar* Function, uint64 Line, const TChar* Expression, const TChar* Message);
		void PostCheckFailed(const TChar* File, const TChar* Function, uint64 Line, const TChar* Expression, const TChar* Message);

		void PreCheckNoEntryFailed(const TChar* File, const TChar* Function, uint64 Line);
		void PostCheckNoEntryFailed(const TChar* File, const TChar* Function, uint64 Line);

		void PreCheckUnimplementedFailed(const TChar* File, const TChar* Function, uint64 Line);
		void PostCheckUnimplementedFailed(const TChar* File, const TChar* Function, uint64 Line);

		void PreVerifyFailed(const TChar* File, const TChar* Function, uint64 Line, const TChar* Expression, const TChar* Message);
		void PostVerifyFailed(const TChar* File, const TChar* Function, uint64 Line, const TChar* Expression, const TChar* Message);

		void EnsureFailed(const TChar* File, const TChar* Function, uint64 Line, const TChar* Expression, const TChar* Message);
		void EnsureAlwaysFailed(const TChar* File, const TChar* Function, uint64 Line, const TChar* Expression, const TChar* Message);

	private:
		ACrashReporter();
		~ACrashReporter();
		ACrashReporter(const ACrashReporter&) = delete;
		ACrashReporter& operator=(const ACrashReporter&) = delete;

	public:
		TChar* AssertionBuffer = nullptr;
		uint64 AssertionBufferSize = 0;

	private:
		TChar* m_InternalBuffer = nullptr;
		uint64 m_InternalBufferSize = 0;
	};

	APRICOT_API extern ACrashReporter* GCrashReporter;

}