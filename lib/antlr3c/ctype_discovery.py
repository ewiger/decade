'''
A module for (semi)automatic discovery and dynamical creation of ctypes based
on data structures parsed form C headers using pyclibrary.py

'''
__author__ = 'Yauhen Yakimovich <eugeny.yakimovitch@gmail.com>'

import sys
from ctypes import *
from pprint import pprint


# cache all dynamic constructs and partial (incomplete types)
declarations = dict()
incomplete_types = list()


def declare_signatures():
    signature_tuples = (
        (None, ['void']),
        (c_bool, ['_Bool']), #bool (1)
        (c_char, ['char']),	#1-character string
        (c_wchar, ['wchar_t']), #1-character unicode string
        (c_byte, ['char']), #int/long
        (c_ubyte, ['unsigned', 'char']), #int/long
        (c_ubyte, ['uint8_t']),
        (c_short, ['short']), #int/long
        (c_ushort, ['unsigned', 'short']), #int/long
        (c_int,	['int']), #int/long
        (c_int,	['int32_t']),
        (c_uint, ['unsigned', 'int']), #int/long
        (c_uint, ['uint32_t']),
        (c_long, ['long']), #int/long
        (c_ulong, ['unsigned', 'long']), #int/long
        (c_longlong, ['__int64']), 
        (c_longlong, ['long', 'long']), #int/long
        (c_ulonglong, ['unsigned','__int64']), 
        (c_ulonglong, ['unsigned','long', 'long']),	#int/long
        (c_float, ['float']), #float
        (c_double, ['double']),	#float
        (c_longdouble, ['long', 'double']),	#float
        (c_char_p,	['char', '*']), #(NUL terminated)	string or None
        (c_wchar_p,	['wchar_t', '*']), #(NUL terminated)	unicode or None
        (c_void_p, ['void', '*']), #int/long or None
    )
    ctype_to_c = dict()
    c_to_ctype = dict()
    for ctyp, sign in signature_tuples:
        ctype_to_c[type(ctyp).__name__] = sign
        c_to_ctype[str(sign)] = ctyp
    return (signature_tuples, ctype_to_c, c_to_ctype)


signature_tuples, ctype_to_c, c_to_ctype = declare_signatures()


