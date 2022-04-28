#pragma once

#include "Base.h"
#include "Apricot/Utils/Buffer.h"

namespace Apricot {

	class OutputSink : public RefCounted
	{
	public:
		enum class ContentSeverity
		{
			Unknown = -1, None = 0,
			Trace = 1, Debug = 2, Info = 3, Warn = 4, Error = 5, Fatal = 6,
			MaxEnumValue,
		};

		enum class ConsoleColor
		{
			Black = 0,
			Blue = 1,
			Green = 2,
			Aqua = 3,
			Red = 4,
			Purple = 5,
			Yellow = 6,
			White = 7,
			Gray = 8,
			BrightBlue = 9,
			BrightGreen = 10,
			BrightAqua = 11,
			BrightRed = 12,
			BrightPurple = 13,
			BrightYellow = 14,
			BrightWhite = 15,

			MaxEnumValue,
		};

		enum class Encoding
		{
			Unknown = -1, None = 0,
			ASCII = 1, Unicode = 2,
			// UTF8 = 3, UTF16 = 4,
			MaxEnumValue,
		};

		enum class IOStreamType
		{
			None = 0,
			Input = 1, Output = 2,
			MaxEnumValue
		};

		enum class IOStreamDestinationType
		{
			None = 0,
			Console = 1, File = 2, Callback = 3,
			MaxEnumValue
		};

		struct IOStream
		{
			IOStreamType Type = IOStreamType::None;
			IOStreamDestinationType Destination = IOStreamDestinationType::None;
			ContentSeverity Severity = ContentSeverity::None;

			const char* Format = "{content}\n";

			struct Param
			{
				const void* Data = nullptr;
				uint64 Size = 0;
			};
			astl::array<Param, 4> Parameters;
			uint64 ParametersCount = 0;

			void* Handle = nullptr;

		private:
			Ref<StreamingBuffer> m_SinkBuffer;

			friend class OutputSink;
		};

		struct ConsoleColorScheme
		{
			struct ColorCombination
			{
				ConsoleColor Foreground = ConsoleColor::White;
				ConsoleColor Background = ConsoleColor::Black;
			};

			ColorCombination Colors[(uint32)ContentSeverity::MaxEnumValue] =
			{
				{ ConsoleColor::White,        ConsoleColor::Black },
				{ ConsoleColor::Gray,         ConsoleColor::Black },
				{ ConsoleColor::Purple,       ConsoleColor::Black },
				{ ConsoleColor::Green,        ConsoleColor::Black },
				{ ConsoleColor::BrightYellow, ConsoleColor::Black },
				{ ConsoleColor::BrightRed,    ConsoleColor::Black },
				{ ConsoleColor::Black,        ConsoleColor::Red   }
			};
		};

		struct Specification
		{
			const char* SinkTag = "Unnamed Sink";

			astl::array<IOStream, 8> Streams;
			uint64 StreamsCount = 0;

			Encoding TextEncoding = Encoding::None;

			ConsoleColorScheme ColorScheme;
		};

		struct FormatParameters
		{
			
		};

	public:
		OutputSink() = default;
		~OutputSink() = default;

		void Init();
		void Destroy();

	public:
		void SyncWriteContent(const void* content, uint64 contentSize);
		void SyncFlushContent();
		void SyncWriteAndFlushContent(const void* content, uint64 contentSize);

		void AsyncWriteContent(const void* content, uint64 contentSize);
		void AsyncFlushContent();
		void AsyncWriteAndFlushContent(const void* content, uint64 contentSize);

		template<typename OpenFunction>
		void OpenStream(uint64 streamIndex, OpenFunction open)
		{
			if (streamIndex >= m_Specification.StreamsCount) return;
			open(m_Specification.Streams[streamIndex].Handle);
		}

		template<typename CloseFunction>
		void CloseStream(uint64 streamIndex, CloseFunction close)
		{
			if (streamIndex >= m_Specification.StreamsCount) return;
			close(m_Specification.Streams[streamIndex].Handle);
		}

		void SetStreamSeverity(uint64 streamIndex, ContentSeverity severity);
		void SetAllStreamsSeverity(ContentSeverity severity);

		void SyncStreamWriteContent(uint64 streamIndex, const void* content, uint64 contentSize);
		void SyncStreamFlushContent(uint64 streamIndex);

		void AsyncStreamWriteContent(uint64 streamIndex, const void* content, uint64 contentSize);
		void AsyncStreamFlushContent(uint64 streamIndex);

		void ClearContent();

		IOStream& GetStream(uint64 streamIndex)
		{
			AE_CORE_ASSERT(streamIndex < m_Specification.StreamsCount);
			return m_Specification.Streams[streamIndex];
		}

		const IOStream& GetStream(uint64 streamIndex) const
		{
			AE_CORE_ASSERT(streamIndex < m_Specification.StreamsCount);
			return m_Specification.Streams[streamIndex];
		}

