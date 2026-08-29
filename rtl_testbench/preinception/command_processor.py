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

    # Handshake command
    dut.rx_packet.value = 1 | 0 << 8 | 1 << 16 | 0 << 24
    dut.rx_packet_ready.value = 1
    await Timer(8*clk_ns, unit='ns')

    assert dut.tx_packet.value == 1 | 1 << 8 | 1 << 16 | 0 << 24 | 2 << 32 | 1 << 40
    assert dut.tx_packet_ready.value == 1

    dut.tx_packet_sent.value = 1
    await Timer(2*clk_ns, unit='ns')

    ##########
    # Add 1
    ##########
    VAL_1 = 65
    ADDR_1 = 10

    VAL_2 = 43
    ADDR_2 = 25

    ADDR_3 = 44

    # OP 1
    dut.rx_packet_ready.value = 0
    await Timer(clk_ns, unit='ns')

    dut.rx_packet.value = 1 | 0 << 8 | 2 << 16 | 0 << 24 | ADDR_1 << 32
    dut.rx_packet_ready.value = 1
    await Timer(8*clk_ns, unit='ns')
    
    assert dut.tx_packet_ready.value == 1
    assert dut.tx_packet.value == 1 | 0 << 8 | 0 << 16 | 0 << 24 | ADDR_1 << 32

    dut.tx_packet_sent.value = 1
    await Timer(2*clk_ns, unit='ns')

    dut.rx_packet_ready.value = 0
    await Timer(clk_ns, unit='ns')

    dut.rx_packet.value = 1 | 1 << 8 | 0 << 16 | 0 << 24 | VAL_1 << 32
    dut.rx_packet_ready.value = 1
    await Timer(8*clk_ns, unit='ns')

    assert dut.tx_packet_ready.value == 1
    assert dut.tx_packet.value == 1 | 1 << 8 | 2 << 16 | 0 << 24

    dut.tx_packet_sent.value = 1
    await Timer(2*clk_ns, unit='ns')

    # OP 2
    dut.rx_packet_ready.value = 0
    await Timer(clk_ns, unit='ns')

    dut.rx_packet.value = 2 | 0 << 8 | 2 << 16 | 1 << 24 | ADDR_2 << 32
    dut.rx_packet_ready.value = 1
    await Timer(8*clk_ns, unit='ns')
    
    assert dut.tx_packet_ready.value == 1
    assert dut.tx_packet.value == 2 | 0 << 8 | 0 << 16 | 0 << 24 | ADDR_2 << 32

    dut.tx_packet_sent.value = 1
    await Timer(2*clk_ns, unit='ns')

    dut.rx_packet_ready.value = 0
    await Timer(clk_ns, unit='ns')

    dut.rx_packet.value = 2 | 1 << 8 | 0 << 16 | 0 << 24 | VAL_2 << 32
    dut.rx_packet_ready.value = 1
    await Timer(8*clk_ns, unit='ns')

    assert dut.tx_packet_ready.value == 1
    assert dut.tx_packet.value == 2 | 1 << 8 | 2 << 16 | 1 << 24

    dut.tx_packet_sent.value = 1
    await Timer(2*clk_ns, unit='ns')

    # OP 3
    dut.rx_packet_ready.value = 0
    await Timer(clk_ns, unit='ns')

    dut.rx_packet.value = 3 | 0 << 8 | 2 << 16 | 2 << 24 | ADDR_3 << 32
    dut.rx_packet_ready.value = 1
    await Timer(8*clk_ns, unit='ns')
    
    assert dut.tx_packet_ready.value == 1
    assert dut.tx_packet.value == 3 | 0 << 8 | 0 << 16 | 0 << 24 | ADDR_3 << 32

    dut.tx_packet_sent.value = 1
    await Timer(6*clk_ns, unit='ns')

    assert dut.tx_packet_ready.value == 1
    assert dut.tx_packet.value == 3 | 0 << 8 | 0 << 16 | 0 << 24 | (VAL_1 + VAL_2) << 32

    dut.tx_packet_sent.value = 1
    await Timer(2*clk_ns, unit='ns')

    dut.rx_packet_ready.value = 0
    await Timer(clk_ns, unit='ns')

    dut.rx_packet.value = 3 | 1 << 8 | 0 << 16 | 0 << 24
    dut.rx_packet_ready.value = 1
    await Timer(8*clk_ns, unit='ns')

    dut.tx_packet_sent.value = 1
    await Timer(2*clk_ns, unit='ns')

    assert dut.tx_packet_ready.value == 1
    assert dut.tx_packet.value == 3 | 1 << 8 | 2 << 16 | 2 << 24
    assert dut.cp_state.value == 6 # CP_STOP