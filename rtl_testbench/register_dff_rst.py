import cocotb
from cocotb.triggers import RisingEdge, FallingEdge, Timer
from cocotb.clock import Clock

@cocotb.test()
async def register_dff_rst(dut):
    dut.async_reset.value = 0
    dut.inp.value = 0

    clk = Clock(dut.extern_clock, 1, unit='ns')
    clk.start(start_high=False)

    await RisingEdge(dut.extern_clock)

    await Timer(2, unit='ns')

    dut.async_reset.value = 1
    await Timer(2, unit='ns')
    dut.async_reset.value = 0
    await Timer(2, unit='ns')
    assert dut.op.value == 0, "Register async reset failed"

    dut.inp.value = 0b10101010
    await Timer(2, unit='ns')
    assert dut.op.value == 0b10101010, "Register set failed"
    