		Specification& GetSpecification() { return m_Specification; }
		const Specification& GetSpecification() const { return m_Specification; }

	private:
		void SyncWriteToConsole(IOStream& stream);
		void AsyncWriteToConsole(IOStream& stream);

	private:
		Specification m_Specification;
	};

	class APRICOT_API Logger
	{
	public:
		static void Init();
		static void Destroy();

		static void SyncWrite(const char* content, OutputSink::ContentSeverity severity);
		static void SyncWriteTagged(const char* tag, const char* content, OutputSink::ContentSeverity severity);
	};

}

#ifdef AE_DEBUG
	#define AE_ENABLE_TRACE_LOGS
	#define AE_ENABLE_DEBUG_LOGS
	#define AE_ENABLE_INFO_LOGS
	#define AE_ENABLE_WARN_LOGS
	#define AE_ENABLE_ERROR_LOGS
	#define AE_ENABLE_FATAL_LOGS
#elif defined(RELEASE_BUILD)
	#define AE_ENABLE_INFO_LOGS
	#define AE_ENABLE_WARN_LOGS
	#define AE_ENABLE_ERROR_LOGS
	#define AE_ENABLE_FATAL_LOGS
#elif defined(AE_SHIPPING)
	#define AE_ENABLE_FATAL_LOGS
#endif

#ifdef AE_ENABLE_TRACE_LOGS
	#define AE_CORE_TRACE(...)          { ::Apricot::Logger::SyncWrite(__VA_ARGS__, ::Apricot::OutputSink::ContentSeverity::Trace); }
	#define AE_CORE_TRACE_TAG(Tag, ...) { ::Apricot::Logger::SyncWriteTagged(Tag, __VA_ARGS__, ::Apricot::OutputSink::ContentSeverity::Trace); }
#else
	#define AE_CORE_TRACE(...)
	#define AE_CORE_TRACE_TAG(Tag, ...)
#endif

#ifdef AE_ENABLE_DEBUG_LOGS
	#define AE_CORE_DEBUG(...)          { ::Apricot::Logger::SyncWrite(__VA_ARGS__, ::Apricot::OutputSink::ContentSeverity::Debug); }
	#define AE_CORE_DEBUG_TAG(Tag, ...) { ::Apricot::Logger::SyncWriteTagged(Tag, __VA_ARGS__, ::Apricot::OutputSink::ContentSeverity::Debug); }
#else
	#define AE_CORE_DEBUG(...)
	#define AE_CORE_DEBUG_TAG(Tag, ...)
#endif

#ifdef AE_ENABLE_INFO_LOGS
	#define AE_CORE_INFO(...)           { ::Apricot::Logger::SyncWrite(__VA_ARGS__, ::Apricot::OutputSink::ContentSeverity::Info); }
	#define AE_CORE_INFO_TAG(Tag, ...)  { ::Apricot::Logger::SyncWriteTagged(Tag, __VA_ARGS__, ::Apricot::OutputSink::ContentSeverity::Info); }
#else
	#define AE_CORE_INFO(...)
	#define AE_CORE_INFO_TAG(Tag, ...)
#endif

#ifdef AE_ENABLE_WARN_LOGS
	#define AE_CORE_WARN(...)           { ::Apricot::Logger::SyncWrite(__VA_ARGS__, ::Apricot::OutputSink::ContentSeverity::Warn); }
	#define AE_CORE_WARN_TAG(Tag, ...)  { ::Apricot::Logger::SyncWriteTagged(Tag, __VA_ARGS__, ::Apricot::OutputSink::ContentSeverity::Warn); }
#else
	#define AE_CORE_WARN(...)
	#define AE_CORE_WARN_TAG(Tag, ...)
#endif

#ifdef AE_ENABLE_ERROR_LOGS
	#define AE_CORE_ERROR(...)          { ::Apricot::Logger::SyncWrite(__VA_ARGS__, ::Apricot::OutputSink::ContentSeverity::Error); }
	#define AE_CORE_ERROR_TAG(Tag, ...) { ::Apricot::Logger::SyncWriteTagged(Tag, __VA_ARGS__, ::Apricot::OutputSink::ContentSeverity::Error); }
#else
	#define AE_CORE_ERROR(...)
	#define AE_CORE_ERROR_TAG(Tag, ...)
#endif

#ifdef AE_ENABLE_FATAL_LOGS
	#define AE_CORE_FATAL(...)          { ::Apricot::Logger::SyncWrite(__VA_ARGS__, ::Apricot::OutputSink::ContentSeverity::Fatal); }
	#define AE_CORE_FATAL_TAG(Tag, ...) { ::Apricot::Logger::SyncWriteTagged(Tag, __VA_ARGS__, ::Apricot::OutputSink::ContentSeverity::Fatal); }
#else
	#define AE_CORE_FATAL(...)
	#define AE_CORE_FATAL_TAG(Tag, ...)
#endif