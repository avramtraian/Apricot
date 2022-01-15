// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"
#include "Path.h"

#define AE_FILESYSTEM_CHECK(Code) ((uint64)(Code) & ::Apricot::Filesystem::ERROR_CODE_SUCCESS)

namespace Apricot { namespace Filesystem {
	
	// This is defined in the platform filesystem layer.
	extern void* GInvalidNativeHandle;
	
	enum ErrorCode : uint64
	{
		ERROR_CODE_NONE               = 0,
						              
		ERROR_CODE_SUCCESS            = 1 << 0,
		ERROR_CODE_FAILURE            = 1 << 1,
								      
		ERROR_CODE_FILE_NOT_FOUND     = 1 << 2,
		ERROR_CODE_PATH_NOT_FOUND     = 1 << 3,
		ERROR_CODE_FILE_IN_USE        = 1 << 4,
		ERROR_CODE_BUFFER_FAILURE     = 1 << 5,
		ERROR_CODE_FILE_EXISTS        = 1 << 6,
		ERROR_CODE_ACCESS_DENIED      = 1 << 7,
		ERROR_CODE_DEPENDENCY_FAILURE = 1 << 8,
		ERROR_CODE_UNKNOWN            = 1 << 31,
	};

	enum FileMode : uint64
	{
		FILE_MODE_NONE             = 0,
		
		// Currently unused.
		FILE_MODE_READ             = 1 << 0,
		// Currently unused.
		FILE_MODE_WRITE            = 1 << 1,
						           
		// Currently unused.
		FILE_MODE_TEXT             = 1 << 2,
		// Currently unused.
		FILE_MODE_BINARY           = 1 << 3,
		
		// Always creates a file on disk. If it already exists, this flag does nothing.
		FILE_MODE_ALWAYS_CREATE    = 1 << 4,

		// Clears the file content when opened.
		FILE_MODE_CLEAR_ON_OPEN    = 1 << 5,

		// Opens a file only if it exists on disk.
		FILE_MODE_OPEN_IF_EXISTS   = 1 << 6,

		// The function fails if the file already exists.
		FILE_MODE_SHOULD_NOT_EXIST = 1 << 0,
	};

	/*
	* Holds the file handle and buffer.
	* Used in reading and writing files.
	*/
	class APRICOT_API File
	{
	public:
		enum class Status
		{
			// Unknown = bad
			Unknown = 0,

			// File can be used to read from or write to.
			Closed,

			// File is open and the native handle is alive.
			OpenRead, OpenWrite,

			// There was an error trying to read or write a file.
			FailedRead, FailedWrite
		};

	public:
		File() = default;

		File (const File&) = delete;

		File(File&& other) noexcept
		{
			m_Buffer = std::move(other.m_Buffer);
			m_Status = std::move(other.m_Status);
			m_NativeHandle = std::move(other.m_NativeHandle);

			other.m_Status = Status::Closed;
			other.m_NativeHandle = GInvalidNativeHandle;
		}

		~File()
		{
			Close();
		}

	public:
		File& operator=(const File&) = delete;

		File& operator=(File&& other) noexcept
		{
			m_Buffer = std::move(other.m_Buffer);
			m_Status = std::move(other.m_Status);
			m_NativeHandle = std::move(other.m_NativeHandle);

			other.m_Status = Status::Closed;
			other.m_NativeHandle = GInvalidNativeHandle;

			return *this;
		}

	public:
		const TVector<uint8>& Buffer() const { return m_Buffer; }
		TVector<uint8>& Buffer() { return m_Buffer; }

		Status FileStatus() const { return m_Status; }
		void SetFileStatus(Status status) { m_Status = status; }

		void* NativeHandle() const { return m_NativeHandle; }
		void SetNativeHandle(void* nativeHandle) { m_NativeHandle = nativeHandle; }

		/*
		* The handle and status are valid.
		*/
		bool IsValid() const;

		/*
		* The handle and status are valid.
		* The buffer mark still has data. The buffer marker is not at the top.
		*/
		bool IsGood() const;

		/*
		* Frees the native handle. Automatically called in the deconstructor.
		*/
		void Close();

