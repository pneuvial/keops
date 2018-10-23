import subprocess
from pykeops import build_folder, script_folder, verbose, build_type
from pykeops.common.utils import c_type
from pykeops.common.parse_type import check_aliases_list
from pykeops.common.get_options import torch_include_path


def run_and_display(args, msg=''):
    """
    This function run the command stored in args and display the output if needed
    :param args: list
    :param msg: str
    :return: None
    """
    try:
        proc = subprocess.run(args, cwd=build_folder, stdout=subprocess.PIPE, check=True)
        if verbose:
            print(proc.stdout.decode('utf-8'))

    except subprocess.CalledProcessError as e:
        print('\n--------------------- ' + msg + ' DEBUG -----------------')
        print(e)
        print(e.stdout.decode('utf-8'))
        print('--------------------- ----------- -----------------')


def compile_generic_routine(formula, aliases, dllname, cuda_type, lang):
    aliases = check_aliases_list(aliases)

    def process_alias(alias):
        if alias.find("=") == -1:
            return '' # because in this case it is not really an alias, the variable is just named
        else:
            return 'auto ' + str(alias) + '; '

    def process_disp_alias(alias):
        return str(alias) + '; '

    alias_string = ''.join([process_alias(alias) for alias in aliases])
    alias_disp_string = ''.join([process_disp_alias(alias) for alias in aliases])

    target = dllname
    
    print('Compiling ' + dllname + ' in ' + build_folder + ':\n' + '       formula: ' + formula + '\n       aliases: ' + alias_disp_string + '\n       dtype  : ' + cuda_type + '\n... ', end='', flush=True)

    run_and_display(['cmake', script_folder,
                     '-GNinja',
                     '-DCMAKE_BUILD_TYPE=' + build_type,
                     '-DFORMULA_OBJ=' + formula,
                     '-DVAR_ALIASES=' + alias_string,
                     '-Dshared_obj_name=' + dllname,
                     '-D__TYPE__=' + c_type[cuda_type],
                     '-DPYTHON_LANG=' + lang,
                     '-DPYTORCH_INCLUDE_DIR=' + torch_include_path,
                     ],
                    msg='CMAKE')

    run_and_display(['cmake', '--build', '.', '--target', target], msg='MAKE')
    print('Done. ', end='\n', flush=True)


def compile_specific_conv_routine(target, cuda_type):
    print('Compiling ' + target + ' using ' + cuda_type + '... ', end='', flush=True)
    run_and_display(['cmake', script_folder,
                     '-GNinja',
                     '-DCMAKE_BUILD_TYPE=' + build_type,
                     '-Ushared_obj_name',
                     '-D__TYPE__=' + c_type[cuda_type],
                    ],
                    msg='CMAKE')
    run_and_display(['cmake', '--build', '.', '--target', target], msg='MAKE')
    print('Done. ', end='\n', flush=True)


def compile_specific_fshape_scp_routine(target, kernel_geom, kernel_sig, kernel_sphere, cuda_type):
    print('Compiling ' + target + ' using ' + cuda_type + '... ', end='', flush=True)
    run_and_display(['cmake', script_folder,
                     '-GNinja',
                     '-DCMAKE_BUILD_TYPE=' + build_type,
                     '-Ushared_obj_name',
                     '-DKERNEL_GEOM=' + kernel_geom,
                     '-DKERNEL_SIG=' + kernel_sig,
                     '-DKERNEL_SPHERE=' + kernel_sphere,
                     '-D__TYPE__=' + c_type[cuda_type],
                    ],
                    msg='CMAKE')
    run_and_display(['cmake', '--build', '.', '--target', target], msg='MAKE')
    print('Done. ', end='\n', flush=True)
