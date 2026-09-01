import os, argparse, shutil, sys, json
sys.path.append('scripts/')
import utils

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--config', required=True)
    parser.add_argument('--export_dir', required=True)
    args = parser.parse_args()

    utils.init()
    config = json.load(open(args.config))

    try:
        tests = config['rtl_testbench']
    except:
        utils.error_exit('Did not find any RTL sources & testbenches')
    
    verilog_dst_dir = f'{args.export_dir}/src/'
    testbench_dst_file = f'{args.export_dir}/test/test.py'

    try:
        device_name = config['device']['name']

        rtl_dir = f'device/rtl/'
        utils._copy_recursive(rtl_dir, verilog_dst_dir)
            
        if device_name != '':
            rtl_dir = f'device/{device_name}/rtl/'
            utils._copy_recursive(rtl_dir, verilog_dst_dir)
        
        testbench_src = tests[0]['testbench_src']
        testbench_src = f'rtl_testbench/{device_name}/{testbench_src}'
        utils._copy_file(testbench_src, testbench_dst_file)

    except:
        utils.print_red('Cannot export RTL sources & testbenches')