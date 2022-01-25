#pragma once

#include "Base.h"

namespace Apricot {

	class APRICOT_API ACrashReporter
	{
	public:
		static void Init();
		static void Destroy();

	public:
		void PreCheckFailed(const char8* File, const char8* Function, uint64 Line, const char8* Expression, const char8* Message);
		void PostCheckFailed(const char8* File, const char8* Function, uint64 Line, const char8* Expression, const char8* Message);

		void PreCheckNoEntryFailed(const char8* File, const char8* Function, uint64 Line);
		void PostCheckNoEntryFailed(const char8* File, const char8* Function, uint64 Line);

		void PreCheckUnimplementedFailed(const char8* File, const char8* Function, uint64 Line);
		void PostCheckUnimplementedFailed(const char8* File, const char8* Function, uint64 Line);

		void PreVerifyFailed(const char8* File, const char8* Function, uint64 Line, const char8* Expression, const char8* Message);
		void PostVerifyFailed(const char8* File, const char8* Function, uint64 Line, const char8* Expression, const char8* Message);

		void EnsureFailed(const char8* File, const char8* Function, uint64 Line, const char8* Expression, const char8* Message);
		void EnsureAlwaysFailed(const char8* File, const char8* Function, uint64 Line, const char8* Expression, const char8* Message);

	private:
		ACrashReporter();
		~ACrashReporter();
		ACrashReporter(const ACrashReporter&) = delete;
		ACrashReporter& operator=(const ACrashReporter&) = delete;

	public:
		char8* AssertionBuffer = nullptr;
		uint64 AssertionBufferSize = 0;

	private:
		char8* m_InternalBuffer = nullptr;
		uint64 m_InternalBufferSize = 0;
	};

	APRICOT_API extern ACrashReporter* GCrashReporter;

}