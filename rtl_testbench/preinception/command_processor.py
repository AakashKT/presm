import cocotb
from cocotb.triggers import RisingEdge, FallingEdge, Timer
from cocotb.clock import Clock

@cocotb.test()
async def command_processor(dut):
    freq = 27000000 
    baud_rate = 115200

    clk_ns = round(1e9 / freq, 2)
    bit_hold = round(clk_ns * float(freq) / baud_rate, 2)

    clk = Clock(dut.extern_clock, clk_ns, unit='ns')
    clk.start(start_high=False)

    await RisingEdge(dut.extern_clock)
    await Timer(clk_ns, unit='ns')
    
    dut.extern_reset.value = 1
    await Timer(clk_ns, unit='ns')
    dut.extern_reset.value = 0
    await Timer(clk_ns, unit='ns')