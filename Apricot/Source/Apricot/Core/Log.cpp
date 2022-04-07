#include "aepch.h"
#include "Log.h"

#include "Platform.h"
#include "Apricot/Utils/Buffer.h"

namespace Apricot {

	static uint64 s_EncodingSizes[(uint32)OutputSink::Encoding::MaxEnumValue] =
	{
		0, // None
		1, // ASCII
		2  // Unicode
	};

	static const char* s_LogSeveritiesASCII[(uint32)OutputSink::ContentSeverity::MaxEnumValue] =
	{
		"NONE",
		"TRACE",
		"DEBUG",
		"INFO",
		"WARN",
		"ERROR",
		"FATAL"
	};

	static const wchar_t* s_LogSeveritiesUnicode[(uint32)OutputSink::ContentSeverity::MaxEnumValue] =
	{
		L"NONE",
		L"TRACE",
		L"DEBUG",
		L"INFO",
		L"WARN",
		L"ERROR",
		L"FATAL"
	};

	namespace Utils {

		static void ConvertTimeToStringASCII(uint64 time, char* buffer)
		{
			uint8 digit = (uint8)time / 10;
			buffer[0] = '0' + digit;
			digit = time % 10;
			buffer[1] = '0' + digit;
		}

		static void ConvertTimeToStringUnicode(uint64 time, wchar_t* buffer)
		{
			uint8 digit = (uint8)time / 10;
			buffer[0] = L'0' + digit;
			digit = time % 10;
			buffer[1] = L'0' + digit;
		}

	}

	void OutputSink::Init()
	{
		for (uint64 index = 0; index < m_Specification.StreamsCount; index++)
		{
			GetStream(index).m_SinkBuffer = StreamingBuffer::Create(8 * Mega2Bytes);
		}
	}

	void OutputSink::Destroy()
	{
		for (uint64 index = 0; index < m_Specification.StreamsCount; index++)
		{
			GetStream(index).m_SinkBuffer.Release();
		}
	}

	void OutputSink::SyncWriteContent(const void* content, uint64 contentSize)
	{
		for (uint64 index = 0; index < m_Specification.StreamsCount; index++)
		{
			SyncStreamWriteContent(index, content, contentSize);
		}
	}

	void OutputSink::SyncFlushContent()
	{
		for (uint64 index = 0; index < m_Specification.StreamsCount; index++)
		{
			SyncStreamFlushContent(index);
		}
	}

	void OutputSink::SyncWriteAndFlushContent(const void* content, uint64 contentSize)
	{
		SyncWriteContent(content, contentSize);
		SyncFlushContent();
	}

	void OutputSink::SetStreamSeverity(uint64 streamIndex, ContentSeverity severity)
	{
		auto& stream = m_Specification.Streams[streamIndex];
		stream.Severity = severity;
	}

	void OutputSink::SetAllStreamsSeverity(ContentSeverity severity)
	{
		for (uint64 index = 0; index < m_Specification.StreamsCount; index++)
		{
			SetStreamSeverity(index, severity);
		}
	}

