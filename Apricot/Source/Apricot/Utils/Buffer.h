#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Memory.h"
#include "Apricot/Core/Containers/Ref.h"

namespace Apricot {

	class StreamingBuffer;

	class APRICOT_API Buffer : public RefCounted
	{
	public:
		static Ref<Buffer> Create()
		{
			return Ref<Buffer>::Create();
		}

		static Ref<Buffer> Create(const Buffer& other)
		{
			return Ref<Buffer>::Create(other);
		}

		static Ref<Buffer> Create(Buffer&& other)
		{
			return Ref<Buffer>::Create(astl::move(other));
		}

		static Ref<Buffer> Create(void* data, uint64 size)
		{
			return Ref<Buffer>::Create(data, size);
		}

		static Ref<Buffer> Create(uint64 size)
		{
			return Ref<Buffer>::Create(size);
		}

	public:
		~Buffer()
		{
			Reset();
		}

	private:
		Buffer()
			: Data(nullptr)
			, Size(0)
			, OwnsMemory(false)
		{
		}

		Buffer(const Buffer& other)
			: Data(nullptr)
			, Size(0)
			, OwnsMemory(false)
		{
			ReAllocate(other.Size);
			MemCpy(Data, other.Data, other.Size);
		}

		Buffer(Buffer&& other) noexcept
			: Data(other.Data)
			, Size(other.Size)
			, OwnsMemory(other.OwnsMemory)
		{
			other.Data = nullptr;
			other.Size = 0;
			other.OwnsMemory = false;
		}

		Buffer(void* data, uint64 size)
			: Data(data)
			, Size(size)
			, OwnsMemory(false)
		{
		}

		Buffer(uint64 size)
			: Size(size)
			, OwnsMemory(true)
		{
			Data = anew uint8[Size];
		}

	public:
		Buffer& operator=(const Buffer& other)
		{
			if (Size < other.Size)
			{
				ReAllocate(other.Size);
			}
			
			MemCpy(Data, other.Data, other.Size);

			return *this;
		}

		Buffer& operator=(Buffer&& other) noexcept
		{
			Reset();

			Data       = other.Data;
			Size       = other.Size;
			OwnsMemory = other.OwnsMemory;
			
			other.Data = nullptr;
			other.Size = 0;
			other.OwnsMemory = false;

			return *this;
		}

	public:
		void Reset()
		{
			if (OwnsMemory && Data)
			{
				adelete[] Data;
			}
			Data = nullptr;
			Size = 0;
			OwnsMemory = false;
		}

		void ReAllocate(uint64 newSize)
		{
			Reset();
			Data = anew uint8[newSize];
			Size = newSize;
			OwnsMemory = true;
		}

	public:
		void* Data;
		uint64 Size;
		bool OwnsMemory;

		friend class StreamingBuffer;
		friend class Ref<Buffer>;
	};

	class StreamingBuffer : public RefCounted
	{
	public:
		static Ref<StreamingBuffer> Create()
		{
			return Ref<StreamingBuffer>::Create();
		}

		static Ref<StreamingBuffer> Create(const StreamingBuffer& other)
		{
			return Ref<StreamingBuffer>::Create(other);
		}

		static Ref<StreamingBuffer> Create(StreamingBuffer&& other)
		{
			return Ref<StreamingBuffer>::Create(astl::move(other));
		}

		static Ref<StreamingBuffer> Create(void* data, uint64 size)
		{
			return Ref<StreamingBuffer>::Create(data, size);
		}

		static Ref<StreamingBuffer> Create(uint64 size)
		{
			return Ref<StreamingBuffer>::Create(size);
		}

	public:
		~StreamingBuffer()
		{
		}

	private:
		StreamingBuffer()
			: m_Buffer()
			, Offset(0)
		{
		}

		StreamingBuffer(const StreamingBuffer& other)
			: m_Buffer(other.m_Buffer)
			, Offset(other.Offset)
		{
		}

		StreamingBuffer(StreamingBuffer&& other) noexcept
			: m_Buffer(other.m_Buffer)
			, Offset(other.Offset)
		{
			other.Offset = 0;
		}

		StreamingBuffer(void* data, uint64 size)
			: m_Buffer(data, size)
			, Offset(0)
		{
		}

		StreamingBuffer(uint64 size)
			: m_Buffer(size)
			, Offset(0)
		{
		}

	public:
		StreamingBuffer& operator=(const StreamingBuffer& other)
		{
			m_Buffer = other.m_Buffer;
			Offset   = other.Offset;
			return *this;
		}

		StreamingBuffer& operator=(StreamingBuffer&& other) noexcept
		{
			m_Buffer     = astl::move(other.m_Buffer);
			Offset       = astl::move(other.Offset);
			other.Offset = 0;
			return *this;
		}

	public:
		Buffer& GetBuffer() { return m_Buffer; }
		const Buffer& GetBuffer() const { return m_Buffer; }

		void* Data() { return m_Buffer.Data; }
		const void* Data() const { return m_Buffer.Data; }

		template<typename T>
		T* AllocateRaw()
		{
			AE_CORE_ASSERT(Offset + sizeof(T) <= m_Buffer.Size); // Buffer out of data
			uint8* memory = (uint8*)m_Buffer.Data + Offset;
			Offset += sizeof(T);
			return (T*)memory;
		}

		template<typename T, typename... Args>
		T* Allocate(Args&&... args)
		{
			T* object = AllocateRaw<T>();
			new (object) T(astl::forward<Args>(args)...);
			return object;
		}

		void* Allocate(uint64 size)
		{
			void* memory = (uint8*)m_Buffer.Data + Offset;
			Offset += size;
			return memory;
		}

		template<typename T>
		T* AllocateAs(uint64 count)
		{
			void* memory = (uint8*)m_Buffer.Data + Offset;
			Offset += (count * sizeof(T));
			return (T*)memory;
		}

		void Reset()
		{
			Offset = 0;
		}

		void Free()
		{
			m_Buffer.Reset();
			Offset = 0;
		}

	public:
		uint64 Offset;

	private:
		Buffer m_Buffer;

		friend class Ref<StreamingBuffer>;
	};

}