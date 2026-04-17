import cocotb
from cocotb.triggers import RisingEdge, FallingEdge, Timer
from cocotb.clock import Clock

@cocotb.test()
async def command_processor(dut):
    TIMER_WAIT = 10
    
    dut.async_reset.value = 0
    dut.in_host.value = 0
    dut.in_en.value = 0

    clk = Clock(dut.extern_clock, 1, unit='ns')
    clk.start(start_high=False)

    await RisingEdge(dut.extern_clock)
    await Timer(TIMER_WAIT, unit='ns')
    
    dut.async_reset.value = 1
    await Timer(TIMER_WAIT, unit='ns')
    dut.async_reset.value = 0
    await Timer(TIMER_WAIT, unit='ns')

    # Handshake
    dut.in_host.value = 0x6e656469
    dut.in_en.value = 1
    await Timer(TIMER_WAIT, unit='ns')

    dut.in_host.value = 0x69646461
    dut.in_en.value = 1
    await Timer(TIMER_WAIT, unit='ns')

    dut.in_en.value = 0
    await Timer(TIMER_WAIT, unit='ns')
    dut.in_host.value = 25
    dut.in_en.value = 1
    await Timer(TIMER_WAIT, unit='ns')

    dut.in_en.value = 0
    await Timer(TIMER_WAIT, unit='ns')
    dut.in_host.value = 50
    dut.in_en.value = 1
    await Timer(TIMER_WAIT, unit='ns')

    dut.in_en.value = 0
    await Timer(TIMER_WAIT, unit='ns')
    dut.in_host.value = 75
    dut.in_en.value = 1
    await Timer(TIMER_WAIT, unit='ns')

    assert dut.out_en.value == 1
    assert dut.out_host.value == 0x726d656d # memr assert

    dut.out_ack.value = 1
    dut.in_en.value = 0
    await Timer(1, unit='ns')
    dut.out_ack.value = 0
    await Timer(TIMER_WAIT, unit='ns')
    
    assert dut.out_en.value == 1
    assert dut.out_host.value == 25 # Address 1 assert

    dut.out_ack.value = 1
    dut.in_en.value = 0
    await Timer(1, unit='ns')
    dut.in_en.value = 1
    dut.in_host.value = 40
    await Timer(TIMER_WAIT, unit='ns')
    
    dut.out_ack.value = 1
    await Timer(1, unit='ns')
    dut.out_ack.value = 0
    await Timer(TIMER_WAIT, unit='ns')
    
    assert dut.out_en.value == 1
    assert dut.out_host.value == 50 # Address 2 assert

    dut.out_ack.value = 1
    dut.in_en.value = 0
    await Timer(1, unit='ns')
    dut.in_en.value = 1
    dut.in_host.value = 40
    await Timer(7, unit='ns')

    assert dut.mem_addr.value == 75
    
    dut.out_ack.value = 0
    await Timer(1, unit='ns')
    assert dut.out_en.value == 1
    assert dut.out_host.value == 0x776d656d # memw assert

    dut.out_ack.value = 1
    await Timer(1, unit='ns')

    dut.out_ack.value = 0
    await Timer(1, unit='ns')
    assert dut.out_en.value == 1
    assert dut.out_host.value == 75 # Address

    dut.out_ack.value = 1
    await Timer(1, unit='ns')

    dut.out_ack.value = 0
    await Timer(1, unit='ns')
    assert dut.out_en.value == 1
    assert dut.out_host.value == 80 # Addition result
