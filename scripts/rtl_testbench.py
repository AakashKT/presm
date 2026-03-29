import os, argparse, shutil, utils, json

def setup(args, config, execution_dir):    
    common_rtl_dir = 'device/rtl/'
    utils._copy_recursive(common_rtl_dir, execution_dir)

    common_testbench_dir = f'rtl_testbench/'
    utils._copy_files_only(common_testbench_dir, execution_dir)

    try:
        device_name = config['device']['name']
        device_type = config['device']['type']
        
        if device_type != 'rtl_testbench':
            device_rtl_dir = f'device/{device_name}/rtl/'
            utils._copy_recursive(device_rtl_dir, execution_dir)

            device_testbench_dir = f'rtl_testbench/{device_name}/'
            utils._copy_files_only(device_testbench_dir, execution_dir)
    except:
        utils.print_red('Cannot include RTL testbenches for device.')

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--config', required=True)
    args = parser.parse_args()

    utils.init()
    config = json.load(open(args.config))

    try:
        tests = config['rtl_testbench']
    except:
        utils.error_exit('No RTL testbenches defined')
    
    utils.sanitize_presm_config(config)

    execution_dir = 'rtl_testbench_runs'
    execution_dir = utils.make_numbered_execution_dir(execution_dir)

    setup(args, config, execution_dir)

    for testbench in tests:
        if not testbench['enable']:
            continue

        rtl_src = testbench['rtl_src']
        rtl_top_level = testbench['rtl_top_level']

        test_folder = rtl_src.replace('.v', '').replace('.sv', '')
        utils._copy_files_only(f'rtl_testbench/{test_folder}', execution_dir)

    os.chdir(execution_dir)
    for testbench in tests:
        if not testbench['enable']:
            continue

        rtl_src = testbench['rtl_src']
        testbench_src = testbench['testbench_src']
        rtl_top_level = testbench['rtl_top_level']

        os.system(f'make VERILOG_SOURCES={rtl_src} \
                        COCOTB_TOPLEVEL={rtl_top_level} \
                        COCOTB_TEST_MODULES={testbench_src}')