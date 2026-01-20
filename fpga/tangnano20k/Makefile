PRESM_DEVICE=""
TOP_MODULE_SRC=none.v
TOP_MODULE=none

YOSYS_EXEC=./../../extern/yosys/yosys
NEXTPNR_EXEC=./../../extern/nextpnr/build/nextpnr-himbaechel
OPENFPGALOADER_EXEC=./../../extern/openFPGALoader/build/openFPGALoader

DEVICE=GW2AR-LV18QN88C8/I7
DEVICE_FAMILY=GW2A-18C
BOARD=tangnano20k

CST_SRC=constraints.cst
VERILOG_DIR=../../device/${PRESM_DEVICE}/verilog/
VERILOG_SRC_DIR=$(VERILOG_DIR)/

all: GEN_BITSTREAM
	@echo "Programming FPGA $(DEVICE)"
	sudo $(OPENFPGALOADER_EXEC) -b ${BOARD} -f $(TOP_MODULE).fs

GEN_BITSTREAM: PLACE_AND_ROUTE
	@echo "Generating bitstream $(TOP_MODULE).fs"
	gowin_pack -d ${DEVICE_FAMILY} -o $(TOP_MODULE).fs $(TOP_MODULE)_PNR.json

PLACE_AND_ROUTE: SYNTHESIS
	@echo "Generating place-and-route $(TOP_MODULE)_PNR.json"
	$(NEXTPNR_EXEC) --json $(TOP_MODULE)_SYNTH.json --write $(TOP_MODULE)_PNR.json --device $(DEVICE) --vopt cst=$(CST_SRC) --vopt family=$(DEVICE_FAMILY)

SYNTHESIS: 
	@echo "Synthesizing: $(TOP_MODULE)_SYNTH.json"
	$(YOSYS_EXEC) -p "read_verilog $(VERILOG_SRC_DIR)/$(TOP_MODULE_SRC); synth_gowin -top $(TOP_MODULE) -json $(TOP_MODULE)_SYNTH.json"

.INTERMEDIATE: $(TOP_MODULE).fs $(TOP_MODULE)_PNR.json $(TOP_MODULE)_SYNTH.json