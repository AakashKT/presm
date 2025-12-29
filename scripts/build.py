import os, argparse, shutil, utils

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--driver', help='Which driver to use? (see driver/ folder)', default='cuda', required=True)
    parser.add_argument('--device', help='Which device to use? (see device/ folder)', default='ptx_gpu_1', required=True)
    parser.add_argument('--clean', help='yes | no', default='no')
    args = parser.parse_args()

    utils.init()

    if args.clean.lower() == 'yes':
        shutil.rmtree('build')
        os.mkdir('build')
    
    if not os.path.exists('build'):
        os.mkdir('build')

    os.chdir('build')

    if args.clean.lower() == 'yes':
        os.system('cmake .. -DDRIVER=%s -DDEVICE=%s -DCMAKE_BUILD_TYPE=Release' % (args.driver.lower(), args.device.lower()))
        
    os.system('cmake --build . --config Release --target install')
