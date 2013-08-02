import os
import sys

already_extended = False
ROOT = 'decade'


def extend_sys_path(module_path, folders):
    global already_extended
    if already_extended:        
        return    
    if os.path.islink(module_path):
        # module_path points to a symlink
        module_path = os.path.abspath(os.readlink(module_path))
        root_path = os.path.dirname(module_path)
    elif ROOT + '/' in module_path:
        # module_path points to .pyc
        root_path, module_path = module_path.rsplit(ROOT + '/', 1)
        root_path += ROOT
        
    for folder in folders:
        # Prepend path to the modules effectively prioritizing them.
        sys.path.insert(0, os.path.join(root_path, folder))    
    already_extended = True
    

extend_sys_path(__file__, ('lib', 'tools'))

