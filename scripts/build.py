import os, argparse, shutil, utils, platform, json
import urllib.request
import zipfile

def build_presm(args, config):
    # Device specific steps
    if config['device']['name'].lower() == 'riscv_accel':
        # Check if riscv compiler exists
        linux_path = os.path.exists('build_riscv_compiler/linux/bin')
        windows_path = os.path.exists('build_riscv_compiler/windows/xpack/bin')
        if (not linux_path) and (not windows_path):
            utils.print_red('Did not find RISC-V compiler for your platform. Please run with --get_extern_tools to get/build it?')
            exit()

    path_exists = os.path.exists('build')
    if args.clean and path_exists:
        shutil.rmtree('build')
    
    path_exists = os.path.exists('build')
    if not path_exists:
        os.mkdir('build')

    os.chdir('build')

    if args.clean or (not path_exists):
        os.system('cmake .. -DDRIVER=%s -DDEVICE=%s' % (config['driver']['name'].lower(), config['device']['name'].lower()))

    os.system('cmake --build . --config Release')

    # Driver specific steps
    if config['driver']['name'].lower() == 'cuda':
        os.system('cmake --build . --config Release --target install')

def build_riscv_compiler_linux(args, config):
    path_exists = os.path.exists('linux')
    if not path_exists:
        os.mkdir('linux')
    
    os.chdir('linux')

    current_directory = os.getcwd()

    os.system('sudo apt-get install autoconf automake autotools-dev curl python3 python3-pip python3-tomli libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev ninja-build git cmake libglib2.0-dev libslirp-dev libncurses-dev')

    os.chdir('../../extern/riscv-gnu-toolchain/')
    os.system('make clean')

    os.system('./configure --prefix=' 
            + current_directory + ' --with-arch=rv32im')
    os.system('make -j8')

def get_riscv_compiler_windows(args, config):
    path_exists = os.path.exists('windows')
    if not path_exists:
        os.mkdir('windows')
    
    os.chdir('windows')

    # Download risc-v compiler for windows, from xpack-dev-tools
    download_link = 'https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack/releases/download/v15.2.0-1/xpack-riscv-none-elf-gcc-15.2.0-1-win32-x64.zip'
    urllib.request.urlretrieve(download_link, os.getcwd() + '/temp.zip')

    with zipfile.ZipFile('temp.zip', 'r') as zip_ref:
        zip_ref.extractall('./')
    
    os.rename('xpack-riscv-none-elf-gcc-15.2.0-1', 'xpack')

def get_or_build_extern_tools(args, config):
    system_name = platform.system()
    if config['device']['name]'].lower() == 'riscv_accel':
        path_exists = os.path.exists('build_riscv_compiler')
        if not path_exists:
            os.mkdir('build_riscv_compiler')
        
        os.chdir('build_riscv_compiler')

        if system_name == 'Linux':
            build_riscv_compiler_linux(args, config)
        
        elif system_name == 'Windows':
            get_riscv_compiler_windows(args, config)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--config', required=True)
    parser.add_argument('--clean', action="store_true")
    parser.add_argument('--get_extern_tools', action="store_true")
    args = parser.parse_args()

    utils.init()
    config = json.load(open(args.config))

    if args.get_extern_tools:
        get_or_build_extern_tools(args, config)

    build_presm(args, config)

        
    
    
