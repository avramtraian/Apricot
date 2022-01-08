// Part of Apricot Engine. 2022-2022.

#include "aepch.h"

///////////////////////////////////////////////////////////////////////////////
/////////////----  APRICOT FILESYSTEM WINDOWS IMPLEMENTATION  ----/////////////
///////////////----  THIS FILE IS COMPILED ONLY ON WINDOWS  ----///////////////
///////////////////////////////////////////////////////////////////////////////

#ifdef AE_PLATFORM_WINDOWS

#include "Apricot/Filesystem/Filesystem.h"

#include <Windows.h>

namespace Apricot { namespace Filesystem {

	void* GInvalidNativeHandle = INVALID_HANDLE_VALUE;

	struct FilesystemData
	{
		// Holds the engine current working directory.
		Path CurrentWorkingDirectory;

		// Their sizes are never shrunken down.
		TArray<Path, AEC_FILESYSTEM_PATH_BUFFERS_COUNT> BufferPaths;
	};
	static FilesystemData* s_Filesystem = nullptr;

	void Init()
	{
		s_Filesystem = (FilesystemData*)Memory::Allocate(sizeof(FilesystemData), Memory::AllocTag::Filesystem);
		Memory::PlacementNew<FilesystemData>(s_Filesystem);

		char16 buffer[1024];
		GetCurrentDirectory(sizeof(buffer) / sizeof(char16), buffer);
		s_Filesystem->CurrentWorkingDirectory = buffer;
	}

	void Destroy()
	{
		s_Filesystem->~FilesystemData();
		Memory::Free(s_Filesystem, sizeof(FilesystemData), Memory::AllocTag::Filesystem);
	}

	bool File::IsValid() const
	{
		return
			m_NativeHandle != INVALID_HANDLE_VALUE &&
			m_Status != Status::FailedRead &&
			m_Status != Status::FailedWrite &&
			m_Status != Status::Unknown;
	}

	bool File::IsGood() const
	{
		return (IsValid() && (m_BufferMark < Buffer().Size()));
	}

	void File::Close()
	{
		if (m_NativeHandle != INVALID_HANDLE_VALUE)
		{
			CloseHandle((HANDLE)m_NativeHandle);
			m_NativeHandle = INVALID_HANDLE_VALUE;
		}
		m_Status = Status::Closed;
	}

	APRICOT_API const Path& GetWorkingDirectory()
	{
		return s_Filesystem->CurrentWorkingDirectory;
	}

	APRICOT_API void SetWorkingDirectory(const char16* path)
	{
		SetCurrentDirectory(path);
		s_Filesystem->CurrentWorkingDirectory = path;
	}

	APRICOT_API bool Exists(const char16* filepath)
	{
		DWORD fileAttrib = GetFileAttributes(filepath);
		return (fileAttrib != INVALID_FILE_ATTRIBUTES);
	}

