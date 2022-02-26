// Part of Apricot Engine. 2022-2022.
// Module: Core

#pragma once

#include "Base.h"

namespace Apricot {

	class AMemoryArena;
	enum class EMemoryError : int16;

	class APRICOT_API ACrashReporter
	{
	/* Typedefs */
	public:
		enum class EMemoryOperation : uint8
		{
			None = 0, Unknown = 1,

			Allocate, Free, NewPage, Create, Destroy,
		};

		struct AMemoryState
		{
			AMemoryArena* Arena = nullptr;

			uint64 Size = 0;

			uint64 Alignment = 0;

			void* MemoryBlock = nullptr;

			uint64 FreeSize = 0;
		};

	public:
		static void Init();
		static void Destroy();
	
	/* Assertions */
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
	
	/* Memory */
	public:
		void SubmitArenaFailure(EMemoryOperation PerformedOperation, EMemoryError ErrorFlag);

	private:
		ACrashReporter();
		~ACrashReporter();
		ACrashReporter(const ACrashReporter&) = delete;
		ACrashReporter& operator=(const ACrashReporter&) = delete;

	public:
		AMemoryState MemoryState;

		TChar* AssertionBuffer = nullptr;
		uint64 AssertionBufferSize = 0;

	private:
		TChar* m_InternalBuffer = nullptr;
		uint64 m_InternalBufferSize = 0;
	};

	APRICOT_API extern ACrashReporter* GCrashReporter;

}