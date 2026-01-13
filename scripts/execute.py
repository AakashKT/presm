import os, argparse, shutil, json
import utils

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--config', required=True)
    parser.add_argument('--executable', required=True)
    parser.add_argument('--args', nargs='+', type=str)
    args = parser.parse_args()

    utils.init()
    config = json.load(open(args.config))

    if not args.args:
        app_args = []
    else:
        app_args = args.args

    working_directory, executable = os.path.split(args.executable)
    source = utils.presm_execute(working_directory, executable, app_args, config['driver']['name'])
    print(source)