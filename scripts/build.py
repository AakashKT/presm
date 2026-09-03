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

    if device_type == 'serial':
        baud = config['device']['serial_config']['baud_rate']
        d_mem_sz = int(config['device']['memory_size_in_bytes'])

    else:
        baud = 0
        d_mem_sz = 0

    utils._execute(f'cmake .. -DDRIVER={driver_name} -DDEVICE={device_name} -DDEVICE_TYPE={device_type} -DDEVICE_MEM_SIZE_IN_BYTES=\"{d_mem_sz}\" -DBAUD_RATE=\"{baud}\"')

    utils._execute('cmake --build . --config Release -- -j 4')
    utils._execute('cmake --build . --config Release --target install -- -j 4')

def build_fpga_toolchain(args, config):
    if len(os.listdir('extern')) == 0:
        utils.print_red('No submodules found. Did you reun git clone --recursive-submodules?')
        exit()

    # Build yosys (Synthesis)
    utils._chdir('extern/yosys')
    utils._execute('cmake -B build .')

    # Build nextpnr-himbaechel (place-and-route)
    utils._chdir('../nextpnr')
    if os.path.exists('build'):
        shutil.rmtree('build')
    os.mkdir('build')
    utils._chdir('build')

    utils._execute('cmake .. -DARCH=himbaechel -DHIMBAECHEL_UARCH=gowin')
    utils._execute('make -j4')

    # Build openFPGALoader (programming the FPGA)
    utils._chdir('../../openFPGALoader/')
    if os.path.exists('build'):
        shutil.rmtree('build')
    os.mkdir('build')
    utils._chdir('build')

    utils._execute('cmake ..')
    utils._execute('make -j4')

    utils._chdir('../../../')

def install_ubuntu_packages():
    utils._execute('sudo apt update')

    utils._execute('sudo apt-get install iverilog')

    utils._execute('sudo apt-get install autoconf automake autotools-dev \
                make lld libffi-dev libfl-dev pkg-config tcl-dev graphviz xdot libeigen3-dev \
                gzip libftdi1-2 libftdi1-dev libhidapi-hidraw0 libhidapi-dev libudev-dev g++ \
                curl python3 python3-pip python3-tomli libmpc-dev libmpfr-dev \
                libgmp-dev gawk build-essential bison flex texinfo gperf libtool \
                patchutils bc zlib1g-dev libexpat-dev ninja-build git cmake \
                libglib2.0-dev libslirp-dev libncurses-dev libreadline-dev \
                libboost-all-dev')
    utils._execute('pip install apycula cocotb')

def get_or_build_extern_tools(args, config):
    system_name = platform.system()

    install_ubuntu_packages()
    build_fpga_toolchain(args, config)

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

        
    
    
