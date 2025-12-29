import os, argparse, shutil, utils

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--driver', help='Which driver to use? (see driver/ folder)', default='cuda', required=True)
    parser.add_argument('--device', help='Which device to use? (see device/ folder)', default='ptx_gpu_1', required=True)
    parser.add_argument('--clean', action="store_true")
    parser.add_argument('--static_lib', action="store_true")
    args = parser.parse_args()

    utils.init()

    if args.clean:
        shutil.rmtree('build')
        os.mkdir('build')
    
    path_exists = os.path.exists('build')
    if not path_exists:
        os.mkdir('build')

    os.chdir('build')

    if args.clean or (not path_exists):

        if args.static_lib:
            os.system('cmake .. -DPRESM_SHARED_LIB=FALSE -DDRIVER=%s -DDEVICE=%s -DCMAKE_BUILD_TYPE=Release' % (args.driver.lower(), args.device.lower()))
        else:
            os.system('cmake .. -DPRESM_SHARED_LIB=TRUE -DDRIVER=%s -DDEVICE=%s -DCMAKE_BUILD_TYPE=Release' % (args.driver.lower(), args.device.lower()))

    os.system('cmake --build . --config Release --target install')
