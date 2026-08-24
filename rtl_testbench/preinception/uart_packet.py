import cocotb
from cocotb.triggers import RisingEdge, FallingEdge, Timer
from cocotb.clock import Clock

async def send_rx_bits(dut, bit_hold, bits, num_bits):
    for i in range(num_bits):
        bit = (bits >> i) & 1

        dut.extern_uart_rx.value = bit
        await Timer(bit_hold, unit='ns')

@cocotb.test()
async def uart_interface_write(dut):
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

    # Packet 1
    b0 = 0b1000000010
    b1 = 0b1000010000
    b2 = 0b1000000000
    b3 = 0b1000000010
    await send_rx_bits(dut, bit_hold, b0, 10)
    await send_rx_bits(dut, bit_hold, b1, 10)
    await send_rx_bits(dut, bit_hold, b2, 10)
    await send_rx_bits(dut, bit_hold, b3, 10)

    assert dut.rx_packet_ready.value == 0

    b0 = 0b1000000010
    b1 = 0b1000000100
    b2 = 0b1000001000
    b3 = 0b1000010000
    await send_rx_bits(dut, bit_hold, b0, 10)
    await send_rx_bits(dut, bit_hold, b1, 10)
    await send_rx_bits(dut, bit_hold, b2, 10)
    await send_rx_bits(dut, bit_hold, b3, 10)

    b0 = 0b1000000010
    b1 = 0b1000000100
    b2 = 0b1000001000
    b3 = 0b1000010000
    await send_rx_bits(dut, bit_hold, b0, 10)
    await send_rx_bits(dut, bit_hold, b1, 10)
    await send_rx_bits(dut, bit_hold, b2, 10)
    await send_rx_bits(dut, bit_hold, b3, 10)

    assert dut.rx_packet_ready.value == 1
    assert dut.rx_packet[1].value == 8
    assert dut.rx_packet[5].value == 0b00000010
    assert dut.rx_packet[10].value == 0b00000100

    # Packet 2
    b0 = 0b1000000010
    b1 = 0b1000010000
    b2 = 0b1000000000
    b3 = 0b1000000010
    await send_rx_bits(dut, bit_hold, b0, 10)
    await send_rx_bits(dut, bit_hold, b1, 10)
    await send_rx_bits(dut, bit_hold, b2, 10)
    await send_rx_bits(dut, bit_hold, b3, 10)

    assert dut.rx_packet_ready.value == 0

    b0 = 0b1000000010
    b1 = 0b1000000100
    b2 = 0b1000001000
    b3 = 0b1000010000
    await send_rx_bits(dut, bit_hold, b0, 10)
    await send_rx_bits(dut, bit_hold, b1, 10)
    await send_rx_bits(dut, bit_hold, b2, 10)
    await send_rx_bits(dut, bit_hold, b3, 10)

    b0 = 0b1000000010
    b1 = 0b1000000100
    b2 = 0b1000001000
    b3 = 0b1000010000
    await send_rx_bits(dut, bit_hold, b0, 10)
    await send_rx_bits(dut, bit_hold, b1, 10)
    await send_rx_bits(dut, bit_hold, b2, 10)
    await send_rx_bits(dut, bit_hold, b3, 10)

    assert dut.rx_packet_ready.value == 1
    assert dut.rx_packet[1].value == 8
    assert dut.rx_packet[5].value == 0b00000010
    assert dut.rx_packet[10].value == 0b00000100
