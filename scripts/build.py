import os, argparse, shutil, utils

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--driver', help='Which driver to use?', default='cuda', required=True)
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
        os.system('cmake .. -DDRIVER=%s' % (args.driver.lower()))
        
    os.system('cmake --build . --target install')
