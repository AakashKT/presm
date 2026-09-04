import os, argparse, shutil, utils, json

def setup(args, device_name, device_type, testbench, execution_dir):
    try:
        utils._copy_file('rtl_testbench/Makefile', execution_dir)

        rtl_dir = f'device/rtl/'
        utils._copy_recursive(rtl_dir, execution_dir)
        
        if device_name != '':
            rtl_dir = f'device/{device_name}/rtl/'
            utils._copy_recursive(rtl_dir, execution_dir)

        testbench_src = testbench['testbench_src']
        testbench_dir = f'rtl_testbench/{device_name}/{testbench_src}'
        utils._copy_file(testbench_dir, execution_dir)

    except:
        utils.print_red('Cannot include RTL testbenches for device.')

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--config', required=True)
    args = parser.parse_args()

    utils.init()
    config = json.load(open(args.config))

    try:
        device_name = config['device']['name']
        device_type = config['device']['type']

        tests = config['rtl_testbench']
        sim = config["device"]["rtl_testbench_config"]["simulator"]
        lang = config["device"]["rtl_testbench_config"]["language"]

    except KeyError as e:
        utils.error_exit(f"Error: The key {e} does not exist in the dictionary.")
    
    execution_dir_top = 'rtl_testbench_runs'

    for testbench in tests:

        try:
            rtl_src = testbench['rtl_src']
            testbench_src = testbench['testbench_src'].replace('.py', '')
            rtl_top_level = testbench['rtl_top_level']
            enable = testbench['enable']

        except KeyError as e:
            utils.error_exit(f"Error: The key {e} does not exist in rtl_testbench list.")

        if not enable:
            continue

        execution_dir = utils.make_numbered_execution_dir(execution_dir_top)
        setup(args, device_name, device_type, testbench, execution_dir)

        os.chdir(execution_dir)
        os.system(f'make VERILOG_SOURCES={rtl_src} \
                        COCOTB_TOPLEVEL={rtl_top_level} \
                        COCOTB_TEST_MODULES={testbench_src}')
        os.chdir('../../')