	void OutputSink::SyncStreamWriteContent(uint64 streamIndex, const void* content, uint64 contentSize)
	{
		IOStream& stream = m_Specification.Streams[streamIndex];
		uint64 formatSize = astl::strlen(stream.Format);
		uint64 encodingSize = s_EncodingSizes[(uint32)m_Specification.TextEncoding];

		uint64 copyPosition = 0;

		char parameter[128] = { 0 };
		uint64 parameterPosition = 0;

		for (uint64 index = 0; index < formatSize; index++)
		{
			char formatCharacter = stream.Format[index];
			switch (formatCharacter)
			{
				case '{':
				{
					if (index - copyPosition > 0)
					{
						char* buffer = (char*)stream.m_SinkBuffer->Allocate((index - copyPosition) * sizeof(char));
						MemCpy(buffer, stream.Format + copyPosition, (index - copyPosition) * sizeof(char));
					}
					parameterPosition = index + 1;
					break;
				}
				case '}':
				{
					if (index - parameterPosition > 0)
					{
						MemCpy(parameter, stream.Format + parameterPosition, (index - parameterPosition) * sizeof(char));
						parameter[index - parameterPosition] = 0;

						if (astl::strcmp(parameter, "content"))
						{
							void* buffer = stream.m_SinkBuffer->Allocate(contentSize);
							MemCpy(buffer, content, contentSize);
						}
						else if (astl::strcmp(parameter, "severity"))
						{
							uint64 severitySize = astl::strlen(s_LogSeveritiesASCII[(uint32)stream.Severity]) * encodingSize;
							uint8* buffer = (uint8*)stream.m_SinkBuffer->Allocate(severitySize);
							switch (m_Specification.TextEncoding)
							{
								case Encoding::ASCII:
								{
									MemCpy(buffer, s_LogSeveritiesASCII[(uint32)stream.Severity], severitySize);
									break;
								}
								case Encoding::Unicode:
								{
									MemCpy(buffer, s_LogSeveritiesUnicode[(uint32)stream.Severity], severitySize);
									break;
								}
							}
						}
						else if (astl::strcmp(parameter, "hour"))
						{
							void* buffer = stream.m_SinkBuffer->Allocate(2 * encodingSize);
							UTCTime time = Platform::TimeGetSystemUTCTime();

							switch (m_Specification.TextEncoding)
							{
								case Encoding::ASCII:
								{
									Utils::ConvertTimeToStringASCII(time.Hour, (char*)buffer);
									break;
								}
								case Encoding::Unicode:
								{
									Utils::ConvertTimeToStringUnicode(time.Hour, (wchar_t*)buffer);
									break;
								}
							}
						}
						else if (astl::strcmp(parameter, "minute"))
						{
							void* buffer = stream.m_SinkBuffer->Allocate(2 * encodingSize);
							UTCTime time = Platform::TimeGetSystemUTCTime();

							switch (m_Specification.TextEncoding)
							{
								case Encoding::ASCII:
								{
									Utils::ConvertTimeToStringASCII(time.Minute, (char*)buffer);
									break;
								}
								case Encoding::Unicode:
								{
									Utils::ConvertTimeToStringUnicode(time.Minute, (wchar_t*)buffer);
									break;
								}
							}
						}
						else if (astl::strcmp(parameter, "second"))
						{
							void* buffer = stream.m_SinkBuffer->Allocate(2 * encodingSize);
							UTCTime time = Platform::TimeGetSystemUTCTime();

							switch (m_Specification.TextEncoding)
							{
							case Encoding::ASCII:
							{
								Utils::ConvertTimeToStringASCII(time.Second, (char*)buffer);
								break;
							}
							case Encoding::Unicode:
							{
								Utils::ConvertTimeToStringUnicode(time.Second, (wchar_t*)buffer);
								break;
							}
							}
						}
						else if (astl::strcmp(parameter, "param0"))
						{
							void* buffer = stream.m_SinkBuffer->Allocate(stream.Parameters[0].Size);
							MemCpy(buffer, stream.Parameters[0].Data, stream.Parameters[0].Size);
						}
						else if (astl::strcmp(parameter, "param1"))
						{
							void* buffer = stream.m_SinkBuffer->Allocate(stream.Parameters[1].Size);
							MemCpy(buffer, stream.Parameters[1].Data, stream.Parameters[1].Size);
						}
						else if (astl::strcmp(parameter, "param2"))
						{
							void* buffer = stream.m_SinkBuffer->Allocate(stream.Parameters[2].Size);
							MemCpy(buffer, stream.Parameters[2].Data, stream.Parameters[2].Size);
						}
						else if (astl::strcmp(parameter, "param3"))
						{
							void* buffer = stream.m_SinkBuffer->Allocate(stream.Parameters[3].Size);
							MemCpy(buffer, stream.Parameters[3].Data, stream.Parameters[3].Size);
						}
					}

					copyPosition = index + 1;
					break;
				}
			}
		}
		if (formatSize - copyPosition > 0)
		{
			char* buffer = (char*)stream.m_SinkBuffer->Allocate((formatSize - copyPosition) * sizeof(char));
			MemCpy(buffer, stream.Format + copyPosition, (formatSize - copyPosition) * sizeof(char));
		}
	}

