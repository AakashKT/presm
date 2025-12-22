import os, argparse, shutil, utils

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    args = parser.parse_args()

    utils.init()

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