	APRICOT_API bool IsDirectory(const char16* filepath)
	{
		DWORD fileAttrib = GetFileAttributes(filepath);
		return (fileAttrib != INVALID_FILE_ATTRIBUTES && (fileAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}

	APRICOT_API uint64 SyncReadFile(const char16* filepath, File& outFile, uint64 flags /*= 0*/)
	{
		// Ensure the file isn't already used.
		if (outFile.FileStatus() == File::Status::OpenRead || outFile.FileStatus() == File::Status::OpenWrite)
		{
			AE_CORE_WARN("File already used. Closing the handle. Trying to open file '{}'.", filepath);
			outFile.Close();
		}

		// Sets the file flags.
		DWORD desiredAccess = GENERIC_READ;
		DWORD shareMode = 0;
		DWORD creationDisposition = 0;
		DWORD flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;

		if (flags & FILE_MODE_ALWAYS_CREATE)
		{
			creationDisposition = OPEN_ALWAYS;

			s_Filesystem->BufferPaths[0] = filepath;
			auto& filepathParent = (s_Filesystem->BufferPaths[1] = s_Filesystem->BufferPaths[0].Parent());
			if (!Exists(filepathParent))
			{
				CreateDirectories(filepathParent);
			}
		}
		else
		{
			creationDisposition = OPEN_EXISTING;
		}

		// Creates the file handle.
		outFile.SetNativeHandle((void*)CreateFile(filepath, desiredAccess, shareMode, NULL, creationDisposition, flagsAndAttributes, NULL));

		if (outFile.NativeHandle() == INVALID_HANDLE_VALUE)
		{
			switch (GetLastError())
			{
				case ERROR_FILE_NOT_FOUND:
				{
					AE_CORE_ERROR("Failed to open file '{}' for reading. ERROR_CODE_FILE_NOT_FOUND", filepath);
					outFile.SetFileStatus(File::Status::FailedRead);
					return ERROR_CODE_FAILURE | ERROR_CODE_FILE_NOT_FOUND;
				}
				case ERROR_SHARING_VIOLATION:
				{
					AE_CORE_ERROR("Failed to open file '{}' for reading. ERROR_CODE_FILE_IN_USE", filepath);
					return ERROR_CODE_FAILURE | ERROR_CODE_FILE_IN_USE;
				}
				default:
				{
					AE_CORE_ERROR("Failed to open file '{}' for reading. ERROR_CODE_UNKNOWN", filepath);
					outFile.SetFileStatus(File::Status::FailedRead);
					return ERROR_CODE_FAILURE | ERROR_CODE_UNKNOWN;
				}
			}
		}

		DWORD bytesRead;
		LARGE_INTEGER fileSize;
		BOOL result;

		// Gets the file' size in bytes.
		result = GetFileSizeEx((HANDLE)outFile.NativeHandle(), &fileSize);
		if (!result)
		{
			AE_CORE_ERROR("Failed to get file size: '{}'. ERROR_CODE_UNKNOWN", filepath);
			return ERROR_CODE_FAILURE | ERROR_CODE_UNKNOWN;
		}

		// If the file is empty, we will return as success.
		if (fileSize.QuadPart == 0)
		{
			outFile.SetFileStatus(File::Status::OpenRead);
			return ERROR_CODE_SUCCESS;
		}

		// Resize the buffer to the file's size.
		outFile.Buffer().Resize(fileSize.QuadPart);

		// Fill the buffer.
		result = ReadFile((HANDLE)outFile.NativeHandle(), outFile.Buffer().Data(), (DWORD)fileSize.QuadPart + 1, &bytesRead, NULL);

#ifdef AE_ENABLE_FILESYSTEM_ERROR_CHECK
		// Error checking.
		if (!result)
		{
			switch (GetLastError())
			{
				case ERROR_INVALID_USER_BUFFER:
				{
					AE_CORE_ERROR("Failed to read file '{}'. ERROR_CODE_BUFFER_FAILURE", filepath);
					return ERROR_CODE_FAILURE | ERROR_CODE_BUFFER_FAILURE;
				}
				case ERROR_NOT_ENOUGH_QUOTA:
				{
					AE_CORE_ERROR("Failed to read file '{}'. ERROR_CODE_BUFFER_FAILURE", filepath);
					return ERROR_CODE_FAILURE | ERROR_CODE_BUFFER_FAILURE;
				}
				default:
				{
					AE_CORE_ERROR("Failed to read file '{}'. ERROR_CODE_UNKNOWN", filepath);
					return ERROR_CODE_FAILURE | ERROR_CODE_UNKNOWN;
				}
			}
		}

		if (outFile.Buffer().Size() < bytesRead)
		{
			AE_CORE_WARN("Only {} bytes were read out of {} bytes.", (uint64)bytesRead, outFile.Buffer().Size());
		}
#endif

		outFile.SetFileStatus(File::Status::OpenRead);
		return ERROR_CODE_SUCCESS;
	}

	APRICOT_API uint64 SyncWriteFile(const char16* filepath, File& file, uint64 flags /*= 0*/)
	{
		// Ensure the file isn't already used.
		if (file.FileStatus() == File::Status::OpenRead || file.FileStatus() == File::Status::OpenWrite)
		{
			AE_CORE_WARN("File already used. Closing the handle. Trying to open file '{}'.", filepath);
			file.Close();
		}

		// Sets the file flags.
		DWORD desiredAccess = 0;
		DWORD shareMode = 0;
		DWORD creationDisposition = 0;
		DWORD flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;

		if (flags & FILE_MODE_CLEAR_ON_OPEN)
		{
			if (flags & FILE_MODE_OPEN_IF_EXISTS)
			{
				creationDisposition = TRUNCATE_EXISTING;
			}
			else
			{
				creationDisposition = CREATE_ALWAYS;

				s_Filesystem->BufferPaths[0] = filepath;
				auto& filepathParent = (s_Filesystem->BufferPaths[1] = s_Filesystem->BufferPaths[0].Parent());
				if (!Exists(filepathParent))
				{
					CreateDirectories(filepathParent);
				}
			}
			desiredAccess |= FILE_WRITE_DATA;
		}
		else if (flags & FILE_MODE_OPEN_IF_EXISTS)
		{
			creationDisposition = OPEN_EXISTING;
			desiredAccess |= FILE_APPEND_DATA;
		}
		else
		{
			creationDisposition = OPEN_ALWAYS;
			desiredAccess |= FILE_APPEND_DATA;

			s_Filesystem->BufferPaths[0] = filepath;
			auto& filepathParent = (s_Filesystem->BufferPaths[1] = s_Filesystem->BufferPaths[0].Parent());
			if (!Exists(filepathParent))
			{
				CreateDirectories(filepathParent);
			}
		}

		// Creates the file handle.
		file.SetNativeHandle((void*)CreateFile(filepath, desiredAccess, shareMode, NULL, creationDisposition, flagsAndAttributes, NULL));

		if ((HANDLE)file.NativeHandle() == INVALID_HANDLE_VALUE)
		{
			switch (GetLastError())
			{
				case ERROR_FILE_NOT_FOUND:
				{
					AE_CORE_ERROR("Failed to open file '{}' for writing. ERROR_CODE_FILE_NOT_FOUND", filepath);
					file.SetFileStatus(File::Status::FailedWrite);
					return ERROR_CODE_FAILURE | ERROR_CODE_FILE_NOT_FOUND;
				}
				case ERROR_SHARING_VIOLATION:
				{
					AE_CORE_ERROR("Failed to open file '{}' for writing. ERROR_CODE_FILE_IN_USE", filepath);
					return ERROR_CODE_FAILURE | ERROR_CODE_FILE_IN_USE;
				}
				default:
				{
					AE_CORE_ERROR("Failed to open file '{}' for writing. ERROR_CODE_UNKNOWN", filepath);
					file.SetFileStatus(File::Status::FailedWrite);
					return ERROR_CODE_FAILURE | ERROR_CODE_UNKNOWN;
				}
			}
		}

		BOOL result;
		DWORD bytesWritten;
		// Writes the buffer to the file.
		result = WriteFile((HANDLE)file.NativeHandle(), file.Buffer().Data(), (DWORD)file.Buffer().Size(), &bytesWritten, NULL);

#ifdef AE_ENABLE_FILESYSTEM_ERROR_CHECK
		// Error checking.
		if (!result)
		{
			switch (GetLastError())
			{
				case ERROR_NOT_ENOUGH_QUOTA:
				{
					AE_CORE_ERROR("Failed to write file '{}'. ERROR_CODE_BUFFER_FAILURE", filepath);
					file.SetFileStatus(File::Status::FailedWrite);
					return ERROR_CODE_FAILURE | ERROR_CODE_BUFFER_FAILURE;
				}
				default:
				{
					AE_CORE_ERROR("Failed to write file '{}'. ERROR_CODE_UNKNOWN", filepath);
					file.SetFileStatus(File::Status::FailedWrite);
					return ERROR_CODE_FAILURE | ERROR_CODE_UNKNOWN;
				}
			}
		}

		if (file.Buffer().Size() > bytesWritten)
		{
			AE_CORE_WARN("Only {} bytes were written out of {} bytes.", (uint64)bytesWritten, file.Buffer().Size());
		}
#endif

		file.SetFileStatus(File::Status::OpenWrite);
		return ERROR_CODE_SUCCESS;
	}

	APRICOT_API uint64 SyncCopyFile(const char16* existingFile, const char16* destination, uint64 flags /*= 0*/)
	{
		BOOL bFailIfExists = FALSE;
		if (flags & FILE_MODE_SHOULD_NOT_EXIST)
		{
			bFailIfExists = TRUE;
		}

		s_Filesystem->BufferPaths[0] = destination;
		auto& parentDestPath = (s_Filesystem->BufferPaths[1] = s_Filesystem->BufferPaths[0].Parent());
		if (!Exists(parentDestPath))
		{
			uint64 resultCode = CreateDirectories(parentDestPath);
#ifdef AE_ENABLE_FILESYSTEM_ERROR_CHECK
			if (!AE_FILESYSTEM_CHECK(resultCode))
			{
				AE_CORE_ERROR("Failed to copy file '{}' to '{}'. ERROR_CODE_DEPENDENCY_FAILURE", existingFile, destination);
				return ERROR_CODE_FAILURE | ERROR_CODE_DEPENDENCY_FAILURE;
			}
#endif
		}

		BOOL result = CopyFile(existingFile, destination, bFailIfExists);

#ifdef AE_ENABLE_FILESYSTEM_ERROR_CHECK
		if (!result)
		{
			switch (GetLastError())
			{
				case ERROR_FILE_NOT_FOUND:
				{
					AE_CORE_ERROR("Failed to copy file '{}' to '{}'. ERROR_CODE_FILE_NOT_FOUND", existingFile, destination);
					return ERROR_CODE_FAILURE | ERROR_CODE_FILE_NOT_FOUND;
				}
				case ERROR_SHARING_VIOLATION:
				{
					AE_CORE_ERROR("Failed to copy file '{}' to '{}'. ERROR_CODE_FILE_IN_USE", existingFile, destination);
					return ERROR_CODE_FAILURE | ERROR_CODE_FILE_IN_USE;
				}
				case ERROR_FILE_EXISTS:
				{
					AE_CORE_ERROR("Failed to copy file '{}' to '{}'. ERROR_CODE_FILE_EXISTS", existingFile, destination);
					return ERROR_CODE_FAILURE | ERROR_CODE_FILE_EXISTS;
				}
				case ERROR_ACCESS_DENIED:
				{
					AE_CORE_ERROR("Failed to copy file '{}' to '{}'. ERROR_CODE_ACCESS_DENIED", existingFile, destination);
					return ERROR_CODE_FAILURE | ERROR_CODE_ACCESS_DENIED;
				}
				default:
				{
					AE_CORE_ERROR("Failed to copy file '{}' to '{}'. ERROR_CODE_UNKNOWN", existingFile, destination);
					return ERROR_CODE_FAILURE | ERROR_CODE_UNKNOWN;
				}
			}
		}
#endif

		return ERROR_CODE_SUCCESS;
	}

	APRICOT_API uint64 CreateDirectories(const char16* path, uint64 flags /*= 0*/)
	{
		if (Exists(path))
		{
			return ERROR_CODE_SUCCESS;
		}

		auto& absolutePath = (s_Filesystem->BufferPaths[0] = GetAbsolutePath(path));

		char16 buffer[1024];
		// 3 -> C:/
		for (uint64 index = 3; index < absolutePath.Size(); index++)
		{
			if (absolutePath[index] == '/' || absolutePath[index] == '\\')
			{
				Memory::Copy(buffer, absolutePath.Data(), index * sizeof(char16));
				buffer[index] = 0;

				if (Exists(buffer))
				{
					continue;
				}

				BOOL result = CreateDirectory(buffer, NULL);

#ifdef AE_ENABLE_FILESYSTEM_ERROR_CHECK
				if (!result)
				{
					switch (GetLastError())
					{
						case ERROR_PATH_NOT_FOUND:
						{
							AE_CORE_ERROR("Failed to create subdirectory '{}' on disk! ERROR_CODE_PATH_NOT_FOUND", buffer);
							return ERROR_CODE_FAILURE | ERROR_CODE_PATH_NOT_FOUND;
						}
						default:
						{
							AE_CORE_ERROR("Failed to create subdirectory '{}' on disk! ERROR_CODE_UNKNOWN", buffer);
							return ERROR_CODE_FAILURE | ERROR_CODE_UNKNOWN;
						}
					}
				}
#endif
			}
		}

		BOOL result = CreateDirectory(absolutePath.Data(), NULL);

#ifdef AE_ENABLE_FILESYSTEM_ERROR_CHECK
		if (!result)
		{
			switch (GetLastError())
			{
				case ERROR_PATH_NOT_FOUND:
				{
					AE_CORE_ERROR("Failed to create subdirectory '{}' on disk! ERROR_CODE_PATH_NOT_FOUND", buffer);
					return ERROR_CODE_FAILURE | ERROR_CODE_PATH_NOT_FOUND;
				}
				default:
				{
					AE_CORE_ERROR("Failed to create subdirectory '{}' on disk! ERROR_CODE_UNKNOWN", buffer);
					return ERROR_CODE_FAILURE | ERROR_CODE_UNKNOWN;
				}
			}
		}
#endif

		return ERROR_CODE_SUCCESS;
	}

	APRICOT_API Path GetAbsolutePath(const Path& path)
	{
		uint64 backwardsDirs = 0;
		uint64 pathOffset = 0;
		for (int64 index = 0; index < (int64)path.Size() - 2; index++)
		{
			if (path[index] == '.' && path[index + 1] == '.' && (path[index + 2] == '/' || path[index + 2] == '\\'))
			{
				backwardsDirs++;
				pathOffset = index + 3;
			}
		}

		auto& cwd = s_Filesystem->CurrentWorkingDirectory;

		if (backwardsDirs == 0)
		{
			Path absolute(cwd.Size() + path.Size() - 1);
			absolute.Resize(absolute.Capacity());
			
			Memory::Copy(absolute.Data(), cwd.Data(), (cwd.Size() - 1) * sizeof(char16));
			absolute[cwd.Size() - 1] = '/';
			Memory::Copy(absolute.Data() + cwd.Size(), path.Data(), path.Size() * sizeof(char16));

			return absolute;
		}
		else
		{
			uint64 cwdCopySize = cwd.Size();
			for (int64 index = cwd.Size() - 1; index >= 0; index--)
			{
				if (cwd[index] == '/' || cwd[index] == '\\')
				{
					backwardsDirs--;
					if (backwardsDirs == 0)
					{
						break;
					}
				}
				cwdCopySize--;
			}

			Path absolute(cwdCopySize + path.Size() - pathOffset);
			absolute.Resize(absolute.Capacity());

			Memory::Copy(absolute.Data(), cwd.Data(), cwdCopySize * sizeof(char16));
			Memory::Copy(absolute.Data() + cwdCopySize, path.Data() + pathOffset, (path.Size() - pathOffset) * sizeof(char16));

			return absolute;
		}
	}

} }

#endif