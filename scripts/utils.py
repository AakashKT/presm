import os, argparse, shutil, subprocess, platform

def print_red(s): print("\033[91m{}\033[00m".format('++ ' + s))
def print_green(s): print("\033[92m{}\033[00m".format('++ ' + s))
def print_white(s): print('++ ' + s)

def init():
    os.chdir('../')

def get_working_directory_and_executable(driver):
    system_name = platform.system()

    if system_name == 'Windows':
        working_directory = os.getcwd() + '/build/verification/' + driver + '/Release/'
        executable = 'verification.exe'
    
    elif system_name == 'Linux':
        working_directory = os.getcwd() + '/build/verification/' + driver + '/'
        executable = 'verification'
    
    if not os.path.exists(working_directory):
        print_red('Verification apps for driver ' + driver + 'do not exist.')
        exit()
    
    return working_directory, executable

def get_driver_lib(driver_name):
    system_name = platform.system()

    if system_name == 'Windows':
        static_lib_path = os.getcwd() + '/build/driver/' + driver_name + '/Release/driver.lib'

        if not os.path.isfile(static_lib_path):
            print_red('Cannot find static library of PRESM. On windows, PRESM can be only run if built as a static library.')
            exit()
        
        return static_lib_path, True
    
    elif system_name == 'Linux':
        shared_lib_path = os.getcwd() + '/build/driver/' + driver_name + '/libdriver.so'
        static_lib_path = os.getcwd() + '/build/driver/' + driver_name + '/libdriver.a'

        if os.path.isfile(shared_lib_path):
            return shared_lib_path, False
        
        elif os.path.isfile(static_lib_path):
            return static_lib_path, True
        
        else:
            print_red('Cannot find static or shared library of PRESM.')
            exit()

def _chdir(directory):
    os.chdir(directory)
    print_white('Changing working directory to: ' + os.getcwd())

def _execute(command : list, env=None):
    print_white('Executing: ' + command)
    
    op = subprocess.run(command.split(' '), env=env, capture_output=True, text=True, errors='ignore')
    if op.returncode != 0:
        print_red(op.stderr)

    return op        

def execute(working_directory, executable, args):
    current_directory = os.getcwd()

    _chdir(working_directory)
    op = _execute('./' + executable + ' ' + ' '.join(args))
    _chdir(current_directory)

    return op.stdout

def presm_execute(working_directory, executable, args, driver_name):
    print_white('======================')
    print_white('PRESM Execution Begin')
    print_white('======================')

    current_directory = os.getcwd()
    library_path, is_static = get_driver_lib(driver_name)

    _chdir(working_directory)
    op = _execute(
        './' + executable + ' ' + ' '.join(args), 
        {'LD_PRELOAD': library_path}
    )
    _chdir(current_directory)

    print_white('======================')
    print_white('PRESM Execution End')
    print_white('======================')

    return op.stdout

def verify_output(source, target, print_output=False):
    i = 0
    for line in target:
        if line != source[i]:
            print_red('Verification failed.')

            print('\nSource output:')
            print(source)

            print('\nTarget output:')
            print(target)

            exit(-1)
        
        i += 1
    
    print_green('Verification succeeded.')

    if print_output:
        print('\nSource output:')
        print(source)

        print('\nTarget output:')
        print(target)