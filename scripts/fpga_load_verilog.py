import os, argparse, shutil, utils, json

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--config', required=True)
    args = parser.parse_args()

    utils.init()
    config = json.load(open(args.config))

    fpga_name = config['fpga']['name']
    device_name = config['device']['name']
    top_module = config['device']['verilog']['top_module']
    top_module_src = config['device']['verilog']['top_module_src']

    os.chdir('fpga/' + fpga_name)

    os.system('make PRESM_DEVICE=%s TOP_MODULE=%s TOP_MODULE_SRC=%s' \
                    % (device_name, top_module, top_module_src))