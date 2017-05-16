/* This file is part of crutils.
 *
 * crutils is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * crutils is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with crutils. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Copyright (C)
 *  2013-2017 Alexander Haase <ahaase@alexhaase.de>
 */

/* This header file is for internal macros, which are dependend on the used
 * compiler. Macros used in this header won't be public and may change from time
 * to time, dependend on the needs of this project. */

#ifndef CODEREADER_PRIVATE_ATTRIBUTES_H
#define CODEREADER_PRIVATE_ATTRIBUTES_H


#include "config.h" // HAVE_FOPENCOOKIE
#ifdef HAVE_FOPENCOOKIE
#include <stdio.h>
#endif


/** \brief Mark function as internal.
 *
 * \details This macro will be used to mark functions as internal functions,
 *  which will be hidden for global exports.
 */
#ifdef __GNUC__
#define CODEREADER_INTERNAL __attribute__((visibility("hidden")))
#else
#define CODEREADER_INTERNAL
#endif


/** \brief Message prefix.
 *
 * \details This prefix should be used for any output e.g. for error messages,
 *  so the user can identify the origin of this message.
 */
#define CODEREADER_MESSAGE_PREFIX "[codereader] "


/* The definitions of codereader_read depends on the used function to create the
 * stream, as they use different signatures for the internal functions. */
#ifdef HAVE_FOPENCOOKIE
#define CODEREADER_READ_RETURN_TYPE ssize_t
#define CODEREADER_READ_SIZE_TYPE size_t
#else
#define CODEREADER_READ_RETURN_TYPE int
#define CODEREADER_READ_SIZE_TYPE int
#endif


/** \brief Set this token, if AddressSanitizer is activated.
 */
#if __SANITIZE_ADDRESS__
#define CODEREADER_SANITIZE_ADDRESS
#else
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
#define CODEREADER_SANITIZE_ADDRESS
#endif
#endif
#endif


CODEREADER_READ_RETURN_TYPE codereader_read(void *cookie, char *buf,
                                            CODEREADER_READ_SIZE_TYPE size);
int codereader_close(void *cookie);


#endif
