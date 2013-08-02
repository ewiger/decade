''' 
VARIABLES

This is an auto-generated ctypes file from ANTLR3 C runtime headers. Note that
editing this file is not smart! For more details check genclib.py

It should be possible to redefine things in __init__.py if necessary (right
after imports section).

wbr, yy
'''

VARIABLES = \
{'accept': (None, ['ANTLR3_INT32', '*']),
 'decisionNumber': (None, ['ANTLR3_INT32']),
 'description': (None, ['pANTLR3_UCHAR']),
 'eof': (None, ['ANTLR3_INT32', '*']),
 'eot': (None, ['ANTLR3_INT32', '*']),
 'max': (None, ['ANTLR3_INT32', '*']),
 'min': (None, ['ANTLR3_INT32', '*']),
 'predict': (None,
             ['ANTLR3_INT32',
              (('ctx', ['void', '*'], None),
               ('recognizer', ['pANTLR3_BASE_RECOGNIZER'], None),
               ('is', ['pANTLR3_INT_STREAM'], None),
               ('dfa', ['struct ANTLR3_CYCLIC_DFA_struct', '*'], None)),
              '*']),
 'special': (None, ['ANTLR3_INT32', '*']),
 'specialStateTransition': (None,
                            ['ANTLR3_INT32',
                             (('ctx', ['void', '*'], None),
                              ('recognizer',
                               ['pANTLR3_BASE_RECOGNIZER'],
                               None),
                              ('is', ['pANTLR3_INT_STREAM'], None),
                              ('dfa',
                               ['struct ANTLR3_CYCLIC_DFA_struct', '*'],
                               None),
                              ('s', ['ANTLR3_INT32'], None)),
                             '*']),
 'specialTransition': (None,
                       ['ANTLR3_INT32',
                        (('ctx', ['void', '*'], None),
                         ('recognizer', ['pANTLR3_BASE_RECOGNIZER'], None),
                         ('is', ['pANTLR3_INT_STREAM'], None),
                         ('dfa',
                          ['struct ANTLR3_CYCLIC_DFA_struct', '*'],
                          None),
                         ('s', ['ANTLR3_INT32'], None)),
                        '*'])}
