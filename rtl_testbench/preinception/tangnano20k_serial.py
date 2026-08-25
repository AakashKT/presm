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
async def uart_interface_test(dut):
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

    b0 = 0b1000000010
    b1 = 0b1000000100
    b2 = 0b1000000000
    b3 = 0b1000000000
    await send_rx_bits(dut, bit_hold, b0, 10)
    await send_rx_bits(dut, bit_hold, b1, 10)
    await send_rx_bits(dut, bit_hold, b2, 10)
    await send_rx_bits(dut, bit_hold, b3, 10)

    b0 = 0b1000000110
    b1 = 0b1000001110
    await send_rx_bits(dut, bit_hold, b0, 10)
    await send_rx_bits(dut, bit_hold, b1, 10)

    assert dut.rx_packet_ready.value == 1

    bits_to_assert = 0b1000000010
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1000000100
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1000000000
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1000000000
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1000001100
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1000000110
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    assert dut.rx_packet_ready.value == 1