	void OutputSink::SyncStreamFlushContent(uint64 streamIndex)
	{
		auto& stream = m_Specification.Streams[streamIndex];
		switch (stream.Destination)
		{
			case IOStreamDestinationType::Console:
			{
				SyncWriteToConsole(stream);
				break;
			}
			case IOStreamDestinationType::File:
			{
				break;
			}
			case IOStreamDestinationType::Callback:
			{
				break;
			}
		}
		stream.m_SinkBuffer->Reset();
	}

	void OutputSink::ClearContent()
	{
		for (uint64 index = 0; index < m_Specification.StreamsCount; index++)
		{
			GetStream(index).m_SinkBuffer->Reset();
		}
	}
	
	struct LoggerData
	{
		Ref<OutputSink> Sink;
	};
	static LoggerData s_LoggerData;

	void Logger::Init()
	{
		s_LoggerData.Sink = Ref<OutputSink>::Create();

		OutputSink::Specification& spec = s_LoggerData.Sink->GetSpecification();
		spec.TextEncoding = OutputSink::Encoding::ASCII;
		spec.SinkTag = "Apricot Core Sink";
		spec.StreamsCount = 3;

		// Main Stream
		spec.Streams[0].Destination = OutputSink::IOStreamDestinationType::Console;
		spec.Streams[0].Type = OutputSink::IOStreamType::Output;
		spec.Streams[0].Severity = OutputSink::ContentSeverity::Info;
		spec.Streams[0].Format = "[{hour}:{minute}:{second}][{severity}]: {content}\n";

		// Tagged Stream
		spec.Streams[1].Destination = OutputSink::IOStreamDestinationType::Console;
		spec.Streams[1].Type = OutputSink::IOStreamType::Output;
		spec.Streams[1].Severity = OutputSink::ContentSeverity::Info;
		spec.Streams[1].Format = "[{hour}:{minute}:{second}][{severity}][{param0}]: {content}\n";

		// Dump Crash Stream
		spec.Streams[2].Destination = OutputSink::IOStreamDestinationType::File;
		spec.Streams[2].Type = OutputSink::IOStreamType::Output;
		spec.Streams[2].Severity = OutputSink::ContentSeverity::Fatal;
		spec.Streams[2].Format = "[{hour}:{minute}:{second}][CRASH_REPORT][{severity}]: {content}\n";

		s_LoggerData.Sink->Init();

		s_LoggerData.Sink->OpenStream(0, [](void*& handle)
		{
			handle = Platform::ConsoleGetNativeHandle();
		});

		s_LoggerData.Sink->OpenStream(1, [](void*& handle)
		{
			handle = Platform::ConsoleGetNativeHandle();
		});

		s_LoggerData.Sink->OpenStream(2, [](void*& handle)
		{
			const wchar_t* handleFilePath = L"CrashReport.log";
			handle = (void*)handleFilePath;
		});
	}

	void Logger::Destroy()
	{
		s_LoggerData.Sink->Destroy();
		s_LoggerData.Sink.Release();
	}

	void Logger::SyncWrite(const char* content, OutputSink::ContentSeverity severity)
	{
		s_LoggerData.Sink->SetStreamSeverity(0, severity);

		s_LoggerData.Sink->SyncStreamWriteContent(0, content, astl::strlen(content) * sizeof(char));
		s_LoggerData.Sink->SyncStreamFlushContent(0);
	}

	void Logger::SyncWriteTagged(const char* tag, const char* content, OutputSink::ContentSeverity severity)
	{
		s_LoggerData.Sink->SetStreamSeverity(1, severity);
		
		auto& stream = s_LoggerData.Sink->GetStream(1);
		stream.ParametersCount = 1;
		stream.Parameters[0].Data = tag;
		stream.Parameters[0].Size = astl::strlen(tag) * sizeof(char);

		s_LoggerData.Sink->SyncStreamWriteContent(1, content, astl::strlen(content) * sizeof(char));
		s_LoggerData.Sink->SyncStreamFlushContent(1);
	}

}