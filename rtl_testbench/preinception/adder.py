import cocotb
from cocotb.triggers import RisingEdge, FallingEdge, Timer
from cocotb.clock import Clock
import random

@cocotb.test()
async def adder(dut):
    TIMER_WAIT = 10
    
    dut.async_reset.value = 0
    dut.ra.value = 0
    dut.rb.value = 0
    dut.rdy.value = 0

    clk = Clock(dut.extern_clock, 1, unit='ns')
    clk.start(start_high=False)

    await RisingEdge(dut.extern_clock)
    await Timer(TIMER_WAIT, unit='ns')
    
    dut.async_reset.value = 1
    await Timer(TIMER_WAIT, unit='ns')
    dut.async_reset.value = 0
    await Timer(TIMER_WAIT, unit='ns')
    assert dut.rc_rdy.value == 0, "Async reset failed"

    for i in range(10):
        a = random.randint(-1000, 1000)
        b = random.randint(-1000, 1000)
        c = a + b

        dut.ra.value = a
        dut.rb.value = b
        dut.rdy.value = 1
        await Timer(TIMER_WAIT, unit='ns')
        
        dut_value = dut.rc.value.to_signed()
        assert dut.rc_rdy.value == 1
        assert dut_value == c, "(%d) %d + %d != %d" % (i, a, b, dut_value)

        dut.rdy.value = 0
        await Timer(TIMER_WAIT, unit='ns')