		/*
		* Pushes data to the end of the buffer.
		* 
		* @param data Pointer to the data.
		* @param size The size (in bytes) of the data.
		*/
		void Push(void* data, uint64 size)
		{
			uint64 oldSize = m_Buffer.Size();
			m_Buffer.Resize(oldSize + size);
			Memory::Copy(m_Buffer.Data() + oldSize, data, size);
		}

		/*
		* Pops the data from the end of the buffer to the start.
		* 
		* @param size The size (in bytes) of data that will be popped.
		*/
		void Pop(uint64 size)
		{
			AE_CORE_VERIFY_RETURN(size <= m_Buffer.Size(), AE_VOID, "");
			m_Buffer.Resize(m_Buffer.Size() - size);
		}

		/*
		* Reads the data from the buffer. From the beginning of the buffer to the end.
		* 
		* @params size The size (in bytes) of data to read. Used to increment the buffer marker.
		* 
		* @returns Pointer to the data. It's recommended to be considered the size of the passed argument, to avoid reading the same data multiple times.
		*/
		uint8* Read(uint64 size)
		{
			AE_CORE_VERIFY_RETURN(m_BufferMark < m_Buffer.Size(), nullptr, "");
			uint8* data = m_Buffer.Data() + m_BufferMark;
			m_BufferMark += size;
			return data;
		}

	private:
		/*
		* The buffer of the file.
		* It will be filled when a file is read.
		* It will be written to the file.
		*/
		TVector<uint8> m_Buffer;

		/*
		* Status of the file.
		*/
		Status m_Status = Status::Closed;

		/*
		* The platform specific file handle.
		* Managed entirely by the Filesystem.
		*/
		void* m_NativeHandle = GInvalidNativeHandle;

		/*
		* The buffer marker. Used to read the file's data buffer.
		*/
		uint64 m_BufferMark = 0;
	};

	//////////// WorkingDirectory ////////////

	/*
	* 
	*/
	APRICOT_API const Path& GetWorkingDirectory();

	/*
	* 
	*/
	APRICOT_API void SetWorkingDirectory(const char16* path);

	inline void SetWorkingDirectory(const Path& path)
	{
		SetWorkingDirectory(path.c_str());
	}

	template<uint64 S>
	inline void SetWorkingDirectory(const TStackPath<S>& path)
	{
		SetWorkingDirectory(path.c_str());
	}

	//////////// Exists ////////////

	/*
	* Checks if a file (or directory) exists on the disk.
	*/
	APRICOT_API bool Exists(const char16* filepath);

	inline bool Exists(const Path& filepath)
	{
		return Exists(filepath.c_str());
	}

	template<uint64 S>
	inline bool Exists(const TStackPath<S>& filepath)
	{
		return Exists(filepath.c_str());
	}

	//////////// IsDirectory ////////////

	/*
	* Checks if a disk object is a directory.
	*/
	APRICOT_API bool IsDirectory(const char16* filepath);

	inline bool IsDirectory(const Path& filepath)
	{
		return IsDirectory(filepath.c_str());
	}

	template<uint64 S>
	inline bool IsDirectory(const TStackPath<S>& filepath)
	{
		return IsDirectory(filepath.c_str());
	}

	//////////// SyncReadFile ////////////

	/*
	* Synchronous operation of reading from a disk file.
	* 
	* @param filepath Filepath of the desired disk file.
	* @param outFile File structure to be filled.
	* @param flags Bit-combination of FileMode flags.
	* 
	* Available flags:
	*	- FILE_MODE_ALWAYS_CREATE
	* 
	* @returns Bit-combination of ErrorCode flags.
	* 
	* Returns error codes:
	*	- ERROR_CODE_FILE_NOT_FOUND if the file doesn't exists on disk and
	*		the flag FILE_MODE_ALWAYS_CREATE isn't present.
	*   - ERROR_CODE_BUFFER_FAILURE If there are any errors with the buffer.
	*   - ERROR_CODE_UNKNOWN
	*/
	APRICOT_API uint64 SyncReadFile(const char16* filepath, File& outFile, uint64 flags = 0);

	inline uint64 SyncReadFile(const Path& filepath, File& outFile, uint64 flags = 0)
	{
		return SyncReadFile(filepath.c_str(), outFile, flags);
	}

	template<uint64 S>
	inline uint64 SyncReadFile(const TStackPath<S>& filepath, File& outFile, uint64 flags = 0)
	{
		return SyncReadFile(filepath.c_str());
	}

