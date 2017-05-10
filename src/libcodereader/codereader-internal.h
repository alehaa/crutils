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


#endif
