import os, argparse, shutil, json
import utils

def setup(args, config, execution_dir):
    executable_dir, executable = os.path.split(args.executable)
    utils._copy_recursive(executable_dir, execution_dir)

    driver_lib_path, _ = utils.get_driver_lib(config['driver']['name'])
    utils._copy_file(driver_lib_path, execution_dir)
    driver_lib = driver_lib_path.split('/')[-1]

    utils._copy_file(args.config, execution_dir)

    return os.path.join(os.getcwd(), executable_dir), executable, driver_lib

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--config', required=True)
    parser.add_argument('--executable', required=True)
    parser.add_argument('--args', nargs='+', type=str)
    args = parser.parse_args()

    utils.init()
    config = json.load(open(args.config))

    execution_dir = 'execute_runs'
    execution_dir = utils.make_numbered_execution_dir(execution_dir)

    working_directory, executable, driver_lib = setup(args, config, execution_dir)

    if not args.args:
        app_args = []
    else:
        app_args = args.args

    source = utils.presm_execute(working_directory, executable, driver_lib, app_args, config['driver']['name'])
    print(source)