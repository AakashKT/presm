import cocotb
from cocotb.triggers import RisingEdge, FallingEdge, Timer
from cocotb.clock import Clock

@cocotb.test()
async def command_processor(dut):
    clk = Clock(dut.extern_clock, 1, unit='ns')
    clk.start(start_high=False)

    await RisingEdge(dut.extern_clock)
    await Timer(TIMER_WAIT, unit='ns')
    
    dut.extern_reset.value = 1
    await Timer(TIMER_WAIT, unit='ns')
    dut.extern_reset.value = 0
    await Timer(TIMER_WAIT, unit='ns')

    # Handshake
    dut.rx_packet[0]