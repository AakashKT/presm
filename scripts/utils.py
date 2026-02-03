import os, argparse, shutil, subprocess, platform

def print_red(s): print("\033[91m{}\033[00m".format('++ ' + s))
def print_green(s): print("\033[92m{}\033[00m".format('++ ' + s))
def print_white(s): print('++ ' + s)

def error_exit(string):
    print_red(string)
    exit(1)

def init():
    current_directory = os.getcwd()
    last_dir = current_directory.split('/')[-1]
    if last_dir == 'scripts':
        os.chdir('../')

def make_numbered_execution_dir(parent_dir):
    path_exists = os.path.exists(parent_dir)
    if not path_exists:
        os.mkdir(parent_dir)

    l = os.listdir(parent_dir)
    if len(l) == 0:
        max_num = -1
    else:
        l.sort()
        max_num = int(l[-1].split('/')[-1])

    final_path = parent_dir + '/' + str(max_num+1).zfill(5)
    os.mkdir(final_path)

    return final_path

def get_driver_lib(driver_name):
    system_name = platform.system()

    if system_name == 'Windows':
        static_lib_path = os.getcwd() + '/build/driver/' + driver_name + '/Release/driver.lib'

        if not os.path.isfile(static_lib_path):
            error_exit('Cannot find static library of PRESM. On windows, PRESM can be only run if built as a static library.')
        
        return static_lib_path, True
    
    elif system_name == 'Linux':
        shared_lib_path = os.getcwd() + '/build/driver/' + driver_name + '/libdriver.so'
        static_lib_path = os.getcwd() + '/build/driver/' + driver_name + '/libdriver.a'

        if os.path.isfile(shared_lib_path):
            return shared_lib_path, False
        
        elif os.path.isfile(static_lib_path):
            return static_lib_path, True
        
        else:
            error_exit('Cannot find static or shared library of PRESM.')

def get_verification_dir_and_exe():
    system_name = platform.system()

    if system_name == 'Windows':
        working_directory = os.getcwd() + '/build/verification/Release/'
        executable = 'verification.exe'
    
    elif system_name == 'Linux':
        working_directory = os.getcwd() + '/build/verification/'
        executable = 'verification'
    
    return working_directory, executable

def _copy_file(src_file, dst_dir):
    print_white(f'Copying file {src_file} to {dst_dir}')
    shutil.copy(src_file, dst_dir)

def _copy_files_only(src_dir, dst_dir):
    print_white(f'Copying files from {src_dir} to {dst_dir}')

    src_files = os.listdir(src_dir)
    for file_name in src_files:
        full_file_name = os.path.join(src_dir, file_name)
        if os.path.isfile(full_file_name):
            shutil.copy(full_file_name, dst_dir)

def _copy_recursive(src_dir, dst_dir):
    print_white(f'Copying recursively from {src_dir} to {dst_dir}')
    shutil.copytree(src_dir, dst_dir, dirs_exist_ok=True)

def _chdir(directory):
    os.chdir(directory)
    print_white('Changing working directory to: ' + os.getcwd())

def _execute(command : list, env=None):
    print_white('Executing: ' + command)
    if env:
        print_white('Environment: ')
        print(env)
    
    op = subprocess.run(command.split(' '), env=env, capture_output=True, text=True, errors='ignore')
    if op.returncode != 0:
        print_red(op.stderr)

    return op        

def execute(working_directory, executable, args):
    current_directory = os.getcwd()

    _chdir(working_directory)
    op = _execute('./' + executable + ' ' + ' '.join(args))
    _chdir(current_directory)
    print('')

    return op.stdout

def presm_execute(working_directory, executable, driver_lib, app_args, driver_name):
    print_white('======================')
    print_white('PRESM Execution Begin')
    print_white('======================')

    current_directory = os.getcwd()

    _chdir(working_directory)
    op = _execute(
        './' + executable + ' ' + ' '.join(app_args), 
        {'LD_PRELOAD': driver_lib}
    )
    _chdir(current_directory)

    print_white('======================')
    print_white('PRESM Execution End')
    print_white('======================')
    print('')

    return op.stdout

def sanitize_presm_config(config):
    config_type = config["type"]

    if config_type == "functional":
        try:
            driver_name = config["driver"]["name"]
            device_name = config["device"]["name"]
        
        except:
            utils.error_exit(f"Incomplete config for type='{config_type}'")

    elif config_type == "fpga":
        try:
            driver_name = config["driver"]["name"]

            device_name = config["device"]["name"]
            rtl_device = config["device"]["rtl"]

            fpga_name = config["fpga"]["name"]
        
        except:
            utils.error_exit(f"Incomplete config for type='{config_type}'")

    else:
        utils.error_exit(f"Config with type='{config_type}' is not supported.")