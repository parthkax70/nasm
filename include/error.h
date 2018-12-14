/* ----------------------------------------------------------------------- *
 *
 *   Copyright 1996-2018 The NASM Authors - All Rights Reserved
 *   See the file AUTHORS included with the NASM distribution for
 *   the specific copyright holders.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following
 *   conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *     CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *     INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *     MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 *     CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *     SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *     NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *     CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *     OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 *     EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ----------------------------------------------------------------------- */

/*
 * Error reporting functions for the assembler
 */

#ifndef NASM_ERROR_H
#define NASM_ERROR_H 1

#include "compiler.h"

/*
 * File pointer for error messages
 */
extern FILE *error_file;        /* Error file descriptor */

/*
 * Typedef for the severity field
 */
typedef uint32_t errflags;

/*
 * An error reporting function should look like this.
 */
void printf_func(2, 3) nasm_error(errflags severity, const char *fmt, ...);
void printf_func(1, 2) nasm_debug(const char *fmt, ...);
void printf_func(2, 3) nasm_debugf(errflags flags, const char *fmt, ...);
void printf_func(1, 2) nasm_note(const char *fmt, ...);
void printf_func(2, 3) nasm_notef(errflags flags, const char *fmt, ...);
void printf_func(1, 2) nasm_warn(const char *fmt, ...);
void printf_func(2, 3) nasm_warnf(errflags flags, const char *fmt, ...);
void printf_func(1, 2) nasm_nonfatal(const char *fmt, ...);
void printf_func(2, 3) nasm_nonfatalf(errflags flags, const char *fmt, ...);
fatal_func printf_func(1, 2) nasm_fatal(const char *fmt, ...);
fatal_func printf_func(2, 3) nasm_fatalf(errflags flags, const char *fmt, ...);
fatal_func printf_func(1, 2) nasm_panic(const char *fmt, ...);
fatal_func printf_func(2, 3) nasm_panicf(errflags flags, const char *fmt, ...);
fatal_func nasm_panic_from_macro(const char *file, int line);
#define panic() nasm_panic_from_macro(__FILE__, __LINE__);

typedef void (*vefunc) (errflags severity, const char *fmt, va_list ap);
extern vefunc nasm_verror;

static inline vefunc nasm_set_verror(vefunc ve)
{
	vefunc old_verror = nasm_verror;
	nasm_verror = ve;
	return old_verror;
}

/*
 * These are the error severity codes which get passed as the first
 * argument to an efunc.
 */
#define ERR_DEBUG		0x00000000	/* put out debugging message */
#define ERR_NOTE		0x00000001	/* additional error information */
#define ERR_WARNING		0x00000002	/* warn only: no further action */
#define ERR_NONFATAL		0x00000003	/* terminate assembly after phase */
#define ERR_FATAL		0x00000006	/* instantly fatal: exit with error */
#define ERR_PANIC		0x00000007	/* internal error: panic instantly
						 * and dump core for reference */
#define ERR_MASK		0x00000007	/* mask off the above codes */
#define ERR_NOFILE		0x00000010	/* don't give source file name/line */
#define ERR_HERE		0x00000020      /* point to a specific source location */
#define ERR_USAGE		0x00000040	/* print a usage message */
#define ERR_PASS1		0x00000080	/* only print this error on pass one */
#define ERR_PASS2		0x00000100	/* only print this error on pass one */

#define ERR_NO_SEVERITY		0x00000200	/* suppress printing severity */
#define ERR_PP_PRECOND		0x00000400	/* for preprocessor use */
#define ERR_PP_LISTMACRO	0x00000800	/* from preproc->error_list_macros() */

/*
 * These codes define specific types of suppressible warning.
 * They are assumed to occupy the most significant bits of the
 * severity code.
 */
#define WARN_SHR		12              /* how far to shift right */
#define WARN_IDX(x)     	(((errflags)(x)) >> WARN_SHR)
#define WARN_MASK		((~(errflags)0) << WARN_SHR)

/* This is a bitmask */
#define WARN_ST_ENABLED		1 /* Warning is currently enabled */
#define WARN_ST_ERROR		2 /* Treat this warning as an error */

/* Possible initial state for warnings */
#define WARN_INIT_OFF		0
#define WARN_INIT_ON		WARN_ST_ENABLED
#define WARN_INIT_ERR		(WARN_ST_ENABLED|WARN_ST_ERROR)

/* Process a warning option or directive */
bool set_warning_status(const char *value);

/* Should be included from within error.h only */
#include "warnings.h"

#endif /* NASM_ERROR_H */
