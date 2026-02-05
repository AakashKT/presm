import os, argparse, shutil, utils, json

def verify_output(source, target):
    i = 0
    for line in target:
        if line != source[i]:
            return False
        i += 1
    
    return True

def setup(args, config, execution_dir):
    verification_dir, verification_exe = utils.get_verification_dir_and_exe()
    utils._copy_files_only(verification_dir, execution_dir)

    driver_lib_path, driver_lib, _ = utils.get_driver_lib(config)
    utils._copy_file(f'{driver_lib_path}/{driver_lib}', execution_dir)

    utils._copy_file(args.config, execution_dir + '/presm_config.json')

    return verification_exe, driver_lib

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--config', required=True)
    args = parser.parse_args()

    utils.init()
    config = json.load(open(args.config))

    if 'verification' not in config:
        utils.error_exit('No verification apps defined!')
    
    utils.sanitize_presm_config(config)

    execution_dir = 'verify_runs'
    execution_dir = utils.make_numbered_execution_dir(execution_dir)

    executable, driver_lib = setup(args, config, execution_dir)

    for app in config['verification']:
        app_args = [app['name']] + app['args_device_run']
        source = utils.presm_execute(execution_dir, executable, driver_lib, \
                            app_args, config['driver']['name'])

        if app['verify']:
            app_args = [app['name']] + app['args_host_run']
            target = utils.execute(working_directory, executable, app_args)

            result = verify_output(source, target)
            if result:
                utils.print_green(f'Verification of {app["name"]} succeeded.')
                print('')
            else:
                utils.print_red(f'Verification of {app["name"]} failed.')
                print('\nSource output:')
                print(source)

                print('\nTarget output:')
                print(target)
                print('')
                exit(1)
        
        else:
            utils.print_green('Execution output ==>')
            print(source)