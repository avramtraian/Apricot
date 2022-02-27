// Part of Apricot Engine. 2022-2022.
// Module: Core

#include "aepch.h"
#include "CrashReporter.h"

#include "Memory/ApricotMemory.h"

namespace Apricot {
	
	ACrashReporter* GCrashReporter = nullptr;

	static TChar SAssertionBuffer[32000] = { 0 };

	static TChar SInternalBuffer[32000] = { 0 };

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

		GCrashReporter = (ACrashReporter*)GMalloc->Alloc(sizeof(ACrashReporter));

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

		GMalloc->Free(GCrashReporter, sizeof(ACrashReporter));
		GCrashReporter = nullptr;
	}

	void ACrashReporter::PreCheckFailed(const TChar* File, const TChar* Function, uint64 Line, const TChar* Expression, const TChar* Message)
	{
		if (Message)
		{
			AE_CORE_ERROR(TEXT(
				"CHECK FAILED:\n"
				"            [EXPR]: {}\n"
				"            [FILE]: {}\n"
				"            [FUNC]: {}\n"
				"            [LINE]: {}\n"
				"----------------------------\n"
				"            {}"),
			Expression, File, Function, Line, Message);
		}
	}

	void ACrashReporter::PostCheckFailed(const TChar* File, const TChar* Function, uint64 Line, const TChar* Expression, const TChar* Message)
	{
		
	}

	void ACrashReporter::PreCheckNoEntryFailed(const TChar* File, const TChar* Function, uint64 Line)
	{
		
	}

	void ACrashReporter::PostCheckNoEntryFailed(const TChar* File, const TChar* Function, uint64 Line)
	{
		
	}

	void ACrashReporter::PreCheckUnimplementedFailed(const TChar* File, const TChar* Function, uint64 Line)
	{
		
	}

	void ACrashReporter::PostCheckUnimplementedFailed(const TChar* File, const TChar* Function, uint64 Line)
	{
		
	}

	void ACrashReporter::PreVerifyFailed(const TChar* File, const TChar* Function, uint64 Line, const TChar* Expression, const TChar* Message)
	{
		if (Message)
		{
			AE_CORE_ERROR(TEXT(
				"VERIFY FAILED:\n"
				"            [EXPR]: {}\n"
				"            [FILE]: {}\n"
				"            [FUNC]: {}\n"
				"            [LINE]: {}\n"
				"----------------------------\n"
				"            {}"),
				Expression, File, Function, Line, Message);
		}
	}

	void ACrashReporter::PostVerifyFailed(const TChar* File, const TChar* Function, uint64 Line, const TChar* Expression, const TChar* Message)
	{
		
	}

	void ACrashReporter::EnsureFailed(const TChar* File, const TChar* Function, uint64 Line, const TChar* Expression, const TChar* Message)
	{
		
	}

	void ACrashReporter::EnsureAlwaysFailed(const TChar* File, const TChar* Function, uint64 Line, const TChar* Expression, const TChar* Message)
	{
		if (Message)
		{
			AE_CORE_ERROR(TEXT(
				"ENSURE FAILED:\n"
				"            [EXPR]: {}\n"
				"            [FILE]: {}\n"
				"            [FUNC]: {}\n"
				"            [LINE]: {}\n"
				"----------------------------\n"
				"            {}"),
				Expression, File, Function, Line, Message);
		}
	}

	void ACrashReporter::SubmitArenaFailure(EMemoryOperation PerformedOperation, EMemoryError ErrorFlag)
	{
		switch (PerformedOperation)
		{
			case EMemoryOperation::Allocate:
			{
				switch (ErrorFlag)
				{
					case EMemoryError::OutOfMemory:
					{
						AE_CORE_WARN(TEXT("CrashReporter: Allocate operation triggered OutOfMemory! RequestedSize: {}, FreeSize: {}"), MemoryState.Size, MemoryState.FreeSize);
						break;
					}
					case EMemoryError::OutOfMemoryUnableToGrow:
					{
						AE_CORE_WARN(TEXT("CrashReporter: Allocate operation triggered OutOfMemoryUnableToGrow! Couldn't allocate a new page because 'bShouldGrow' is false... RequestedSize: {}, FreeSize: {}"), MemoryState.Size, MemoryState.FreeSize);
						break;
					}
					default:
					{
						AE_CORE_RASSERT_NO_ENTRY();
						break;
					}
				}
				break;
			}
			case EMemoryOperation::Free:
			{
				break;
			}
			case EMemoryOperation::NewPage:
			{
				break;
			}
			case EMemoryOperation::Create:
			{
				break;
			}
			case EMemoryOperation::Destroy:
			{
				break;
			}
			default:
			{
				AE_CORE_RASSERT_NO_ENTRY();
				break;
			}
		}
		MemoryState = AMemoryState{};
	}

}