import os, argparse, shutil, utils

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--driver', required=True)
    parser.add_argument('--app', required=True)
    parser.add_argument('--device', default='host')
    args = parser.parse_args()

    utils.init()

    working_directory, executable = utils.get_working_directory_and_executable(args.driver)

    if args.device == 'presm':
        op = utils.presm_execute(working_directory, executable, [args.app], args.driver)
    else:
        op = utils.execute(working_directory, executable, [args.app])

    print('')
    utils.print_green('Execution output ==>')
    print(op)