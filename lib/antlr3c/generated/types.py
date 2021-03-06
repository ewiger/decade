''' 
TYPES

This is an auto-generated ctypes file from ANTLR3 C runtime headers. Note that
editing this file is not smart! For more details check genclib.py

It should be possible to redefine things in __init__.py if necessary (right
after imports section).

wbr, yy
'''

TYPES = \
{'ANTLR3_ARBORETUM': ['struct ANTLR3_ARBORETUM_struct'],
 'ANTLR3_BASE_RECOGNIZER': ['struct ANTLR3_BASE_RECOGNIZER_struct'],
 'ANTLR3_BASE_TREE': ['struct ANTLR3_BASE_TREE_struct'],
 'ANTLR3_BITSET': ['struct ANTLR3_BITSET_struct'],
 'ANTLR3_BITSET_LIST': ['struct ANTLR3_BITSET_LIST_struct'],
 'ANTLR3_BITWORD': ['uint64_t'],
 'ANTLR3_BOOLEAN': ['uint32_t'],
 'ANTLR3_CHAR': ['int32_t'],
 'ANTLR3_COMMON_TOKEN': ['struct ANTLR3_COMMON_TOKEN_struct'],
 'ANTLR3_COMMON_TOKEN_STREAM': ['struct ANTLR3_COMMON_TOKEN_STREAM_struct'],
 'ANTLR3_COMMON_TREE': ['struct ANTLR3_COMMON_TREE_struct'],
 'ANTLR3_COMMON_TREE_ADAPTOR': ['struct ANTLR3_COMMON_TREE_ADAPTOR_struct'],
 'ANTLR3_COMMON_TREE_NODE_STREAM': ['struct ANTLR3_COMMON_TREE_NODE_STREAM_struct'],
 'ANTLR3_DEBUG_EVENT_LISTENER': ['struct ANTLR3_DEBUG_EVENT_LISTENER_struct'],
 'ANTLR3_EXCEPTION': ['struct ANTLR3_EXCEPTION_struct'],
 'ANTLR3_FDSC': ['FILE', '*'],
 'ANTLR3_FSTAT_STRUCT': ['struct stat'],
 'ANTLR3_HASH_BUCKET': ['struct ANTLR3_HASH_BUCKET_struct'],
 'ANTLR3_HASH_ENTRY': ['struct ANTLR3_HASH_ENTRY_struct'],
 'ANTLR3_HASH_ENUM': ['struct ANTLR3_HASH_ENUM_struct'],
 'ANTLR3_HASH_KEY': ['struct ANTLR3_HASH_KEY_struct'],
 'ANTLR3_HASH_TABLE': ['struct ANTLR3_HASH_TABLE_struct'],
 'ANTLR3_INPUT_STREAM': ['struct ANTLR3_INPUT_STREAM_struct'],
 'ANTLR3_INT16': ['int16_t'],
 'ANTLR3_INT32': ['int32_t'],
 'ANTLR3_INT64': ['int64_t'],
 'ANTLR3_INT8': ['int8_t'],
 'ANTLR3_INTKEY': ['ANTLR3_UINT32'],
 'ANTLR3_INT_STREAM': ['struct ANTLR3_INT_STREAM_struct'],
 'ANTLR3_INT_TRIE': ['struct ANTLR3_INT_TRIE_struct'],
 'ANTLR3_INT_TRIE_NODE': ['struct ANTLR3_INT_TRIE_NODE_struct'],
 'ANTLR3_LEXER': ['struct ANTLR3_LEXER_struct'],
 'ANTLR3_LEX_STATE': ['struct ANTLR3_LEX_STATE_struct'],
 'ANTLR3_LIST': ['struct ANTLR3_LIST_struct'],
 'ANTLR3_MARKER': ['ANTLR3_INT32'],
 'ANTLR3_PARSER': ['struct ANTLR3_PARSER_struct'],
 'ANTLR3_PARSE_TREE': ['struct ANTLR3_PARSE_TREE_struct'],
 'ANTLR3_RECOGNIZER_SHARED_STATE': ['struct ANTLR3_RECOGNIZER_SHARED_STATE_struct'],
 'ANTLR3_REWRITE_RULE_ELEMENT_STREAM': ['struct ANTLR3_REWRITE_RULE_ELEMENT_STREAM_struct'],
 'ANTLR3_REWRITE_RULE_NODE_STREAM': ['struct ANTLR3_REWRITE_RULE_ELEMENT_STREAM_struct'],
 'ANTLR3_REWRITE_RULE_SUBTREE_STREAM': ['struct ANTLR3_REWRITE_RULE_ELEMENT_STREAM_struct'],
 'ANTLR3_REWRITE_RULE_TOKEN_STREAM': ['struct ANTLR3_REWRITE_RULE_ELEMENT_STREAM_struct'],
 'ANTLR3_SALENT': ['size_t'],
 'ANTLR3_SOCKADDRT': ['struct sockaddr_in'],
 'ANTLR3_STACK': ['struct ANTLR3_STACK_struct'],
 'ANTLR3_STRING': ['struct ANTLR3_STRING_struct'],
 'ANTLR3_STRING_FACTORY': ['struct ANTLR3_STRING_FACTORY_struct'],
 'ANTLR3_TOKEN_FACTORY': ['struct ANTLR3_TOKEN_FACTORY_struct'],
 'ANTLR3_TOKEN_SOURCE': ['struct ANTLR3_TOKEN_SOURCE_struct'],
 'ANTLR3_TOKEN_STREAM': ['struct ANTLR3_TOKEN_STREAM_struct'],
 'ANTLR3_TOPO': ['struct ANTLR3_TOPO_struct'],
 'ANTLR3_TREE_ADAPTOR': ['struct ANTLR3_BASE_TREE_ADAPTOR_struct'],
 'ANTLR3_TREE_NODE_STREAM': ['struct ANTLR3_TREE_NODE_STREAM_struct'],
 'ANTLR3_TREE_PARSER': ['struct ANTLR3_TREE_PARSER_struct'],
 'ANTLR3_TREE_WALK_STATE': ['struct ANTLR3_TREE_WALK_STATE_struct'],
 'ANTLR3_TRIE_ENTRY': ['struct ANTLR3_TRIE_ENTRY_struct'],
 'ANTLR3_UCHAR': ['uint32_t'],
 'ANTLR3_UINT16': ['uint16_t'],
 'ANTLR3_UINT32': ['uint32_t'],
 'ANTLR3_UINT64': ['uint64_t'],
 'ANTLR3_UINT8': ['uint8_t'],
 'ANTLR3_VECTOR': ['struct ANTLR3_VECTOR_struct'],
 'ANTLR3_VECTOR_ELEMENT': ['struct ANTLR3_VECTOR_ELEMENT_struct'],
 'ANTLR3_VECTOR_FACTORY': ['struct ANTLR3_VECTOR_FACTORY_struct'],
 'CDFA_SPECIAL_FUNC': ['ANTLR3_INT32',
                       ((None, ['void', '*'], None),
                        (None, ['pANTLR3_BASE_RECOGNIZER'], None),
                        (None, ['pANTLR3_INT_STREAM'], None),
                        (None,
                         ['struct ANTLR3_CYCLIC_DFA_struct', '*'],
                         None),
                        (None, ['ANTLR3_INT32'], None)),
                       '*'],
 'ConversionFlags': ['enum anonEnum3'],
 'ConversionResult': ['enum anonEnum2'],
 'SOCKET': ['int'],
 'UTF16': ['ANTLR3_UINT16'],
 'UTF32': ['ANTLR3_UINT32'],
 'UTF8': ['ANTLR3_UINT8'],
 'enum anonEnum0': ('enum', 'anonEnum0'),
 'enum anonEnum1': ('enum', 'anonEnum1'),
 'enum anonEnum2': ('enum', 'anonEnum2'),
 'enum anonEnum3': ('enum', 'anonEnum3'),
 'pANTLR3_ARBORETUM': ['struct ANTLR3_ARBORETUM_struct', '*'],
 'pANTLR3_BASE_RECOGNIZER': ['struct ANTLR3_BASE_RECOGNIZER_struct', '*'],
 'pANTLR3_BASE_TREE': ['struct ANTLR3_BASE_TREE_struct', '*'],
 'pANTLR3_BASE_TREE_ADAPTOR': ['struct ANTLR3_BASE_TREE_ADAPTOR_struct', '*'],
 'pANTLR3_BITSET': ['struct ANTLR3_BITSET_struct', '*'],
 'pANTLR3_BITSET_LIST': ['struct ANTLR3_BITSET_LIST_struct', '*'],
 'pANTLR3_BITWORD': ['uint64_t', '*'],
 'pANTLR3_BOOLEAN': ['uint32_t', '*'],
 'pANTLR3_CHAR': ['int32_t', '*'],
 'pANTLR3_COMMON_TOKEN': ['struct ANTLR3_COMMON_TOKEN_struct', '*'],
 'pANTLR3_COMMON_TOKEN_STREAM': ['struct ANTLR3_COMMON_TOKEN_STREAM_struct',
                                 '*'],
 'pANTLR3_COMMON_TREE': ['struct ANTLR3_COMMON_TREE_struct', '*'],
 'pANTLR3_COMMON_TREE_ADAPTOR': ['struct ANTLR3_COMMON_TREE_ADAPTOR_struct',
                                 '*'],
 'pANTLR3_COMMON_TREE_NODE_STREAM': ['struct ANTLR3_COMMON_TREE_NODE_STREAM_struct',
                                     '*'],
 'pANTLR3_CYCLIC_DFA': ['struct ANTLR3_CYCLIC_DFA_struct', '*'],
 'pANTLR3_DEBUG_EVENT_LISTENER': ['struct ANTLR3_DEBUG_EVENT_LISTENER_struct',
                                  '*'],
 'pANTLR3_EXCEPTION': ['struct ANTLR3_EXCEPTION_struct', '*'],
 'pANTLR3_HASH_BUCKET': ['struct ANTLR3_HASH_BUCKET_struct', '*'],
 'pANTLR3_HASH_ENTRY': ['struct ANTLR3_HASH_ENTRY_struct', '*'],
 'pANTLR3_HASH_ENUM': ['struct ANTLR3_HASH_ENUM_struct', '*'],
 'pANTLR3_HASH_KEY': ['struct ANTLR3_HASH_KEY_struct', '*'],
 'pANTLR3_HASH_TABLE': ['struct ANTLR3_HASH_TABLE_struct', '*'],
 'pANTLR3_INPUT_STREAM': ['struct ANTLR3_INPUT_STREAM_struct', '*'],
 'pANTLR3_INT16': ['int16_t', '*'],
 'pANTLR3_INT32': ['int32_t', '*'],
 'pANTLR3_INT64': ['int64_t', '*'],
 'pANTLR3_INT8': ['int8_t', '*'],
 'pANTLR3_INT_STREAM': ['struct ANTLR3_INT_STREAM_struct', '*'],
 'pANTLR3_INT_TRIE': ['struct ANTLR3_INT_TRIE_struct', '*'],
 'pANTLR3_INT_TRIE_NODE': ['struct ANTLR3_INT_TRIE_NODE_struct', '*'],
 'pANTLR3_LEXER': ['struct ANTLR3_LEXER_struct', '*'],
 'pANTLR3_LEX_STATE': ['struct ANTLR3_LEX_STATE_struct', '*'],
 'pANTLR3_LIST': ['struct ANTLR3_LIST_struct', '*'],
 'pANTLR3_PARSER': ['struct ANTLR3_PARSER_struct', '*'],
 'pANTLR3_PARSE_TREE': ['struct ANTLR3_PARSE_TREE_struct', '*'],
 'pANTLR3_RECOGNIZER_SHARED_STATE': ['struct ANTLR3_RECOGNIZER_SHARED_STATE_struct',
                                     '*'],
 'pANTLR3_REWRITE_RULE_ELEMENT_STREAM': ['struct ANTLR3_REWRITE_RULE_ELEMENT_STREAM_struct',
                                         '*'],
 'pANTLR3_REWRITE_RULE_NODE_STREAM': ['struct ANTLR3_REWRITE_RULE_ELEMENT_STREAM_struct',
                                      '*'],
 'pANTLR3_REWRITE_RULE_SUBTREE_STREAM': ['struct ANTLR3_REWRITE_RULE_ELEMENT_STREAM_struct',
                                         '*'],
 'pANTLR3_REWRITE_RULE_TOKEN_STREAM': ['struct ANTLR3_REWRITE_RULE_ELEMENT_STREAM_struct',
                                       '*'],
 'pANTLR3_SOCKADDRC': ['struct sockaddr', '*'],
 'pANTLR3_SOCKADDRT': ['struct sockaddr_in', '*'],
 'pANTLR3_STACK': ['struct ANTLR3_STACK_struct', '*'],
 'pANTLR3_STRING': ['struct ANTLR3_STRING_struct', '*'],
 'pANTLR3_STRING_FACTORY': ['struct ANTLR3_STRING_FACTORY_struct', '*'],
 'pANTLR3_TOKEN_FACTORY': ['struct ANTLR3_TOKEN_FACTORY_struct', '*'],
 'pANTLR3_TOKEN_SOURCE': ['struct ANTLR3_TOKEN_SOURCE_struct', '*'],
 'pANTLR3_TOKEN_STREAM': ['struct ANTLR3_TOKEN_STREAM_struct', '*'],
 'pANTLR3_TOPO': ['struct ANTLR3_TOPO_struct', '*'],
 'pANTLR3_TREE_ADAPTOR': ['struct ANTLR3_BASE_TREE_ADAPTOR_struct', '*'],
 'pANTLR3_TREE_NODE_STREAM': ['struct ANTLR3_TREE_NODE_STREAM_struct', '*'],
 'pANTLR3_TREE_PARSER': ['struct ANTLR3_TREE_PARSER_struct', '*'],
 'pANTLR3_TREE_WALK_STATE': ['struct ANTLR3_TREE_WALK_STATE_struct', '*'],
 'pANTLR3_TRIE_ENTRY': ['struct ANTLR3_TRIE_ENTRY_struct', '*'],
 'pANTLR3_UCHAR': ['uint32_t', '*'],
 'pANTLR3_UINT16': ['uint16_t', '*'],
 'pANTLR3_UINT32': ['uint32_t', '*'],
 'pANTLR3_UINT64': ['uint64_t', '*'],
 'pANTLR3_UINT8': ['uint8_t', '*'],
 'pANTLR3_VECTOR': ['struct ANTLR3_VECTOR_struct', '*'],
 'pANTLR3_VECTOR_ELEMENT': ['struct ANTLR3_VECTOR_ELEMENT_struct', '*'],
 'pANTLR3_VECTOR_FACTORY': ['struct ANTLR3_VECTOR_FACTORY_struct', '*'],
 'struct ANTLR3_ARBORETUM_struct': ('struct', 'ANTLR3_ARBORETUM_struct'),
 'struct ANTLR3_BASE_RECOGNIZER_struct': ('struct',
                                          'ANTLR3_BASE_RECOGNIZER_struct'),
 'struct ANTLR3_BASE_TREE_ADAPTOR_struct': ('struct',
                                            'ANTLR3_BASE_TREE_ADAPTOR_struct'),
 'struct ANTLR3_BASE_TREE_struct': ('struct', 'ANTLR3_BASE_TREE_struct'),
 'struct ANTLR3_BITSET_LIST_struct': ('struct', 'ANTLR3_BITSET_LIST_struct'),
 'struct ANTLR3_BITSET_struct': ('struct', 'ANTLR3_BITSET_struct'),
 'struct ANTLR3_COMMON_TOKEN_STREAM_struct': ('struct',
                                              'ANTLR3_COMMON_TOKEN_STREAM_struct'),
 'struct ANTLR3_COMMON_TOKEN_struct': ('struct', 'ANTLR3_COMMON_TOKEN_struct'),
 'struct ANTLR3_COMMON_TREE_ADAPTOR_struct': ('struct',
                                              'ANTLR3_COMMON_TREE_ADAPTOR_struct'),
 'struct ANTLR3_COMMON_TREE_NODE_STREAM_struct': ('struct',
                                                  'ANTLR3_COMMON_TREE_NODE_STREAM_struct'),
 'struct ANTLR3_COMMON_TREE_struct': ('struct', 'ANTLR3_COMMON_TREE_struct'),
 'struct ANTLR3_CYCLIC_DFA_struct': ('struct', 'ANTLR3_CYCLIC_DFA_struct'),
 'struct ANTLR3_DEBUG_EVENT_LISTENER_struct': ('struct',
                                               'ANTLR3_DEBUG_EVENT_LISTENER_struct'),
 'struct ANTLR3_EXCEPTION_struct': ('struct', 'ANTLR3_EXCEPTION_struct'),
 'struct ANTLR3_HASH_BUCKET_struct': ('struct', 'ANTLR3_HASH_BUCKET_struct'),
 'struct ANTLR3_HASH_ENTRY_struct': ('struct', 'ANTLR3_HASH_ENTRY_struct'),
 'struct ANTLR3_HASH_ENUM_struct': ('struct', 'ANTLR3_HASH_ENUM_struct'),
 'struct ANTLR3_HASH_KEY_struct': ('struct', 'ANTLR3_HASH_KEY_struct'),
 'struct ANTLR3_HASH_TABLE_struct': ('struct', 'ANTLR3_HASH_TABLE_struct'),
 'struct ANTLR3_INPUT_STREAM_struct': ('struct', 'ANTLR3_INPUT_STREAM_struct'),
 'struct ANTLR3_INT_STREAM_struct': ('struct', 'ANTLR3_INT_STREAM_struct'),
 'struct ANTLR3_INT_TRIE_NODE_struct': ('struct',
                                        'ANTLR3_INT_TRIE_NODE_struct'),
 'struct ANTLR3_INT_TRIE_struct': ('struct', 'ANTLR3_INT_TRIE_struct'),
 'struct ANTLR3_LEXER_struct': ('struct', 'ANTLR3_LEXER_struct'),
 'struct ANTLR3_LEX_STATE_struct': ('struct', 'ANTLR3_LEX_STATE_struct'),
 'struct ANTLR3_LIST_struct': ('struct', 'ANTLR3_LIST_struct'),
 'struct ANTLR3_PARSER_struct': ('struct', 'ANTLR3_PARSER_struct'),
 'struct ANTLR3_PARSE_TREE_struct': ('struct', 'ANTLR3_PARSE_TREE_struct'),
 'struct ANTLR3_RECOGNIZER_SHARED_STATE_struct': ('struct',
                                                  'ANTLR3_RECOGNIZER_SHARED_STATE_struct'),
 'struct ANTLR3_REWRITE_RULE_ELEMENT_STREAM_struct': ('struct',
                                                      'ANTLR3_REWRITE_RULE_ELEMENT_STREAM_struct'),
 'struct ANTLR3_STACK_struct': ('struct', 'ANTLR3_STACK_struct'),
 'struct ANTLR3_STRING_FACTORY_struct': ('struct',
                                         'ANTLR3_STRING_FACTORY_struct'),
 'struct ANTLR3_STRING_struct': ('struct', 'ANTLR3_STRING_struct'),
 'struct ANTLR3_TOKEN_FACTORY_struct': ('struct',
                                        'ANTLR3_TOKEN_FACTORY_struct'),
 'struct ANTLR3_TOKEN_SOURCE_struct': ('struct', 'ANTLR3_TOKEN_SOURCE_struct'),
 'struct ANTLR3_TOKEN_STREAM_struct': ('struct', 'ANTLR3_TOKEN_STREAM_struct'),
 'struct ANTLR3_TOPO_struct': ('struct', 'ANTLR3_TOPO_struct'),
 'struct ANTLR3_TREE_NODE_STREAM_struct': ('struct',
                                           'ANTLR3_TREE_NODE_STREAM_struct'),
 'struct ANTLR3_TREE_PARSER_struct': ('struct', 'ANTLR3_TREE_PARSER_struct'),
 'struct ANTLR3_TREE_WALK_STATE_struct': ('struct',
                                          'ANTLR3_TREE_WALK_STATE_struct'),
 'struct ANTLR3_TRIE_ENTRY_struct': ('struct', 'ANTLR3_TRIE_ENTRY_struct'),
 'struct ANTLR3_VECTOR_ELEMENT_struct': ('struct',
                                         'ANTLR3_VECTOR_ELEMENT_struct'),
 'struct ANTLR3_VECTOR_FACTORY_struct': ('struct',
                                         'ANTLR3_VECTOR_FACTORY_struct'),
 'struct ANTLR3_VECTOR_struct': ('struct', 'ANTLR3_VECTOR_struct'),
 'struct sockaddr': ('struct', 'sockaddr'),
 'struct sockaddr_in': ('struct', 'sockaddr_in'),
 'struct stat': ('struct', 'stat'),
 'union anon_union0': ('union', 'anon_union0'),
 'union anon_union1': ('union', 'anon_union1'),
 'union anon_union2': ('union', 'anon_union2'),
 'union anon_union3': ('union', 'anon_union3'),
 'union anon_union4': ('union', 'anon_union4'),
 'union anon_union5': ('union', 'anon_union5')}
