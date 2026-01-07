import os, argparse, shutil, utils, platform

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--driver', help='Which driver to use? (see driver/ folder)', default='cuda', required=True)
    parser.add_argument('--device', help='Which device to use? (see device/ folder)', default='ptx_gpu_1', required=True)
    parser.add_argument('--clean', action="store_true")
    parser.add_argument('--clean_extern_tools', action="store_true")
    parser.add_argument('--static_lib', action="store_true")
    args = parser.parse_args()

    utils.init()

    if args.static_lib:
        utils.print_red('Building a static library of PRESM is currently not supported.')
        exit()

    path_exists = os.path.exists('build')
    if args.clean and path_exists:
        shutil.rmtree('build')
    
    path_exists = os.path.exists('build')
    if not path_exists:
        os.mkdir('build')

    os.chdir('build')

    if args.clean or (not path_exists):

        if args.static_lib:
            os.system('cmake .. -DPRESM_SHARED_LIB=FALSE -DDRIVER=%s -DDEVICE=%s' % (args.driver.lower(), args.device.lower()))
        else:
            os.system('cmake .. -DPRESM_SHARED_LIB=TRUE -DDRIVER=%s -DDEVICE=%s' % (args.driver.lower(), args.device.lower()))

    os.system('cmake --build . --config Release')

    # Driver specific commands
    if args.driver.lower() == 'cuda':
        os.system('cmake --build . --config Release --target install')
    
    # Device specific commands
    if args.device.lower() == 'riscv_accel' and args.clean_extern_tools:
        if platform.system() != 'Linux':
            utils.print_red('riscv_accel device requires a linux system')
            exit()
        
        os.chdir('../')

        path_exists = os.path.exists('build_riscv_compiler')
        if not path_exists:
            os.mkdir('build_riscv_compiler')
        
        os.chdir('build_riscv_compiler')

        current_directory = os.getcwd()

        os.system('sudo apt-get install autoconf automake autotools-dev curl python3 python3-pip python3-tomli libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev ninja-build git cmake libglib2.0-dev libslirp-dev libncurses-dev')

        os.chdir('../extern/riscv-gnu-toolchain/')
        os.system('make clean')

        os.system('./configure --prefix=' 
                + current_directory + ' --with-arch=rv32im')
        os.system('make -j8')
        
    
    
