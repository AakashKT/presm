import cocotb
from cocotb.triggers import RisingEdge, FallingEdge, Timer
from cocotb.clock import Clock

@cocotb.test()
async def d_flip_flop_rst_test(dut):
    dut.async_reset.value = 0
    dut.inp.value = 0

    clk = Clock(dut.extern_clock, 10, unit='ns')
    clk.start(start_high=False)

    await RisingEdge(dut.extern_clock)

    await Timer(20, unit='ns')

    dut.async_reset.value = 1
    await Timer(20, unit='ns')
    dut.async_reset.value = 0
    await Timer(20, unit='ns')
    assert dut.op.value == 0, "Async Reset FF failed"

    dut.inp.value = 1
    await Timer(20, unit='ns')
    assert dut.op.value == 1, "Set '1' failed"

    dut.inp.value = 0
    await Timer(20, unit='ns')
    assert dut.op.value == 0, "Set '0' failed"

    dut.inp.value = 1
    await Timer(20, unit='ns')
    assert dut.op.value == 1, "Set '1' failed"
    
