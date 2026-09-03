import cocotb
from cocotb.triggers import RisingEdge, FallingEdge, Timer
from cocotb.clock import Clock

async def assert_tx_bits(dut, bit_hold, bits, num_bits):
    for i in range(num_bits):
        bit = (bits >> i) & 1

        assert dut.extern_uart_tx.value == bit, "UART TX: expected %d, got %d at bit number: %d" % (bit, dut.extern_uart_tx.value, i+1)
        await Timer(bit_hold, unit='ns')

async def send_rx_bits(dut, bit_hold, bits, num_bits):
    for i in range(num_bits):
        bit = (bits >> i) & 1

        dut.extern_uart_rx.value = bit
        await Timer(bit_hold, unit='ns')

@cocotb.test()
async def uart_packet_transmit(dut):
    freq = 27000000 
    baud_rate = 9600

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

    dut.tx_packet_flat.value = 1 | 2 << 4 | 0 << 8 | 2 << 12 | 2 << 16 | 4 << 24 | 0 << 32 | 0 << 40

    dut.tx_packet_ready.value = 1
    await Timer(3*clk_ns, unit='ns')
    await Timer(1*bit_hold, unit='ns')

    assert dut.tx_packet_sent.value == 0
    
    bits_to_assert = ((1 | 2 << 4) | (1 << 8)) << 1
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)
    assert dut.tx_packet_sent.value == 0

    bits_to_assert = ((0 | 2 << 4) | (1 << 8)) << 1
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)
    assert dut.tx_packet_sent.value == 0

    bits_to_assert = (2 | (1 << 8)) << 1
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)
    assert dut.tx_packet_sent.value == 0

    bits_to_assert = (4 | (1 << 8)) << 1
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)
    assert dut.tx_packet_sent.value == 0

    bits_to_assert = (0 | (1 << 8)) << 1
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)
    assert dut.tx_packet_sent.value == 0

    bits_to_assert = (0 | (1 << 8)) << 1
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)
    
    assert dut.tx_packet_sent.value == 1

@cocotb.test()
async def uart_packet_receive(dut):
    freq = 27000000 
    baud_rate = 9600

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

    # Packet 1
    b0 = ((1 | 8 << 4) | (1 << 8)) << 1
    b1 = ((2 | 4 << 4) | (1 << 8)) << 1
    await send_rx_bits(dut, bit_hold, b0, 10)
    await send_rx_bits(dut, bit_hold, b1, 10)

    assert dut.rx_packet_ready.value == 0

    b0 = (8 | (1 << 8)) << 1
    b1 = (9 | (1 << 8)) << 1
    b2 = (5 | (1 << 8)) << 1
    b3 = (3 | (1 << 8)) << 1
    await send_rx_bits(dut, bit_hold, b0, 10)
    await send_rx_bits(dut, bit_hold, b1, 10)
    await send_rx_bits(dut, bit_hold, b2, 10)
    await send_rx_bits(dut, bit_hold, b3, 10)

    assert dut.rx_packet_ready.value == 1
    assert dut.rx_packet_flat.value == 1 | 8 << 4 | 2 << 8 | 4 << 12 | 8 << 16 | 9 << 24 | 5 << 32 | 3 << 40 

    # Packet 2
    b0 = ((2 | 3 << 4) | (1 << 8)) << 1
    b1 = ((0 | 1 << 4) | (1 << 8)) << 1
    await send_rx_bits(dut, bit_hold, b0, 10)
    await send_rx_bits(dut, bit_hold, b1, 10)

    assert dut.rx_packet_ready.value == 0

    b0 = (11 | (1 << 8)) << 1
    await send_rx_bits(dut, bit_hold, b0, 10)
    await send_rx_bits(dut, bit_hold, b0, 10)
    await send_rx_bits(dut, bit_hold, b0, 10)
    await send_rx_bits(dut, bit_hold, b0, 10)

    assert dut.rx_packet_ready.value == 1
    assert dut.rx_packet_flat.value == 2 | 3 << 4 | 0 << 8 | 1 << 12 | 11 << 16 | 11 << 24 | 11 << 32 | 11 << 40
