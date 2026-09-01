import cocotb
from cocotb.triggers import ClockCycles, Timer
from cocotb.clock import Clock

async def assert_tx_bits(dut, bit_hold, bits, num_bits):
    for i in range(num_bits):
        bit = (bits >> i) & 1

        assert int(dut.uo_out.value) >> 4 == bit, "UART TX: expected %d, got %d at bit number: %d" % (bit, dut.uo_out.value[4], i)
        await Timer(bit_hold, unit='ns')

async def send_rx_bits(dut, bit_hold, bits, num_bits):
    for i in range(num_bits):
        bit = (bits >> i) & 1

        dut.ui_in.value = bit << 3
        await Timer(bit_hold, unit='ns')

@cocotb.test()
async def basic_test(dut):
    freq = 50000000 
    baud_rate = 9600

    clk_ns = round(1e9 / freq, 2)
    bit_hold = round(clk_ns * float(freq) / baud_rate, 2)

    clk = Clock(dut.clk, clk_ns, unit='ns')
    cocotb.start_soon(clk.start())

    # Reset
    dut._log.info("Reset")
    dut.ena.value = 0
    dut.ui_in.value = 0
    dut.uio_in.value = 0
    dut.rst_n.value = 0
    await ClockCycles(dut.clk, 10)
    dut.rst_n.value = 1
    await ClockCycles(dut.clk, 10)

    dut.ena.value = 1
    dut.ui_in.value = 1 << 3
    await ClockCycles(dut.clk, 10)

    assert int(dut.ui_in.value) >> 3 == 1
    assert int(dut.uo_out.value) >> 4 == 1

@cocotb.test()
async def handshake_test(dut):
    freq = 50000000 
    baud_rate = 9600

    clk_ns = round(1e9 / freq, 2)
    bit_hold = round(clk_ns * float(freq) / baud_rate, 2)

    clk = Clock(dut.clk, clk_ns, unit='ns')
    cocotb.start_soon(clk.start())

    # Reset
    dut._log.info("Reset")
    dut.ena.value = 0
    dut.ui_in.value = 0
    dut.uio_in.value = 0
    dut.rst_n.value = 0
    await ClockCycles(dut.clk, 10)
    dut.rst_n.value = 1
    await ClockCycles(dut.clk, 10)

    dut.ena.value = 1
    dut.ui_in.value = 1 << 3
    await ClockCycles(dut.clk, 10)

    b0 = 0b1000000010
    b1 = 0b1000000000
    b2 = 0b1000000010
    b3 = 0b1000000000
    await send_rx_bits(dut, bit_hold, b0, 10)
    await send_rx_bits(dut, bit_hold, b1, 10)
    await send_rx_bits(dut, bit_hold, b2, 10)
    await send_rx_bits(dut, bit_hold, b3, 10)
    await send_rx_bits(dut, bit_hold, b3, 10)
    await send_rx_bits(dut, bit_hold, b3, 10)
    await send_rx_bits(dut, bit_hold, b3, 10)
    await send_rx_bits(dut, bit_hold, b3, 10)
    
    bits_to_assert = 0b1000000010
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1000000010
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1000000010
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1000000000
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1000000100
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1000000010
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1000000000
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1000000000
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)
    

@cocotb.test()
async def op1_test(dut):
    freq = 50000000 
    baud_rate = 9600

    clk_ns = round(1e9 / freq, 2)
    bit_hold = round(clk_ns * float(freq) / baud_rate, 2)

    clk = Clock(dut.clk, clk_ns, unit='ns')
    cocotb.start_soon(clk.start())

    # Reset
    dut._log.info("Reset")
    dut.ena.value = 0
    dut.ui_in.value = 0
    dut.uio_in.value = 0
    dut.rst_n.value = 0
    await ClockCycles(dut.clk, 10)
    dut.rst_n.value = 1
    await ClockCycles(dut.clk, 10)

    dut.ena.value = 1
    dut.ui_in.value = 1 << 3
    await ClockCycles(dut.clk, 10)

    b0 = 0b1000000010
    b1 = 0b1000000000
    b2 = 0b1000000100
    b3 = 0b1000000000
    b4 = 0b1101001010
    b5 = 0b1010100110
    b6 = 0b1001011110
    b7 = 0b1111111110
    await send_rx_bits(dut, bit_hold, b0, 10)
    await send_rx_bits(dut, bit_hold, b1, 10)
    await send_rx_bits(dut, bit_hold, b2, 10)
    await send_rx_bits(dut, bit_hold, b3, 10)
    await send_rx_bits(dut, bit_hold, b4, 10)
    await send_rx_bits(dut, bit_hold, b5, 10)
    await send_rx_bits(dut, bit_hold, b6, 10)
    await send_rx_bits(dut, bit_hold, b7, 10)
    
    bits_to_assert = 0b1000000010
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1000000000
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1000000000
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1000000000
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1101001010
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1010100110
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1001011110
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1111111110
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    b0 = 0b1000000010
    b1 = 0b1000000010
    b2 = 0b1000000000
    b3 = 0b1000000000
    b4 = 0b1101001010
    b5 = 0b1010100110
    b6 = 0b1001011110
    b7 = 0b1111111110
    await send_rx_bits(dut, bit_hold, b0, 10)
    await send_rx_bits(dut, bit_hold, b1, 10)
    await send_rx_bits(dut, bit_hold, b2, 10)
    await send_rx_bits(dut, bit_hold, b3, 10)
    await send_rx_bits(dut, bit_hold, b4, 10)
    await send_rx_bits(dut, bit_hold, b5, 10)
    await send_rx_bits(dut, bit_hold, b6, 10)
    await send_rx_bits(dut, bit_hold, b7, 10)

    bits_to_assert = 0b1000000010
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1000000010
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1000000100
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1000000000
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1000000000
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1000000000
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1000000000
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)

    bits_to_assert = 0b1000000000
    await assert_tx_bits(dut, bit_hold, bits_to_assert, 10)