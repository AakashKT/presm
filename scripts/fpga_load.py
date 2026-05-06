import os, argparse, shutil, utils, json

def setup(args, config, execution_dir):
    device_name = config['device']['name']
    fpga_name = config['device']['fpga']['name']

    os.mkdir(execution_dir + '/rtl/')

    fpga_file_dir = f'fpga/{fpga_name}/'
    utils._copy_recursive(fpga_file_dir, execution_dir)

    common_rtl_dir = 'device/rtl/'
    utils._copy_recursive(common_rtl_dir, execution_dir + '/rtl/')

    device_rtl_dir = f'device/{device_name}/rtl/'
    utils._copy_recursive(device_rtl_dir, execution_dir + '/rtl/')

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--config', required=True)
    args = parser.parse_args()

    utils.init()
    config = json.load(open(args.config))
    
    utils.sanitize_presm_config(config)

    execution_dir = 'fpga_load_runs'
    execution_dir = utils.make_numbered_execution_dir(execution_dir)

    setup(args, config, execution_dir)

    yosys_exec = f'{os.getcwd()}/extern/yosys/yosys'
    nextpnr_exec = f'{os.getcwd()}/extern/nextpnr/build/nextpnr-himbaechel'
    openfpgaloader_exec = f'{os.getcwd()}/extern/openFPGALoader/build/openFPGALoader'

    top_module = config['device']['rtl']['top_module']
    top_module_src = config['device']['rtl']['top_module_src']

    os.chdir(execution_dir)
    os.system(f'make TOP_MODULE={top_module} \
                    TOP_MODULE_SRC={top_module_src} \
                    YOSYS_EXEC={yosys_exec} \
                    NEXTPNR_EXEC={nextpnr_exec} \
                    OPENFPGALOADER_EXEC={openfpgaloader_exec}')