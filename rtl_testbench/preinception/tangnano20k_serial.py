import cocotb
from cocotb.triggers import RisingEdge, FallingEdge, Timer
from cocotb.clock import Clock

async def assert_tx_bits(dut, bit_hold, bits, num_bits):
    for i in range(num_bits):
        bit = (bits >> i) & 1

        assert dut.extern_uart_tx.value == bit, "UART TX: expected %d, got %d at bit number: %d" % (bit, dut.extern_uart_tx.value, i)
        await Timer(bit_hold, unit='ns')

async def send_rx_bits(dut, bit_hold, bits, num_bits):
    for i in range(num_bits):
        bit = (bits >> i) & 1

        dut.extern_uart_rx.value = bit
        await Timer(bit_hold, unit='ns')

@cocotb.test()
async def uart_interface_add_cmd(dut):
    freq = 27000000 
    baud_rate = 115200

    clk_ns = round(1e9 / freq, 2)
    bit_hold = round(clk_ns * float(freq) / baud_rate, 2)

    dut.extern_reset.value = 0
    dut.dword_write_flag.value = 0

    clk = Clock(dut.extern_clock, clk_ns, unit='ns')
    clk.start(start_high=False)

    await RisingEdge(dut.extern_clock)
    await Timer(clk_ns, unit='ns')

    dut.extern_reset.value = 1
    await Timer(clk_ns, unit='ns')
    dut.extern_reset.value = 0
    await Timer(clk_ns, unit='ns')

    # Handshake
    i = 0b1011010010
    d = 0b1011001000
    e = 0b1011001010
    n = 0b1011011100
    await send_rx_bits(dut, bit_hold, i, 10)
    await send_rx_bits(dut, bit_hold, d, 10)
    await send_rx_bits(dut, bit_hold, e, 10)
    await send_rx_bits(dut, bit_hold, n, 10)

    a = 0b1011000010
    d = 0b1011001000
    i = 0b1011010010
    await send_rx_bits(dut, bit_hold, a, 10)
    await send_rx_bits(dut, bit_hold, d, 10)
    await send_rx_bits(dut, bit_hold, d, 10)
    await send_rx_bits(dut, bit_hold, i, 10)

    two = 0b1000000100
    three = 0b1000000110
    five = 0b1000001010
    zero = 0b1000000000
    await send_rx_bits(dut, bit_hold, two, 10)
    await send_rx_bits(dut, bit_hold, zero, 10)
    await send_rx_bits(dut, bit_hold, zero, 10)
    await send_rx_bits(dut, bit_hold, zero, 10)

    await send_rx_bits(dut, bit_hold, three, 10)
    await send_rx_bits(dut, bit_hold, zero, 10)
    await send_rx_bits(dut, bit_hold, zero, 10)
    await send_rx_bits(dut, bit_hold, zero, 10)

    assert dut.dword_write_flag.value == 1
    assert dut.dword_write.value == 5

    await assert_tx_bits(dut, bit_hold, five, 10)
    await assert_tx_bits(dut, bit_hold, zero, 10)
    await assert_tx_bits(dut, bit_hold, zero, 10)
    await assert_tx_bits(dut, bit_hold, zero, 10)

@cocotb.test()
async def uart_interface_write(dut):
    freq = 27000000 
    baud_rate = 115200

    clk_ns = round(1e9 / freq, 2)
    bit_hold = round(clk_ns * float(freq) / baud_rate, 2)

    dut.extern_reset.value = 0
    dut.dword_write_flag.value = 0

    clk = Clock(dut.extern_clock, clk_ns, unit='ns')
    clk.start(start_high=False)

    await RisingEdge(dut.extern_clock)
    await Timer(clk_ns, unit='ns')

    dut.extern_reset.value = 1
    await Timer(clk_ns, unit='ns')
    dut.extern_reset.value = 0
    await Timer(clk_ns, unit='ns')

    # Handshake
    i = 0b1011010010
    d = 0b1011001000
    e = 0b1011001010
    n = 0b1011011100
    await send_rx_bits(dut, bit_hold, i, 10)
    await send_rx_bits(dut, bit_hold, d, 10)
    await send_rx_bits(dut, bit_hold, e, 10)
    await send_rx_bits(dut, bit_hold, n, 10)

    dut.dword_write.value = 0b10101111101011111010111110101111
    dut.dword_write_flag.value = 1
    await Timer(bit_hold, unit='ns')

    bits_to_assert = 0b1101011110
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    dut.dword_write_flag.value = 0
    await Timer(bit_hold, unit='ns')

    dut.dword_write.value = 0b01110010011100100111001001110010
    dut.dword_write_flag.value = 1
    await Timer(bit_hold, unit='ns')

    bits_to_assert = 0b1011100100
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)


@cocotb.test()
async def uart_interface_read(dut):
    freq = 27000000 
    baud_rate = 115200

    clk_ns = round(1e9 / freq, 2)
    bit_hold = round(clk_ns * float(freq) / baud_rate, 2)

    dut.extern_reset.value = 0
    dut.extern_uart_rx.value = 1

    clk = Clock(dut.extern_clock, clk_ns, unit='ns')
    clk.start(start_high=False)

    await RisingEdge(dut.extern_clock)
    await Timer(clk_ns, unit='ns')

    dut.extern_reset.value = 1
    await Timer(clk_ns, unit='ns')
    dut.extern_reset.value = 0
    await Timer(clk_ns, unit='ns')

    # Handshake
    i = 0b1011010010
    d = 0b1011001000
    e = 0b1011001010
    n = 0b1011011100
    await send_rx_bits(dut, bit_hold, i, 10)
    await send_rx_bits(dut, bit_hold, d, 10)
    await send_rx_bits(dut, bit_hold, e, 10)
    await send_rx_bits(dut, bit_hold, n, 10)

    bit_to_send = 0b1101010010
    await send_rx_bits(dut, bit_hold, bit_to_send, 10)
    await send_rx_bits(dut, bit_hold, bit_to_send, 10)
    await send_rx_bits(dut, bit_hold, bit_to_send, 10)
    await send_rx_bits(dut, bit_hold, bit_to_send, 10)

    assert dut.dword_read_flag.value == 1
    assert dut.dword_read.value == 0b10101001101010011010100110101001

    bit_to_send = 0b1101111000
    await send_rx_bits(dut, bit_hold, bit_to_send, 10)
    await send_rx_bits(dut, bit_hold, bit_to_send, 10)
    await send_rx_bits(dut, bit_hold, bit_to_send, 10)
    await send_rx_bits(dut, bit_hold, bit_to_send, 10)

    assert dut.dword_read_flag.value == 1
    assert dut.dword_read.value == 0b10111100101111001011110010111100
