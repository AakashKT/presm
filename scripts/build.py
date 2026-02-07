import os, argparse, shutil, utils, platform, json
import urllib.request
import zipfile

def build_presm(args, config):
    utils.sanitize_presm_config(config)

    path_exists = os.path.exists('build')
    if args.clean and path_exists:
        shutil.rmtree('build')
    
    path_exists = os.path.exists('build')
    if not path_exists:
        os.mkdir('build')

    utils._chdir('build')

    driver_name = config['driver']['name']
    device_name = config['device']['name']
    device_type = config['device']['type']

    os.system(f'cmake .. -DDRIVER={driver_name} -DDEVICE={device_name} \
                -DDEVICE_TYPE={device_type}')

    os.system('cmake --build . --config Release')
    os.system('cmake --build . --config Release --target install')

def build_riscv_compiler_linux(args, config):
    path_exists = os.path.exists('linux')
    if not path_exists:
        os.mkdir('linux')
    
    utils._chdir('linux')

    current_directory = os.getcwd()

    utils._chdir('../../extern/riscv-gnu-toolchain/')
    os.system('make clean')

    os.system('./configure --prefix=' 
            + current_directory + ' --with-arch=rv32im')
    os.system('make -j8')

    utils._chdir('../../build_riscv_compiler')

# def get_riscv_compiler_windows(args, config):
#     path_exists = os.path.exists('windows')
#     if not path_exists:
#         os.mkdir('windows')
    
#     utils._chdir('windows')

#     # Download risc-v compiler for windows, from xpack-dev-tools
#     download_link = 'https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack/releases/download/v15.2.0-1/xpack-riscv-none-elf-gcc-15.2.0-1-win32-x64.zip'
#     urllib.request.urlretrieve(download_link, os.getcwd() + '/temp.zip')

#     with zipfile.ZipFile('temp.zip', 'r') as zip_ref:
#         zip_ref.extractall('./')
    
#     os.rename('xpack-riscv-none-elf-gcc-15.2.0-1', 'xpack')

def build_fpga_toolchain(args, config):
    if len(os.listdir('extern')) == 0:
        utils.print_red('No submodules found. Did you reun git clone --recursive-submodules?')
        exit()

    # Build yosys (Synthesis)
    utils._chdir('extern/yosys')
    utils._execute('make -j8')

    # Build nextpnr-himbaechel (place-and-route)
    utils._chdir('../nextpnr')
    if os.path.exists('build'):
        shutil.rmtree('build')
    os.mkdir('build')
    utils._chdir('build')

    utils._execute('cmake .. -DARCH=himbaechel -DHIMBAECHEL_UARCH=gowin')
    utils._execute('make -j8')

    # Build openFPGALoader (programming the FPGA)
    utils._chdir('../../openFPGALoader/')
    if os.path.exists('build'):
        shutil.rmtree('build')
    os.mkdir('build')
    utils._chdir('build')

    utils._execute('cmake ..')
    utils._execute('make -j8')

    utils._chdir('../../../')

def install_ubuntu_packages():
    os.system('sudo apt update')

    os.system('sudo apt-get install iverilog')

    os.system('sudo apt-get install autoconf automake autotools-dev \
                curl python3 python3-pip python3-tomli libmpc-dev libmpfr-dev \
                libgmp-dev gawk build-essential bison flex texinfo gperf libtool \
                patchutils bc zlib1g-dev libexpat-dev ninja-build git cmake \
                libglib2.0-dev libslirp-dev libncurses-dev libreadline-dev')

def get_or_build_extern_tools(args, config):
    system_name = platform.system()

    install_ubuntu_packages()
    build_fpga_toolchain(args, config)

    path_exists = os.path.exists('build_riscv_compiler')
    if not path_exists:
        os.mkdir('build_riscv_compiler')
    
    utils._chdir('build_riscv_compiler')

    if system_name == 'Linux':
        build_riscv_compiler_linux(args, config)
    
    elif system_name == 'Windows':
        utils.print_red('Cannot satisfy external tool requirements on Windows')

def check_external_tools(args, config):
    yosys_linux_path = os.path.exists('extern/yosys')
    nextpnr_linux_path = os.path.exists('extern/nextpnr/build')
    openFPGALoader_linux_path = os.path.exists('extern/openFPGALoader/build')

    if (not yosys_linux_path) or (not nextpnr_linux_path) or (not openFPGALoader_linux_path):
        utils.error_exit('Did not find FPGA tools. Please run with --get_extern_tools to get/build them.')

    riscv_linux_path = os.path.exists('build_riscv_compiler/linux/bin')
    if (not riscv_linux_path):
        utils.error_exit('Did not find RISC-V compiler for your platform. Please run with --get_extern_tools to get/build it.')

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--config', required=True)
    parser.add_argument('--clean', action="store_true")
    parser.add_argument('--get_extern_tools', action="store_true")
    args = parser.parse_args()

    utils.init()
    config = json.load(open(args.config))

    check_external_tools(args, config)

    if args.get_extern_tools:
        get_or_build_extern_tools(args, config)

    build_presm(args, config)

        
    
    
