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

    dut.tx_packet[0].value = 1
    dut.tx_packet[1].value = 2
    dut.tx_packet[2].value = 0
    dut.tx_packet[3].value = 2
    
    dut.tx_packet[4].value = 2
    dut.tx_packet[5].value = 4
    dut.tx_packet[6].value = 0
    dut.tx_packet[7].value = 0

    dut.tx_packet_ready.value = 1
    await Timer(3*clk_ns, unit='ns')
    await Timer(1*bit_hold, unit='ns')

    assert dut.tx_packet_sent.value == 0
    
    bits_to_assert = 0b1000000010
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)
    assert dut.tx_packet_sent.value == 0

    bits_to_assert = 0b1000000100
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)
    assert dut.tx_packet_sent.value == 0

    bits_to_assert = 0b1000000000
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)
    assert dut.tx_packet_sent.value == 0

    bits_to_assert = 0b1000000100
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)
    assert dut.tx_packet_sent.value == 0

    bits_to_assert = 0b1000000100
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)
    assert dut.tx_packet_sent.value == 0

    bits_to_assert = 0b1000001000
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)
    assert dut.tx_packet_sent.value == 0

    bits_to_assert = 0b1000000000
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)
    assert dut.tx_packet_sent.value == 0

    bits_to_assert = 0b1000000000
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
    b0 = 0b1000000010
    b1 = 0b1000010000
    b2 = 0b1000000100
    b3 = 0b1000001000
    await send_rx_bits(dut, bit_hold, b0, 10)
    await send_rx_bits(dut, bit_hold, b1, 10)
    await send_rx_bits(dut, bit_hold, b2, 10)
    await send_rx_bits(dut, bit_hold, b3, 10)

    assert dut.rx_packet_ready.value == 0

    b0 = 0b1000010000
    b1 = 0b1000010010
    b2 = 0b1000001010
    b3 = 0b1000000110
    await send_rx_bits(dut, bit_hold, b0, 10)
    await send_rx_bits(dut, bit_hold, b1, 10)
    await send_rx_bits(dut, bit_hold, b2, 10)
    await send_rx_bits(dut, bit_hold, b3, 10)

    assert dut.rx_packet_ready.value == 1
    assert dut.rx_packet[0].value == 1
    assert dut.rx_packet[1].value == 8
    assert dut.rx_packet[2].value == 2
    assert dut.rx_packet[3].value == 4
    assert dut.rx_packet[4].value == 8
    assert dut.rx_packet[5].value == 9
    assert dut.rx_packet[6].value == 5
    assert dut.rx_packet[7].value == 3

    # Packet 2
    b0 = 0b1000000100
    b1 = 0b1000000110
    b2 = 0b1000000000
    b3 = 0b1000000010
    await send_rx_bits(dut, bit_hold, b0, 10)
    await send_rx_bits(dut, bit_hold, b1, 10)
    await send_rx_bits(dut, bit_hold, b2, 10)
    await send_rx_bits(dut, bit_hold, b3, 10)

    assert dut.rx_packet_ready.value == 0

    b0 = 0b1000010110
    await send_rx_bits(dut, bit_hold, b0, 10)
    await send_rx_bits(dut, bit_hold, b0, 10)
    await send_rx_bits(dut, bit_hold, b0, 10)
    await send_rx_bits(dut, bit_hold, b0, 10)

    assert dut.rx_packet_ready.value == 1
    assert dut.rx_packet[0].value == 2
    assert dut.rx_packet[1].value == 3
    assert dut.rx_packet[2].value == 0
    assert dut.rx_packet[3].value == 1
    assert dut.rx_packet[4].value == 11
    assert dut.rx_packet[5].value == 11
    assert dut.rx_packet[6].value == 11
    assert dut.rx_packet[7].value == 11