class CTypeMapper(object):

    def __init__(self, parsed_structs, parsed_types, parsed_unions, verbose=True):
        self.parsed_types = parsed_types
        self.parsed_structs = parsed_structs
        self.parsed_unions = parsed_unions
        self._recursion_limit = 30
        self._recursion_count = 0
        self.discovery_stack = list()
        self.verbose = verbose

    def debug(self, msg):
        if not self.verbose:
            return
        print ' *-' + ('-' * self._recursion_count) + msg

    def is_incomplete_type(self, typename):
        return typename in incomplete_types

    def is_a_type_name(self, typename):
        return isinstance(typename, basestring)

    def is_discovered(self, typename):
        assert isinstance(typename, basestring)
        return typename in declarations

    def is_user_defined(self, typename):
        return typename in self.parsed_types

    def is_a_struct(self, typename):
        return typename in self.parsed_structs

    def is_a_union(self, typename):
        return typename in self.parsed_unions

    def is_a_trivial_type(self, signature):
        return isinstance(signature, (list)) \
            and len(signature) <= 3 \
            and str(signature) in c_to_ctype

    def is_a_pointer(self, signature):
        return isinstance(signature, (list)) \
            and signature[-1] == '*'

    def is_a_trivial_pointer(self, signature):
        '''Signature ends with * and items a strings'''
        return isinstance(signature, (list)) \
            and signature[-1] == '*' \
            and all(isinstance(item,basestring) for item in signature)

    def is_fntype(self, signature):
        return any(isinstance(item, tuple) for item in signature) \
            and signature[-1] == '*'

    def _enter_recursion(self, method_name):
        if self._recursion_count > self._recursion_limit:
            print >> sys.stderr, 'Max recurrsion reached'
            exit()
        self._recursion_count += 1
        self.debug('>[%s]' % method_name)
       
    def _leave_recursion(self, method_name, result):
        self._recursion_count -= 1
        self.debug('<[%s]: %s' % (method_name, str(result)))
    
    def discover_type(self, name):
        self._enter_recursion('discover_type')
        result = self._discover_type(name)
        self._leave_recursion('discover_type', result)
        return result

    def _discover_type(self, name):
        self.debug('Discovering %s' % name)
        # allow tuples
        if type(name) == tuple:
            hint = name[0]
            name = name[1]
        if not isinstance(name, basestring):
            raise Exception('Excpeting type name to be a string: %s!' % str(name))
        # Look in global cache.
        if name in declarations:
            # found
            return declarations[name]
        in_discovery_loop = name in self.discovery_stack
        if not in_discovery_loop:
            self.discovery_stack.append(name)
       

        # Create new struct/union/type
        newtype = None
        if not in_discovery_loop:
            if name in self.parsed_structs:
                newtype = self.declare_struct(name)
            elif name in self.parsed_unions:
                newtype = self.declare_union(name)
        else:
            self.debug('Temporary declaring incomplete self-referencing type: %s' % name)
            # Declare and return incomplete struct first
            if name in self.parsed_structs:
                newtype = self.declare_struct(name, incomplete=True)
            elif name in self.parsed_unions:
                newtype = self.declare_union(name, incomplete=True)
        if not newtype and name in self.parsed_types:
            # can map pointers signature directly to their underlying types via
            # discovery recursion
            typename = self.parsed_types[name]
            if type(typename) == tuple and typename[0] in ('struct', 'union'):
                return self.discover_type(typename[1])
            newtype = self.map_signature(typename, False)

        if not newtype:
            raise Exception('Failed to discover: %s' % name)

        return newtype       


    def map_signature(self, signature, is_func_arg=False):
        self._enter_recursion('map_signature')
        result = self._map_signature(signature, is_func_arg)
        self._leave_recursion('map_signature', result)
        return result

    def _map_signature(self, signature, is_func_arg):
        ''' Signature is a list of types, parsed in a AST-aware manner'''
        # check if first item is a string - it may be already a discovered type
        self.debug('Mapping %s' % str(signature))
        if self.is_a_trivial_type(signature):
            return c_to_ctype[str(signature)]

        if self.is_a_trivial_pointer(signature):
            trivial_signature = signature[:-1]
            return POINTER(self.map_signature(trivial_signature))

        # Signature containing a User-defined C type
        typename = None
        if len(signature) == 1:
            typename = signature[0]
        elif isinstance(signature, basestring):
            # Like this we can actually map user-defined strings into
            # discovered types.
            typename = signature

        if typename and self.is_a_type_name(typename):
            if typename.startswith('struct '):
                typename = typename[len('struct '):]
            if self.is_discovered(typename):
                return declarations[typename]
            if self.is_a_struct(typename) \
            or self.is_a_union(typename) \
            or self.is_user_defined(typename):
                return self.discover_type(typename)
           
        if is_func_arg:
            return self.map_signature(signature[1])

        elif self.is_fntype(signature):
            self.debug('signature is a functional')
            restype = list()
            for item in signature:
                if isinstance(item, tuple):
                    break
                restype.append(item)
            rest = signature[len(restype):-1]
            restype = self.map_signature(restype)
            args = list()
            for item in rest:
                if isinstance(item, tuple) and isinstance(item[0], tuple):
                    # is a functype (callback pointer)
                    self.debug('callback arg: ' + str(item[0]))
                    args.append(self.map_signature(item[0], True))
                else:
                    args.append(self.map_signature(item))
            return CFUNCTYPE(restype, *args)
        
        # ARRAY
        elif len(signature) == 2 and isinstance(signature[1], list) \
            and isinstance(signature[1][0], (int,long)) and isinstance(signature[0], basestring):
            # To create an array type we multiple by integer
            array_type = self.discover_type(signature[0])
            times = signature[1][0]
            return array_type * times

        raise Exception('Failed to map signature to any ctype: %s!' %
            str(signature))

    def declare_struct(self, name, as_clsname=None, incomplete=False):    
        return self.declare_struct_or_union(name, as_clsname, incomplete, basecls=Structure)

    def declare_union(self, name, as_clsname=None, incomplete=False):    
        return self.declare_struct_or_union(name, as_clsname, incomplete, basecls=Union)

    def declare_struct_or_union(self, name, as_clsname=None, incomplete=False, basecls=Structure):
        self.debug('Declaring %s (incomplete=%s)' % (name, incomplete))
        if name in declarations:
            self.debug('Taking from cache %s' % name)
            return declarations[name]
        if as_clsname is None:
            as_clsname = name
        if basecls == Structure and name not in self.parsed_structs:
            raise Exception('Unknown struct: "%s"' % name)
        elif basecls == Union and name not in self.parsed_unions:
            raise Exception('Unknown union: "%s"' % name)
        elif basecls is not Structure and basecls is not Union:
            raise Exception('Unknown type: "%s"' % basecls)
        if incomplete:
            newtype = type(
                as_clsname,
                (basecls,),
                dict(),
            )
            incomplete_types.append(name)
            declarations[name] = newtype
            return newtype
        # Discover fields.
        if basecls == Structure:
            members = self.parsed_structs[name]['members']
        elif basecls == Union:            
            members = self.parsed_unions[name]['members']
        fields = list()
        for fieldname, fieldtype, foo in members:
            #print  '%s: %s' % (fieldname, fieldtype)
            fields.append((fieldname, self.map_signature(fieldtype, False)))
        self.debug('Reaching incompletness check for %s' % name)
        if self.is_incomplete_type(name):
            # Was already defined as incomplete. Add found fields to existing
            # incomplete entry.
            declared_type = declarations[name]
            declared_type._fields_ = fields
            incomplete_types.remove(name)
            return declared_type
        # Constructing new type.
        #print 'fields: %s' % fields
        newtype = type(
            as_clsname,
            (basecls,),
            {'_fields_': fields},
        )
        declarations[name] = newtype
        return newtype

