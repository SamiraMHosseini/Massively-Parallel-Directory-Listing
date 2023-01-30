//--------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef FILE_SLOW_H
#define FILE_SLOW_H

// This file assumes Framework.h is included in executable

// Make the assumption of c-char strings, not UNICODE

class FileSlow
{
public:

	typedef void *Handle;

	enum class Mode
	{
		READ = 0x7A000000,
		WRITE,
		READ_WRITE
	};

	enum class Location
	{
		BEGIN = 0x7B000000,
		CURRENT,
		END
	};

	enum class Error
	{
		SUCCESS = 0x7C000000,
		OPEN_FAIL,
		CLOSE_FAIL,
		WRITE_FAIL,
		READ_FAIL,
		SEEK_FAIL,
		TELL_FAIL,
		FLUSH_FAIL,
		UNDEFINED
	};

public:

	static FileSlow::Error Magic(unsigned int &num);
	static FileSlow::Error Open(FileSlow::Handle &fh, const char *const fileName, FileSlow::Mode mode) noexcept;
	static FileSlow::Error Close(FileSlow::Handle &fh) noexcept;
	static FileSlow::Error Write(FileSlow::Handle fh, const void *const buffer, const DWORD inSize) noexcept;
	static FileSlow::Error Read(FileSlow::Handle fh, void *const _buffer, const DWORD _size) noexcept;
	static FileSlow::Error Seek(FileSlow::Handle fh, FileSlow::Location location, int offset) noexcept;
	static FileSlow::Error Tell(FileSlow::Handle fh, DWORD &offset) noexcept;
	static FileSlow::Error Flush(FileSlow::Handle fh) noexcept;
	static bool IsHandleValid(FileSlow::Handle fh) noexcept;

private:
	static DWORD privGetFileDesiredAccess(FileSlow::Mode mode) noexcept;
	static DWORD privGetSeek(FileSlow::Location location) noexcept;
};



#endif

// ---  End of File ---------------
