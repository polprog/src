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

#include <atf-c.h>
#include <errno.h>
#include <limits.h>
#include <locale.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <uchar.h>

ATF_TC(c16rtomb_single);
ATF_TC_HEAD(c16rtomb_single, tc)
{
    atf_tc_set_md_var(tc, "descr", "Test c16rtomb on a single multibyte char");
}
/* This test checks c16rtomb behavior in case of valid conversion. setlocale 
   is called to make sure we're operating in an UTF-8 environment */
ATF_TC_BODY(c16rtomb_single, tc)
{
    int rc;
    mbstate_t st;
    char target[MB_LEN_MAX] = {0x00};
    char expected[sizeof(target)] = {0xE2, 0x9A, 0xA0, 0x00};
    char16_t warning_char = u'\u26A0'; //U+26A0 Warning Sign

    setlocale(LC_ALL, "en_US.UTF-8");
    rc = c16rtomb(target, warning_char, NULL);
    ATF_REQUIRE(rc == 3);
    ATF_REQUIRE(memcmp(target, expected, MB_LEN_MAX) == 0);
}

ATF_TP_ADD_TCS(tp)
{
    ATF_TP_ADD_TC(tp, c16rtomb_single);
    return atf_no_error();
}