	//////////// SyncWriteFile ////////////

	/*
	* Synchronous operation of writing to a disk file.
	*
	* @param filepath Filepath of the desired disk file.
	* @param file File structure. Contains the buffer to be written.
	* @param flags Bit-combination of FileMode flags.
	* 
	* Available flags:
	*	- FILE_MODE_CLEAR_ON_OPEN
	*	- FILE_MODE_OPEN_IF_EXISTS
	* 
	* @returns Bit-combination of ErrorCode flags.
	*
	* Returns error codes:
	*	- ERROR_CODE_FILE_NOT_FOUND If the file doesn't exists on disk and
	*		the flag FILE_MODE_OPEN_IF_EXISTS is present.
	*   - ERROR_CODE_BUFFER_FAILURE If there are any errors with the buffer.
	*   - ERROR_CODE_UNKNOWN
	*/
	APRICOT_API uint64 SyncWriteFile(const char16* filepath, File& file, uint64 flags = 0);

	inline uint64 SyncWriteFile(const Path& filepath, File& file, uint64 flags = 0)
	{
		return SyncWriteFile(filepath.c_str(), file, flags);
	}

	template<uint64 S>
	inline uint64 SyncWriteFile(const TStackPath<S>& filepath, File& file, uint64 flags = 0)
	{
		return SyncWriteFile(filepath.c_str(), file, flags);
	}

	//////////// SyncCopyFile ////////////

	/*
	* Copies a file.
	* 
	* @param existingFile The path to the file that you want to be copied.
	* @param destination The destination path of the new file.
	* @param flags Bit-combination of FileMode flags.
	* 
	* Available flags:
	*	- FILE_MODE_SHOULD_NOT_EXIST
	* 
	* @returns Bit-combination of ErrorCode flags.
	* 
	* Returns error codes:
	*	- ERROR_CODE_FILE_NOT_FOUND The file to be copied doesn't exist.
	*	- ERROR_CODE_FILE_IN_USE The file to be copied is used in another process.
	*	- ERROR_CODE_FILE_EXISTS The destination represents a valid file. This error-code is returned only when
	*		FILE_MODE_SHOULD_NOT_EXIST is present.
	*	- ERROR_CODE_DEPENDENCY_FAILURE One of the function called internally failed.
	*	- ERROR_CODE_ACCESS_DENIED The access is denied.
	*	- ERROR_CODE_UNKNOWN
	*/
	APRICOT_API uint64 SyncCopyFile(const char16* existingFile, const char16* destination, uint64 flags = 0);

	inline uint64 SyncCopyFile(const Path& existingFile, const Path& destination, uint64 flags = 0)
	{
		return SyncCopyFile(existingFile.c_str(), destination.c_str());
	}

	template<uint64 S>
	inline uint64 SyncCopyFile(const TStackPath<S>& existingFile, const TStackPath<S>& destination, uint64 flags = 0)
	{
		return SyncCopyFile(existingFile.c_str(), destination.c_str());
	}

	//////////// CreateDirectories ////////////

	/*
	* Creates a directory on disk. It will create all required subdirectories.
	* 
	* @param path The desired directory path.
	* @param flags Bit-combination of FileMode flags.
	* 
	* Available flags:
	*	- 
	* 
	* @returns Bit-combination of ErrorCode flags.
	*
	* Returns error codes:
	*	- ERROR_CODE_PATH_NOT_FOUND Specified directory path is invalid.
	*	- ERROR_CODE_UNKNOWN
	*/
	APRICOT_API uint64 CreateDirectories(const char16* path, uint64 flags = 0);

	inline uint64 CreateDirectories(const Path& path, uint64 flags = 0)
	{
		return CreateDirectories(path.c_str(), flags);
	}

	template<uint64 S>
	inline uint64 CreateDirectories(const TStackPath<S>& path, uint64 flags = 0)
	{
		return CreateDirectories(path.c_str(), flags);
	}

	//////////// Paths ////////////

	/*
	* Returns the path relative to the disk drive.
	*/
	APRICOT_API Path GetAbsolutePath(const Path& path);

	//////////// Init/Destroy ////////////

	/*
	* Init/Destroy commands.
	*/
	void Init();
	void Destroy();

} }