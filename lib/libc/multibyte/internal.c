/*	$NetBSD$	*/

/*-
 * Copyright (c) 2019 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by 
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "internal.h"

#define C(x) ( x<2 ? -1 : (signed) ( R(0x80,0xc0) | x ) )
#define D(x) C((  x+16))
#define E(x) ( (  x==0 ? R(0xa0,0xc0) :  \
                  x==0xd ? R(0x80,0xa0) : \
                 R(0x80,0xc0) ) \
             | ( R(0x80,0xc0) >> 6 ) \
             | x )
#define F(x) ( (  x>=5 ? 0 : \
                  x==0 ? R(0x90,0xc0) : \
                  x==4 ? R(0x80,0x90) : \
                 R(0x80,0xc0) ) \
             | ( R(0x80,0xc0) >> 6 ) \
             | ( R(0x80,0xc0) >> 12 ) \
             | x )

const uint32_t bittab[] = {
	              C(0x2),C(0x3),C(0x4),C(0x5),C(0x6),C(0x7),
	C(0x8),C(0x9),C(0xa),C(0xb),C(0xc),C(0xd),C(0xe),C(0xf),
	D(0x0),D(0x1),D(0x2),D(0x3),D(0x4),D(0x5),D(0x6),D(0x7),
	D(0x8),D(0x9),D(0xa),D(0xb),D(0xc),D(0xd),D(0xe),D(0xf),
	E(0x0),E(0x1),E(0x2),E(0x3),E(0x4),E(0x5),E(0x6),E(0x7),
	E(0x8),E(0x9),E(0xa),E(0xb),E(0xc),E(0xd),E(0xe),E(0xf),
	F(0x0),F(0x1),F(0x2),F(0x3),F(0x4)
};
