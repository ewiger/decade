''' 
FNMACROS

This is an auto-generated ctypes file from ANTLR3 C runtime headers. Note that
editing this file is not smart! For more details check genclib.py

It should be possible to redefine things in __init__.py if necessary (right
after imports section).

wbr, yy
'''

FNMACROS = \
{'ANTLR3_CALLOC': ('calloc  (%s, (size_t)(%s))', [0, 1]),
 'ANTLR3_FREE': ('free    ((void *)(%s))', [0]),
 'ANTLR3_FUNC_PTR': ('(void *)((ANTLR3_UINT32)(%s))', [0]),
 'ANTLR3_MALLOC': ('malloc  ((size_t)(%s))', [0]),
 'ANTLR3_MEMCPY': ('memcpy((void *)(%s), (const void *)(%s), (size_t)(%s))',
                   [0, 1, 2]),
 'ANTLR3_MEMMOVE': ('memmove((void *)(%s), (const void *)(%s), (size_t)(%s))',
                    [0, 1, 2]),
 'ANTLR3_MEMSET': ('memset((void *)(%s), (int)(%s), (size_t)(%s))', [0, 1, 2]),
 'ANTLR3_REALLOC': ('realloc ((void *)(%s), (size_t)(%s))', [0, 1]),
 'ANTLR3_STRDUP': ('(pANTLR3_UINT8)(strdup  ((const char *)(%s)))', [0]),
 'ANTLR3_UINT32_CAST': ('(ANTLR3_UINT32)(%s)', [0]),
 'ANTLR3_UINT64_CAST': ('(ANTLR3_UINT64)((ANTLR3_UINT32)(%s))', [0]),
 'ANTLR3_UINT64_LIT': ('%s##ULL', [0]),
 'UNI_MAX_BMP': ('0x0000FFFF', []),
 'UNI_MAX_LEGAL_UTF32': ('0x0010FFFF', []),
 'UNI_MAX_UTF16': ('0x0010FFFF', []),
 'UNI_MAX_UTF32': ('0x7FFFFFFF', []),
 'UNI_REPLACEMENT_CHAR': ('0x0000FFFD', []),
 'UNI_SUR_HIGH_END': ('0xDBFF', []),
 'UNI_SUR_HIGH_START': ('0xD800', []),
 'UNI_SUR_LOW_END': ('0xDFFF', []),
 'UNI_SUR_LOW_START': ('0xDC00', [])}
