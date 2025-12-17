YOSYS_EXEC=./../../extern/yosys/yosys
NEXTPNR_EXEC=./../../extern/nextpnr/build/nextpnr-himbaechel
OPENFPGALOADER_EXEC=./../../extern/openFPGALoader/build/openFPGALoader

DEVICE=GW2AR-LV18QN88C8/I7
DEVICE_FAMILY=GW2A-18C
BOARD=tangnano20k

VERILOG_DIR=../../device/verilog_rtl/
VERILOG_SRC_DIR=$(VERILOG_DIR)/src/
PHYSICAL_CONSTRAINTS_DIR=$(VERILOG_DIR)/physical_constraints/

VERILOG_FILE=none.v
CST_FILE=none.cst
TOP_MODULE=none

all: $(TOP_MODULE).fs
	@echo "Programming FPGA $(DEVICE)"
	sudo $(OPENFPGALOADER_EXEC) -b ${BOARD} -f $(TOP_MODULE).fs

$(TOP_MODULE).fs: $(TOP_MODULE)_PNR.json
	@echo "Generating bitstream $(TOP_MODULE).fs"
	gowin_pack -d ${DEVICE_FAMILY} -o $(TOP_MODULE).fs $(TOP_MODULE)_PNR.json

$(TOP_MODULE)_PNR.json: $(TOP_MODULE)_SYNTH.json
	@echo "Generating place-and-route $(TOP_MODULE)_PNR.json"
	$(NEXTPNR_EXEC) --json $(TOP_MODULE)_SYNTH.json --write $(TOP_MODULE)_PNR.json --device $(DEVICE) --vopt cst=$(PHYSICAL_CONSTRAINTS_DIR)/$(CST_FILE) --vopt family=$(DEVICE_FAMILY)

$(TOP_MODULE)_SYNTH.json: 
	@echo "Generating netlist $(TOP_MODULE)_SYNTH.json"
	$(YOSYS_EXEC) -p "read_verilog $(VERILOG_SRC_DIR)/$(VERILOG_FILE); synth_gowin -top $(TOP_MODULE) -json $(TOP_MODULE)_SYNTH.json"

.INTERMEDIATE: $(TOP_MODULE).fs $(TOP_MODULE)_PNR.json $(TOP_MODULE)_SYNTH.json