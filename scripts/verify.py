import os, argparse, shutil, utils, json

def verify_output(source, target):
    i = 0
    for line in target:
        if line != source[i]:
            return False
        i += 1
    
    return True

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--config', required=True)
    args = parser.parse_args()

    utils.init()
    config = json.load(open(args.config))

    working_directory, executable = utils.get_working_directory_and_executable()

    for app in config['verification']:
        args = [app['name']] + app['args_device_run']
        source = utils.presm_execute(working_directory, executable, args, config['driver']['name'])

        if app['verify']:
            args = [app['name']] + app['args_host_run']
            target = utils.execute(working_directory, executable, args)

            result = verify_output(source, target)
            if result:
                utils.print_green('Verification: ' + app['name'] + ' succeeded.')
                print('')
            else:
                utils.print_red('Verification: ' + app['name'] + ' failed.')
                print('\nSource output:')
                print(source)

                print('\nTarget output:')
                print(target)
                print('')
                exit(1)
        
        else:
            utils.print_green('Execution output ==>')
            print(source)