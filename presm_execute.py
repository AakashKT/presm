import os, argparse, shutil
import utils

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--driver', help='Which driver to use?', default='cuda', required=True)
    parser.add_argument('--working_directory', required=True)
    parser.add_argument('--executable', required=True)
    parser.add_argument('--args', nargs='+', type=str)
    args = parser.parse_args()

    source = utils.presm_execute(args.working_directory, args.executable, args.args, args.driver)
    print(source)