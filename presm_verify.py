import os, argparse, shutil, utils

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--driver', required=True)
    parser.add_argument('--app', required=True)
    args = parser.parse_args()

    if args.driver == 'cuda':
        working_directory = os.getcwd() + '/build/verification/cuda/'
        executable = 'verification'
    
    else:
        utils.print_red('Verification does not exist.')
        exit(-1)

    target = utils.execute(working_directory, executable, [args.app])
    source = utils.presm_execute(working_directory, executable, [args.app], args.driver)

    utils.verify_output(source, target)