''' 
FUNCTIONS

This is an auto-generated ctypes file from ANTLR3 C runtime headers. Note that
editing this file is not smart! For more details check genclib.py

It should be possible to redefine things in __init__.py if necessary (right
after imports section).

wbr, yy
'''

FUNCTIONS = \
{'ANTLR3_TREE_ADAPTORDebugNew': (['pANTLR3_BASE_TREE_ADAPTOR'],
                                 (('strFactory',
                                   ['pANTLR3_STRING_FACTORY'],
                                   None),
                                  ('debugger',
                                   ['pANTLR3_DEBUG_EVENT_LISTENER'],
                                   None))),
 'ANTLR3_TREE_ADAPTORNew': (['pANTLR3_BASE_TREE_ADAPTOR'],
                            (('strFactory',
                              ['pANTLR3_STRING_FACTORY'],
                              None),)),
 'ConvertUTF16toUTF32': (['ConversionResult'],
                         (('sourceStart', ['UTF16', '**'], None),
                          ('sourceEnd', ['UTF16', '*'], None),
                          ('targetStart', ['UTF32', '**'], None),
                          ('targetEnd', ['UTF32', '*'], None),
                          ('flags', ['ConversionFlags'], None))),
 'ConvertUTF16toUTF8': (['ConversionResult'],
                        (('sourceStart', ['UTF16', '**'], None),
                         ('sourceEnd', ['UTF16', '*'], None),
                         ('targetStart', ['UTF8', '**'], None),
                         ('targetEnd', ['UTF8', '*'], None),
                         ('flags', ['ConversionFlags'], None))),
 'ConvertUTF32toUTF16': (['ConversionResult'],
                         (('sourceStart', ['UTF32', '**'], None),
                          ('sourceEnd', ['UTF32', '*'], None),
                          ('targetStart', ['UTF16', '**'], None),
                          ('targetEnd', ['UTF16', '*'], None),
                          ('flags', ['ConversionFlags'], None))),
 'ConvertUTF32toUTF8': (['ConversionResult'],
                        (('sourceStart', ['UTF32', '**'], None),
                         ('sourceEnd', ['UTF32', '*'], None),
                         ('targetStart', ['UTF8', '**'], None),
                         ('targetEnd', ['UTF8', '*'], None),
                         ('flags', ['ConversionFlags'], None))),
 'ConvertUTF8toUTF16': (['ConversionResult'],
                        (('sourceStart', ['UTF8', '**'], None),
                         ('sourceEnd', ['UTF8', '*'], None),
                         ('targetStart', ['UTF16', '**'], None),
                         ('targetEnd', ['UTF16', '*'], None),
                         ('flags', ['ConversionFlags'], None))),
 'ConvertUTF8toUTF32': (['ConversionResult'],
                        (('sourceStart', ['UTF8', '**'], None),
                         ('sourceEnd', ['UTF8', '*'], None),
                         ('targetStart', ['UTF32', '**'], None),
                         ('targetEnd', ['UTF32', '*'], None),
                         ('flags', ['ConversionFlags'], None))),
 'antlr38BitSetupStream': (['void'],
                           (('input', ['pANTLR3_INPUT_STREAM'], None),)),
 'antlr3ArboretumNew': (['pANTLR3_ARBORETUM'],
                        (('factory', ['pANTLR3_STRING_FACTORY'], None),)),
 'antlr3BaseRecognizerNew': (['pANTLR3_BASE_RECOGNIZER'],
                             (('type', ['ANTLR3_UINT32'], None),
                              ('sizeHint', ['ANTLR3_UINT32'], None),
                              ('state',
                               ['pANTLR3_RECOGNIZER_SHARED_STATE'],
                               None))),
 'antlr3BaseTreeAdaptorInit': (['void'],
                               (('adaptor',
                                 ['pANTLR3_BASE_TREE_ADAPTOR'],
                                 None),
                                ('debugger',
                                 ['pANTLR3_DEBUG_EVENT_LISTENER'],
                                 None))),
 'antlr3BaseTreeNew': (['pANTLR3_BASE_TREE'],
                       (('tree', ['pANTLR3_BASE_TREE'], None),)),
 'antlr3BitsetCopy': (['pANTLR3_BITSET'],
                      (('blist', ['pANTLR3_BITSET_LIST'], None),)),
 'antlr3BitsetList': (['pANTLR3_BITSET'],
                      (('list', ['pANTLR3_HASH_TABLE'], None),)),
 'antlr3BitsetLoad': (['pANTLR3_BITSET'],
                      (('blist', ['pANTLR3_BITSET_LIST'], None),)),
 'antlr3BitsetNew': (['pANTLR3_BITSET'],
                     (('numBits', ['ANTLR3_UINT32'], None),)),
 'antlr3BitsetSetAPI': (['void'], (('bitset', ['pANTLR3_BITSET'], None),)),
 'antlr3CommonTokenDebugStreamSourceNew': (['pANTLR3_COMMON_TOKEN_STREAM'],
                                           (('hint',
                                             ['ANTLR3_UINT32'],
                                             None),
                                            ('source',
                                             ['pANTLR3_TOKEN_SOURCE'],
                                             None),
                                            ('debugger',
                                             ['pANTLR3_DEBUG_EVENT_LISTENER'],
                                             None))),
 'antlr3CommonTokenNew': (['pANTLR3_COMMON_TOKEN'],
                          (('ttype', ['ANTLR3_UINT32'], None),)),
 'antlr3CommonTokenStreamNew': (['pANTLR3_COMMON_TOKEN_STREAM'],
                                (('hint', ['ANTLR3_UINT32'], None),)),
 'antlr3CommonTokenStreamSourceNew': (['pANTLR3_COMMON_TOKEN_STREAM'],
                                      (('hint', ['ANTLR3_UINT32'], None),
                                       ('source',
                                        ['pANTLR3_TOKEN_SOURCE'],
                                        None))),
 'antlr3CommonTreeNew': (['pANTLR3_COMMON_TREE'], ((None, ['void'], None),)),
 'antlr3CommonTreeNewFromToken': (['pANTLR3_COMMON_TREE'],
                                  (('tree', ['pANTLR3_COMMON_TOKEN'], None),)),
 'antlr3CommonTreeNewFromTree': (['pANTLR3_COMMON_TREE'],
                                 (('tree', ['pANTLR3_COMMON_TREE'], None),)),
 'antlr3CommonTreeNodeStreamNew': (['pANTLR3_COMMON_TREE_NODE_STREAM'],
                                   (('strFactory',
                                     ['pANTLR3_STRING_FACTORY'],
                                     None),
                                    ('hint', ['ANTLR3_UINT32'], None))),
 'antlr3CommonTreeNodeStreamNewStream': (['pANTLR3_COMMON_TREE_NODE_STREAM'],
                                         (('inStream',
                                           ['pANTLR3_COMMON_TREE_NODE_STREAM'],
                                           None),)),
 'antlr3CommonTreeNodeStreamNewTree': (['pANTLR3_COMMON_TREE_NODE_STREAM'],
                                       (('tree',
                                         ['pANTLR3_BASE_TREE'],
                                         None),
                                        ('hint', ['ANTLR3_UINT32'], None))),
 'antlr3DebugListenerNew': (['pANTLR3_DEBUG_EVENT_LISTENER'], ()),
 'antlr3EBCDICSetupStream': (['void'],
                             (('input', ['pANTLR3_INPUT_STREAM'], None),)),
 'antlr3EnumNew': (['pANTLR3_HASH_ENUM'],
                   (('table', ['pANTLR3_HASH_TABLE'], None),)),
 'antlr3ExceptionNew': (['pANTLR3_EXCEPTION'],
                        (('exception', ['ANTLR3_UINT32'], None),
                         ('name', ['void', '*'], None),
                         ('message', ['void', '*'], None),
                         ('freeMessage', ['ANTLR3_BOOLEAN'], None))),
 'antlr3Fclose': (['void'], (('fd', ['ANTLR3_FDSC'], None),)),
 'antlr3FileStreamNew': (['pANTLR3_INPUT_STREAM'],
                         (('fileName', ['pANTLR3_UINT8'], None),
                          ('encoding', ['ANTLR3_UINT32'], None))),
 'antlr3Fopen': (['ANTLR3_FDSC'],
                 (('filename', ['pANTLR3_UINT8'], None),
                  ('mode', ['char', '*'], None))),
 'antlr3Fread': (['ANTLR3_UINT32'],
                 (('fdsc', ['ANTLR3_FDSC'], None),
                  ('count', ['ANTLR3_UINT32'], None),
                  ('data', ['void', '*'], None))),
 'antlr3Fsize': (['ANTLR3_UINT32'], (('filename', ['pANTLR3_UINT8'], None),)),
 'antlr3GenericSetupStream': (['void'],
                              (('input', ['pANTLR3_INPUT_STREAM'], None),)),
 'antlr3Hash': (['ANTLR3_UINT32'],
                (('key', ['void', '*'], None),
                 ('keylen', ['ANTLR3_UINT32'], None))),
 'antlr3HashTableNew': (['pANTLR3_HASH_TABLE'],
                        (('sizeHint', ['ANTLR3_UINT32'], None),)),
 'antlr3IntStreamNew': (['pANTLR3_INT_STREAM'], ((None, ['void'], None),)),
 'antlr3IntTrieNew': (['pANTLR3_INT_TRIE'],
                      (('depth', ['ANTLR3_UINT32'], None),)),
 'antlr3LexerNew': (['pANTLR3_LEXER'],
                    (('sizeHint', ['ANTLR3_UINT32'], None),
                     ('state', ['pANTLR3_RECOGNIZER_SHARED_STATE'], None))),
 'antlr3LexerNewStream': (['pANTLR3_LEXER'],
                          (('sizeHint', ['ANTLR3_UINT32'], None),
                           ('input', ['pANTLR3_INPUT_STREAM'], None),
                           ('state',
                            ['pANTLR3_RECOGNIZER_SHARED_STATE'],
                            None))),
 'antlr3ListNew': (['pANTLR3_LIST'], (('sizeHint', ['ANTLR3_UINT32'], None),)),
 'antlr3MTExceptionNew': (['void'],
                          (('recognizer', ['pANTLR3_BASE_RECOGNIZER'], None),)),
 'antlr3MTNExceptionNew': (['void'],
                           (('recognizer',
                             ['pANTLR3_BASE_RECOGNIZER'],
                             None),)),
 'antlr3ParserNew': (['pANTLR3_PARSER'],
                     (('sizeHint', ['ANTLR3_UINT32'], None),
                      ('state', ['pANTLR3_RECOGNIZER_SHARED_STATE'], None))),
 'antlr3ParserNewStream': (['pANTLR3_PARSER'],
                           (('sizeHint', ['ANTLR3_UINT32'], None),
                            ('tstream', ['pANTLR3_TOKEN_STREAM'], None),
                            ('state',
                             ['pANTLR3_RECOGNIZER_SHARED_STATE'],
                             None))),
 'antlr3ParserNewStreamDbg': (['pANTLR3_PARSER'],
                              (('sizeHint', ['ANTLR3_UINT32'], None),
                               ('tstream', ['pANTLR3_TOKEN_STREAM'], None),
                               ('dbg',
                                ['pANTLR3_DEBUG_EVENT_LISTENER'],
                                None),
                               ('state',
                                ['pANTLR3_RECOGNIZER_SHARED_STATE'],
                                None))),
 'antlr3RecognitionExceptionNew': (['void'],
                                   (('recognizer',
                                     ['pANTLR3_BASE_RECOGNIZER'],
                                     None),)),
 'antlr3RewriteRuleNODEStreamNewAE': (['pANTLR3_REWRITE_RULE_NODE_STREAM'],
                                      (('adaptor',
                                        ['pANTLR3_BASE_TREE_ADAPTOR'],
                                        None),
                                       ('rec',
                                        ['pANTLR3_BASE_RECOGNIZER'],
                                        None),
                                       ('description',
                                        ['pANTLR3_UINT8'],
                                        None))),
 'antlr3RewriteRuleNODEStreamNewAEE': (['pANTLR3_REWRITE_RULE_NODE_STREAM'],
                                       (('adaptor',
                                         ['pANTLR3_BASE_TREE_ADAPTOR'],
                                         None),
                                        ('rec',
                                         ['pANTLR3_BASE_RECOGNIZER'],
                                         None),
                                        ('description',
                                         ['pANTLR3_UINT8'],
                                         None),
                                        ('oneElement', ['void', '*'], None))),
 'antlr3RewriteRuleNODEStreamNewAEV': (['pANTLR3_REWRITE_RULE_NODE_STREAM'],
                                       (('adaptor',
                                         ['pANTLR3_BASE_TREE_ADAPTOR'],
                                         None),
                                        ('rec',
                                         ['pANTLR3_BASE_RECOGNIZER'],
                                         None),
                                        ('description',
                                         ['pANTLR3_UINT8'],
                                         None),
                                        ('vector', ['pANTLR3_VECTOR'], None))),
 'antlr3RewriteRuleSubtreeStreamNewAE': (['pANTLR3_REWRITE_RULE_SUBTREE_STREAM'],
                                         (('adaptor',
                                           ['pANTLR3_BASE_TREE_ADAPTOR'],
                                           None),
                                          ('rec',
                                           ['pANTLR3_BASE_RECOGNIZER'],
                                           None),
                                          ('description',
                                           ['pANTLR3_UINT8'],
                                           None))),
 'antlr3RewriteRuleSubtreeStreamNewAEE': (['pANTLR3_REWRITE_RULE_SUBTREE_STREAM'],
                                          (('adaptor',
                                            ['pANTLR3_BASE_TREE_ADAPTOR'],
                                            None),
                                           ('rec',
                                            ['pANTLR3_BASE_RECOGNIZER'],
                                            None),
                                           ('description',
                                            ['pANTLR3_UINT8'],
                                            None),
                                           ('oneElement',
                                            ['void', '*'],
                                            None))),
 'antlr3RewriteRuleSubtreeStreamNewAEV': (['pANTLR3_REWRITE_RULE_SUBTREE_STREAM'],
                                          (('adaptor',
                                            ['pANTLR3_BASE_TREE_ADAPTOR'],
                                            None),
                                           ('rec',
                                            ['pANTLR3_BASE_RECOGNIZER'],
                                            None),
                                           ('description',
                                            ['pANTLR3_UINT8'],
                                            None),
                                           ('vector',
                                            ['pANTLR3_VECTOR'],
                                            None))),
 'antlr3RewriteRuleTOKENStreamNewAE': (['pANTLR3_REWRITE_RULE_TOKEN_STREAM'],
                                       (('adaptor',
                                         ['pANTLR3_BASE_TREE_ADAPTOR'],
                                         None),
                                        ('rec',
                                         ['pANTLR3_BASE_RECOGNIZER'],
                                         None),
                                        ('description',
                                         ['pANTLR3_UINT8'],
                                         None))),
 'antlr3RewriteRuleTOKENStreamNewAEE': (['pANTLR3_REWRITE_RULE_TOKEN_STREAM'],
                                        (('adaptor',
                                          ['pANTLR3_BASE_TREE_ADAPTOR'],
                                          None),
                                         ('rec',
                                          ['pANTLR3_BASE_RECOGNIZER'],
                                          None),
                                         ('description',
                                          ['pANTLR3_UINT8'],
                                          None),
                                         ('oneElement', ['void', '*'], None))),
 'antlr3RewriteRuleTOKENStreamNewAEV': (['pANTLR3_REWRITE_RULE_TOKEN_STREAM'],
                                        (('adaptor',
                                          ['pANTLR3_BASE_TREE_ADAPTOR'],
                                          None),
                                         ('rec',
                                          ['pANTLR3_BASE_RECOGNIZER'],
                                          None),
                                         ('description',
                                          ['pANTLR3_UINT8'],
                                          None),
                                         ('vector',
                                          ['pANTLR3_VECTOR'],
                                          None))),
 'antlr3SetCTAPI': (['void'], (('tree', ['pANTLR3_COMMON_TREE'], None),)),
 'antlr3SetTokenAPI': (['void'], (('token', ['pANTLR3_COMMON_TOKEN'], None),)),
 'antlr3SetVectorApi': (['void'],
                        (('vector', ['pANTLR3_VECTOR'], None),
                         ('sizeHint', ['ANTLR3_UINT32'], None))),
 'antlr3StackNew': (['pANTLR3_STACK'],
                    (('sizeHint', ['ANTLR3_UINT32'], None),)),
 'antlr3StringFactoryNew': (['pANTLR3_STRING_FACTORY'],
                            (('encoding', ['ANTLR3_UINT32'], None),)),
 'antlr3StringStreamNew': (['pANTLR3_INPUT_STREAM'],
                           (('data', ['pANTLR3_UINT8'], None),
                            ('encoding', ['ANTLR3_UINT32'], None),
                            ('size', ['ANTLR3_UINT32'], None),
                            ('name', ['pANTLR3_UINT8'], None))),
 'antlr3TokenFactoryNew': (['pANTLR3_TOKEN_FACTORY'],
                           (('input', ['pANTLR3_INPUT_STREAM'], None),)),
 'antlr3TopoNew': (['pANTLR3_TOPO'], ()),
 'antlr3TreeNodeStreamNew': (['pANTLR3_TREE_NODE_STREAM'], ()),
 'antlr3TreeParserNewStream': (['pANTLR3_TREE_PARSER'],
                               (('sizeHint', ['ANTLR3_UINT32'], None),
                                ('ctnstream',
                                 ['pANTLR3_COMMON_TREE_NODE_STREAM'],
                                 None),
                                ('state',
                                 ['pANTLR3_RECOGNIZER_SHARED_STATE'],
                                 None))),
 'antlr3UTF16SetupStream': (['void'],
                            (('input', ['pANTLR3_INPUT_STREAM'], None),
                             ('machineBigEndian', ['ANTLR3_BOOLEAN'], None),
                             ('inputBigEndian', ['ANTLR3_BOOLEAN'], None))),
 'antlr3UTF32SetupStream': (['void'],
                            (('input', ['pANTLR3_INPUT_STREAM'], None),
                             ('machineBigEndian', ['ANTLR3_BOOLEAN'], None),
                             ('inputBigEndian', ['ANTLR3_BOOLEAN'], None))),
 'antlr3UTF8SetupStream': (['void'],
                           (('input', ['pANTLR3_INPUT_STREAM'], None),)),
 'antlr3UnbufTreeNodeStreamNew': (['pANTLR3_COMMON_TREE_NODE_STREAM'],
                                  (('strFactory',
                                    ['pANTLR3_STRING_FACTORY'],
                                    None),
                                   ('hint', ['ANTLR3_UINT32'], None))),
 'antlr3UnbufTreeNodeStreamNewTree': (['pANTLR3_COMMON_TREE_NODE_STREAM'],
                                      (('tree', ['pANTLR3_BASE_TREE'], None),
                                       ('hint', ['ANTLR3_UINT32'], None))),
 'antlr3VectorFactoryNew': (['pANTLR3_VECTOR_FACTORY'],
                            (('sizeHint', ['ANTLR3_UINT32'], None),)),
 'antlr3VectorNew': (['pANTLR3_VECTOR'],
                     (('sizeHint', ['ANTLR3_UINT32'], None),)),
 'antlr3c8toAntlrc': (['ANTLR3_UCHAR'], (('inc', ['ANTLR3_INT8'], None),)),
 'antlr3dfapredict': (['ANTLR3_INT32'],
                      (('ctx', ['void', '*'], None),
                       ('rec', ['pANTLR3_BASE_RECOGNIZER'], None),
                       ('is', ['pANTLR3_INT_STREAM'], None),
                       ('cdfa', ['pANTLR3_CYCLIC_DFA'], None))),
 'antlr3dfaspecialStateTransition': (['ANTLR3_INT32'],
                                     (('ctx', ['void', '*'], None),
                                      ('rec',
                                       ['pANTLR3_BASE_RECOGNIZER'],
                                       None),
                                      ('is', ['pANTLR3_INT_STREAM'], None),
                                      ('dfa', ['pANTLR3_CYCLIC_DFA'], None),
                                      ('s', ['ANTLR3_INT32'], None))),
 'antlr3dfaspecialTransition': (['ANTLR3_INT32'],
                                (('ctx', ['void', '*'], None),
                                 ('rec', ['pANTLR3_BASE_RECOGNIZER'], None),
                                 ('is', ['pANTLR3_INT_STREAM'], None),
                                 ('dfa', ['pANTLR3_CYCLIC_DFA'], None),
                                 ('s', ['ANTLR3_INT32'], None))),
 'antlr3read8Bit': (['ANTLR3_UINT32'],
                    (('input', ['pANTLR3_INPUT_STREAM'], None),
                     ('fileName', ['pANTLR3_UINT8'], None))),
 'fillBufferExt': (['void'],
                   (('tokenStream', ['pANTLR3_COMMON_TOKEN_STREAM'], None),)),
 'isLegalUTF8Sequence': (['ANTLR3_BOOLEAN'],
                         (('source', ['UTF8', '*'], None),
                          ('sourceEnd', ['UTF8', '*'], None)))}