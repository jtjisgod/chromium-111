/* Copyright 2012 The Chromium Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/* From private/pp_private_font_charset.idl,
 *   modified Fri Sep 07 12:50:35 2012.
 */

#ifndef PPAPI_C_PRIVATE_PP_PRIVATE_FONT_CHARSET_H_
#define PPAPI_C_PRIVATE_PP_PRIVATE_FONT_CHARSET_H_

#include "ppapi/c/pp_macros.h"

/**
 * @file
 */


/**
 * @addtogroup Enums
 * @{
 */
typedef enum {
  PP_PRIVATEFONTCHARSET_ANSI = 0,
  PP_PRIVATEFONTCHARSET_DEFAULT = 1,
  PP_PRIVATEFONTCHARSET_SYMBOL = 2,
  PP_PRIVATEFONTCHARSET_MAC = 77,
  PP_PRIVATEFONTCHARSET_SHIFTJIS = 128,
  PP_PRIVATEFONTCHARSET_HANGUL = 129,
  PP_PRIVATEFONTCHARSET_JOHAB = 130,
  PP_PRIVATEFONTCHARSET_GB2312 = 134,
  PP_PRIVATEFONTCHARSET_CHINESEBIG5 = 136,
  PP_PRIVATEFONTCHARSET_GREEK = 161,
  PP_PRIVATEFONTCHARSET_TURKISH = 162,
  PP_PRIVATEFONTCHARSET_VIETNAMESE = 163,
  PP_PRIVATEFONTCHARSET_HEBREW = 177,
  PP_PRIVATEFONTCHARSET_ARABIC = 178,
  PP_PRIVATEFONTCHARSET_BALTIC = 186,
  PP_PRIVATEFONTCHARSET_RUSSIAN = 204,
  PP_PRIVATEFONTCHARSET_THAI = 222,
  PP_PRIVATEFONTCHARSET_EASTEUROPE = 238,
  PP_PRIVATEFONTCHARSET_OEM = 255,
  PP_PRIVATEFONTCHARSET_LAST = PP_PRIVATEFONTCHARSET_OEM
} PP_PrivateFontCharset;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_PrivateFontCharset, 4);
/**
 * @}
 */

#endif  /* PPAPI_C_PRIVATE_PP_PRIVATE_FONT_CHARSET_H_ */

