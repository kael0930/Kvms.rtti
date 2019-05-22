#ifndef KvPlatformdefs_h__
#define KvPlatformdefs_h__

#include "KvGlobal.h"

#include <tchar.h>
#include <io.h>
#include <direct.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>
//#include <qt_windows.h>
#include <limits.h>

#if !defined(_WIN32_WINNT) || (_WIN32_WINNT-0 < 0x0500)
typedef enum {
	NameUnknown = 0,
	NameFullyQualifiedDN = 1,
	NameSamCompatible = 2,
	NameDisplay = 3,
	NameUniqueId = 6,
	NameCanonical = 7,
	NameUserPrincipal = 8,
	NameCanonicalEx = 9,
	NameServicePrincipal = 10,
	NameDnsDomain = 12
} EXTENDED_NAME_FORMAT, *PEXTENDED_NAME_FORMAT;
#endif

#define KV_FS_FAT
#ifdef KV_LARGEFILE_SUPPORT
#define KV_STATBUF		struct _stati64		// non-ANSI defs
#define KV_STATBUF4TSTAT	struct _stati64		// non-ANSI defs
#define KV_STAT			::_stati64
#define KV_FSTAT		::_fstati64
#else
#define KV_STATBUF		struct _stat		// non-ANSI defs
#define KV_STATBUF4TSTAT	struct _stat		// non-ANSI defs
#define KV_STAT			::_stat
#define KV_FSTAT		::_fstat
#endif
#define KV_STAT_REG		_S_IFREG
#define KV_STAT_DIR		_S_IFDIR
#define KV_STAT_MASK		_S_IFMT
#if defined(_S_IFLNK)
#  define KV_STAT_LNK		_S_IFLNK
#endif
#define KV_FILENO		_fileno
#define KV_OPEN			::_open
#define KV_CLOSE		::_close
#ifdef KV_LARGEFILE_SUPPORT
#define KV_LSEEK		::_lseeki64
#ifndef UNICODE
#define KV_TSTAT		::_stati64
#else
#define KV_TSTAT		::_wstati64
#endif
#else
#define KV_LSEEK		::_lseek
#ifndef UNICODE
#define KV_TSTAT		::_stat
#else
#define KV_TSTAT		::_wstat
#endif
#endif
#define KV_READ			::_read
#define KV_WRITE		::_write
#define KV_ACCESS		::_access
#define KV_GETCWD		::_getcwd
#define KV_CHDIR		::_chdir
#define KV_MKDIR		::_mkdir
#define KV_RMDIR		::_rmdir
#define KV_OPEN_LARGEFILE       0
#define KV_OPEN_RDONLY		_O_RDONLY
#define KV_OPEN_WRONLY		_O_WRONLY
#define KV_OPEN_RDWR		_O_RDWR
#define KV_OPEN_CREAT		_O_CREAT
#define KV_OPEN_TRUNC		_O_TRUNC
#define KV_OPEN_APPEND		_O_APPEND
#if defined(O_TEXT)
# define KV_OPEN_TEXT		_O_TEXT
# define KV_OPEN_BINARY		_O_BINARY
#endif

#define KV_FPOS_T               fpos_t
#define KV_OFF_T                long

#define KV_FOPEN                ::fopen
#define KV_FSEEK                ::fseek
#define KV_FTELL                ::ftell
#define KV_FGETPOS              ::fgetpos
#define KV_FSETPOS              ::fsetpos

#ifdef KV_LARGEFILE_SUPPORT
#undef KV_FSEEK
#undef KV_FTELL
#undef KV_OFF_T

#define KV_FSEEK                ::fseeko64
#define KV_FTELL                ::ftello64
#define KV_OFF_T                off64_t
#endif

#define KV_SIGNAL_ARGS		int

#define KV_VSNPRINTF		::_vsnprintf
#define KV_SNPRINTF		::_snprintf

# define F_OK	0
# define X_OK	1
# define W_OK	2
# define R_OK	4

#endif // KvPlatformdefs_h__
