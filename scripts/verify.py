import os, argparse, shutil, utils

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--driver', required=True)
    parser.add_argument('--app', required=True)
    args = parser.parse_args()

    utils.init()

    working_directory, executable = utils.get_working_directory_and_executable()

    target = utils.execute(working_directory, executable, [args.app])
    source = utils.presm_execute(working_directory, executable, [args.app], args.driver)

    utils.verify_output(source, target)