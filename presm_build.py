import os, argparse, shutil

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--driver', help='Which driver to use?', default='cuda', required=True)
    args = parser.parse_args()

    if os.path.exists('build'):
        shutil.rmtree('build')
    
    os.mkdir('build')
    os.chdir('build')

    os.system('cmake .. -DDRIVER=%s' % (args.driver.lower()))
    os.system('make -j8')