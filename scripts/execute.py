import os, argparse, shutil, json
import utils

def setup(args, config, execution_dir):
    executable_dir, executable = os.path.split(args.executable)
    utils._copy_recursive(executable_dir, execution_dir)

    driver_lib_path, driver_lib, _ = utils.get_driver_lib(config)
    utils._copy_file(f'{driver_lib_path}/{driver_lib}', execution_dir)

    utils._copy_file(args.config, execution_dir + '/hw_config.json')

    return executable, driver_lib

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

    executable, driver_lib = setup(args, config, execution_dir)

    if not args.args:
        app_args = []
    else:
        app_args = args.args

    source = utils.presm_execute(execution_dir, executable, driver_lib, \
                            app_args, config['driver']['name'])

    utils.print_green('Execution output ==>')
    print(source)