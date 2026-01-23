import os, argparse, shutil, utils, json

def setup(args, config, execution_dir):
    device = config['device']

    os.mkdir(execution_dir + '/output/')
    
    common_rtl_dir = 'device/rtl/'
    utils._copy_recursive(common_rtl_dir, execution_dir)

    common_testbench_dir = f'rtl_testbench/'
    utils._copy_files_only(common_testbench_dir, execution_dir)

    if device:
        device_name = device['name']

        device_rtl_dir = f'device/{device_name}/rtl/'
        utils._copy_recursive(device_rtl_dir, execution_dir)

        device_testbench_dir = f'rtl_testbench/{device_name}/'
        utils._copy_files_only(device_testbench_dir, execution_dir)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--config', required=True)
    args = parser.parse_args()

    utils.init()
    config = json.load(open(args.config))

    execution_dir = 'rtl_testbench_runs'
    execution_dir = utils.make_numbered_execution_dir(execution_dir)

    setup(args, config, execution_dir)

    os.chdir(execution_dir)

    for testbench in config['rtl_testbench']:
        test_name = testbench['name'].replace(' ', '_')
        rtl_src = testbench['rtl_src']
        testbench_src = testbench['testbench_src']

        os.system(f'iverilog -o output/{test_name}.o \
                            -s test \
                            {rtl_src} \
                            {testbench_src}')
        os.system(f'vvp output/{test_name}.o')