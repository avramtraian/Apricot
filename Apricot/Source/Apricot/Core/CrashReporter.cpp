// Part of Apricot Engine. 2022-2022.

#include "aepch.h"
#include "CrashReporter.h"

namespace Apricot {
	
	ACrashReporter* GCrashReporter = nullptr;

	static char8 SAssertionBuffer[32000] = { 0 };

	static char8 SInternalBuffer[32000] = { 0 };

	ACrashReporter::ACrashReporter()
	{
	}

	ACrashReporter::~ACrashReporter()
	{
	}

	void ACrashReporter::Init()
	{
		if (GCrashReporter)
		{
			return;
		}

		GCrashReporter = (ACrashReporter*)Memory_Alloc(sizeof(ACrashReporter), EAllocTag::Core);

		GCrashReporter->AssertionBuffer = SAssertionBuffer;
		GCrashReporter->AssertionBufferSize = AE_ARRAY_LENGTH(SAssertionBuffer);

		GCrashReporter->m_InternalBuffer = SInternalBuffer;
		GCrashReporter->m_InternalBufferSize = AE_ARRAY_LENGTH(SInternalBuffer);
	}

	void ACrashReporter::Destroy()
	{
		if (!GCrashReporter)
		{
			return;
		}

		Memory_Free(GCrashReporter, sizeof(ACrashReporter), EAllocTag::Core);
		GCrashReporter = nullptr;
	}

	void ACrashReporter::PreCheckFailed(const char8* File, const char8* Function, uint64 Line, const char8* Expression, const char8* Message)
	{
		if (Message)
		{
			AE_CORE_ERROR(
				"CHECK FAILED:\n"
				"            [EXPR]: {}\n"
				"            [FILE]: {}\n"
				"            [FUNC]: {}\n"
				"            [LINE]: {}\n"
				"----------------------------\n"
				"            {}",
			Expression, File, Function, Line, Message);
		}
	}

	void ACrashReporter::PostCheckFailed(const char8* File, const char8* Function, uint64 Line, const char8* Expression, const char8* Message)
	{
		
	}

	void ACrashReporter::PreCheckNoEntryFailed(const char8* File, const char8* Function, uint64 Line)
	{
		
	}

	void ACrashReporter::PostCheckNoEntryFailed(const char8* File, const char8* Function, uint64 Line)
	{
		
	}

	void ACrashReporter::PreCheckUnimplementedFailed(const char8* File, const char8* Function, uint64 Line)
	{
		
	}

	void ACrashReporter::PostCheckUnimplementedFailed(const char8* File, const char8* Function, uint64 Line)
	{
		
	}

	void ACrashReporter::PreVerifyFailed(const char8* File, const char8* Function, uint64 Line, const char8* Expression, const char8* Message)
	{
		if (Message)
		{
			AE_CORE_ERROR(
				"VERIFY FAILED:\n"
				"            [EXPR]: {}\n"
				"            [FILE]: {}\n"
				"            [FUNC]: {}\n"
				"            [LINE]: {}\n"
				"----------------------------\n"
				"            {}",
				Expression, File, Function, Line, Message);
		}
	}

	void ACrashReporter::PostVerifyFailed(const char8* File, const char8* Function, uint64 Line, const char8* Expression, const char8* Message)
	{
		
	}

	void ACrashReporter::EnsureFailed(const char8* File, const char8* Function, uint64 Line, const char8* Expression, const char8* Message)
	{
		
	}

	void ACrashReporter::EnsureAlwaysFailed(const char8* File, const char8* Function, uint64 Line, const char8* Expression, const char8* Message)
	{
		if (Message)
		{
			AE_CORE_ERROR(
				"ENSURE FAILED:\n"
				"            [EXPR]: {}\n"
				"            [FILE]: {}\n"
				"            [FUNC]: {}\n"
				"            [LINE]: {}\n"
				"----------------------------\n"
				"            {}",
				Expression, File, Function, Line, Message);
		}
	}

}