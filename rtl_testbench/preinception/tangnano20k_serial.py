import cocotb
from cocotb.triggers import ClockCycles, Timer
from cocotb.clock import Clock

async def assert_tx_bits(dut, bit_hold, bits, num_bits):
    for i in range(num_bits):
        bit = (bits >> i) & 1

        assert int(dut.extern_uart_tx.value) == bit, "UART TX: expected %d, got %d at bit number: %d" % (bit, dut.extern_uart_tx, i)
        await Timer(bit_hold, unit='ns')

async def send_rx_bits(dut, bit_hold, bits, num_bits):
    for i in range(num_bits):
        bit = (bits >> i) & 1

        dut.extern_uart_rx.value = bit
        await Timer(bit_hold, unit='ns')

async def send_packet(dut, bit_hold, packet):
    for item in packet:
        await send_rx_bits(dut, bit_hold, item, 10)

async def assert_packet(dut, bit_hold, packet):
    for item in packet:
        await assert_tx_bits(dut, bit_hold, item, 10)

async def rx_pin(dut):
    dut.extern_uart_rx.value = 1
    await ClockCycles(dut.extern_clock, 10)

async def reset(dut):
    dut.extern_reset.value = 1
    await ClockCycles(dut.extern_clock, 10)
    dut.extern_reset.value = 0
    await ClockCycles(dut.extern_clock, 10)

def init(dut):
    freq = 27000000 
    baud_rate = 9600

    clk_ns = round(1e9 / freq, 2)
    bit_hold = round(clk_ns * float(freq) / baud_rate, 2)

    clk = Clock(dut.extern_clock, clk_ns, unit='ns')
    cocotb.start_soon(clk.start())

    return bit_hold

@cocotb.test()
async def reset_test(dut):
    init(dut)
    await reset(dut)
    await rx_pin(dut)

    assert dut.extern_uart_rx.value == 1
    assert dut.extern_uart_tx.value == 1

@cocotb.test()
async def handshake_test(dut):
    bit_hold = init(dut)
    await reset(dut)
    await rx_pin(dut)

    packet = [
        ((1 | 0 << 4) | (1 << 8)) << 1, ((1 | 0 << 4) | (1 << 8)) << 1,
        0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await send_packet(dut, bit_hold, packet)

    packet = [
        ((1 | 1 << 4) | (1 << 8)) << 1, ((1 | 0 << 4) | (1 << 8)) << 1,
        0b1000000100, 0b1000000010, 0b1000000000, 0b1000000000
    ]
    await assert_packet(dut, bit_hold, packet)
    
@cocotb.test()
async def op1_test(dut):
    bit_hold = init(dut)
    await reset(dut)
    await rx_pin(dut)

    packet = [
        ((1 | 0 << 4) | (1 << 8)) << 1, ((2 | 0 << 4) | (0 << 8)) << 1,
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await send_packet(dut, bit_hold, packet)
    
    packet = [
        ((1 | 0 << 4) | (1 << 8)) << 1, ((0 | 0 << 4) | (1 << 8)) << 1,
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await assert_packet(dut, bit_hold, packet)

    packet = [
        ((1 | 1 << 4) | (1 << 8)) << 1, ((0 | 0 << 4) | (1 << 8)) << 1, 
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await send_packet(dut, bit_hold, packet)

    packet = [
        ((1 | 1 << 4) | (1 << 8)) << 1, ((2 | 0 << 4) | (1 << 8)) << 1,
        0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await assert_packet(dut, bit_hold, packet)

@cocotb.test()
async def op2_test(dut):
    bit_hold = init(dut)
    await reset(dut)
    await rx_pin(dut)

    packet = [
        ((1 | 0 << 4) | (1 << 8)) << 1, ((2 | 1 << 4) | (0 << 8)) << 1,
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await send_packet(dut, bit_hold, packet)
    
    packet = [
        ((1 | 0 << 4) | (1 << 8)) << 1, ((0 | 0 << 4) | (1 << 8)) << 1,
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await assert_packet(dut, bit_hold, packet)

    packet = [
        ((1 | 1 << 4) | (1 << 8)) << 1, ((0 | 0 << 4) | (1 << 8)) << 1, 
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await send_packet(dut, bit_hold, packet)

    packet = [
        ((1 | 1 << 4) | (1 << 8)) << 1, ((2 | 1 << 4) | (1 << 8)) << 1,
        0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await assert_packet(dut, bit_hold, packet)

@cocotb.test()
async def add_cmd_test(dut):
    bit_hold = init(dut)
    await reset(dut)
    await rx_pin(dut)

    ######################
    # OP 1
    ######################
    packet = [
        ((1 | 0 << 4) | (1 << 8)) << 1, ((2 | 0 << 4) | (0 << 8)) << 1,
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await send_packet(dut, bit_hold, packet)
    packet = [
        ((1 | 0 << 4) | (1 << 8)) << 1, ((0 | 0 << 4) | (1 << 8)) << 1,
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await assert_packet(dut, bit_hold, packet)

    packet = [
        ((1 | 1 << 4) | (1 << 8)) << 1, ((0 | 0 << 4) | (1 << 8)) << 1, 
        0b1000000110, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await send_packet(dut, bit_hold, packet)
    packet = [
        ((1 | 1 << 4) | (1 << 8)) << 1, ((2 | 0 << 4) | (1 << 8)) << 1,
        0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await assert_packet(dut, bit_hold, packet)

    ######################
    # OP 2
    ######################
    packet = [
        ((2 | 0 << 4) | (1 << 8)) << 1, ((2 | 1 << 4) | (1 << 8)) << 1,
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await send_packet(dut, bit_hold, packet)
    packet = [
        ((1 | 0 << 4) | (1 << 8)) << 1, ((0 | 0 << 4) | (1 << 8)) << 1, 
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await assert_packet(dut, bit_hold, packet)

    packet = [
        ((1 | 1 << 4) | (1 << 8)) << 1, ((0 | 0 << 4) | (1 << 8)) << 1,
        0b1000000110, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await send_packet(dut, bit_hold, packet)
    packet = [
        ((2 | 1 << 4) | (1 << 8)) << 1, ((2 | 1 << 4) | (1 << 8)) << 1,
        0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await assert_packet(dut, bit_hold, packet)

    ######################
    # OP 3
    ######################
    packet = [
        ((3 | 0 << 4) | (1 << 8)) << 1, ((2 | 2 << 4) | (1 << 8)) << 1,
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await send_packet(dut, bit_hold, packet)

    packet = [
        ((1 | 0 << 4) | (1 << 8)) << 1, ((0 | 1 << 4) | (1 << 8)) << 1,
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await assert_packet(dut, bit_hold, packet)
    packet = [
        ((2 | 0 << 4) | (1 << 8)) << 1, ((0 | 1 << 4) | (1 << 8)) << 1, 
        0b1000001100, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await assert_packet(dut, bit_hold, packet)

    packet = [
        ((2 | 1 << 4) | (1 << 8)) << 1, ((0 | 1 << 4) | (1 << 8)) << 1, 
        0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await send_packet(dut, bit_hold, packet)
    packet = [
        ((3 | 1 << 4) | (1 << 8)) << 1, ((2 | 2 << 4) | (1 << 8)) << 1,
        0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await assert_packet(dut, bit_hold, packet)

@cocotb.test()
async def mulp2_cmd_test(dut):
    bit_hold = init(dut)
    await reset(dut)
    await rx_pin(dut)

    ######################
    # OP 1
    ######################
    packet = [
        ((1 | 0 << 4) | (1 << 8)) << 1, ((3 | 0 << 4) | (1 << 8)) << 1,
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await send_packet(dut, bit_hold, packet)
    packet = [
        ((1 | 0 << 4) | (1 << 8)) << 1, ((0 | 0 << 4) | (1 << 8)) << 1, 
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await assert_packet(dut, bit_hold, packet)

    packet = [
        ((1 | 1 << 4) | (1 << 8)) << 1, ((0 | 0 << 4) | (1 << 8)) << 1, 
        0b1000000110, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await send_packet(dut, bit_hold, packet)
    packet = [
        ((1 | 1 << 4) | (1 << 8)) << 1, ((3 | 0 << 4) | (1 << 8)) << 1,
        0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await assert_packet(dut, bit_hold, packet)

    ######################
    # OP 2
    ######################
    packet = [
        ((2 | 0 << 4) | (1 << 8)) << 1, ((3 | 1 << 4) | (1 << 8)) << 1,
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await send_packet(dut, bit_hold, packet)
    packet = [
        ((1 | 0 << 4) | (1 << 8)) << 1, ((0 | 0 << 4) | (1 << 8)) << 1, 
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await assert_packet(dut, bit_hold, packet)

    packet = [
        ((1 | 1 << 4) | (1 << 8)) << 1, ((0 | 0 << 4) | (1 << 8)) << 1, 
        0b1000000110, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await send_packet(dut, bit_hold, packet)
    packet = [
        ((2 | 1 << 4) | (1 << 8)) << 1, ((3 | 1 << 4) | (1 << 8)) << 1,
        0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await assert_packet(dut, bit_hold, packet)

    ######################
    # OP 3
    ######################
    packet = [
        ((3 | 0 << 4) | (1 << 8)) << 1, ((3 | 2 << 4) | (1 << 8)) << 1,
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await send_packet(dut, bit_hold, packet)

    packet = [
        ((1 | 0 << 4) | (1 << 8)) << 1, ((0 | 1 << 4) | (1 << 8)) << 1, 
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await assert_packet(dut, bit_hold, packet)
    packet = [
        ((2 | 0 << 4) | (1 << 8)) << 1, ((0 | 1 << 4) | (1 << 8)) << 1, 
        0b1000110000, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await assert_packet(dut, bit_hold, packet)

    packet = [
        ((2 | 1 << 4) | (1 << 8)) << 1, ((0 | 1 << 4) | (1 << 8)) << 1, 
        0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await send_packet(dut, bit_hold, packet)
    packet = [
        ((3 | 1 << 4) | (1 << 8)) << 1, ((3 | 2 << 4) | (1 << 8)) << 1,
        0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await assert_packet(dut, bit_hold, packet)

@cocotb.test()
async def divp2_cmd_test(dut):
    bit_hold = init(dut)
    await reset(dut)
    await rx_pin(dut)

    ######################
    # OP 1
    ######################
    packet = [
        ((1 | 0 << 4) | (1 << 8)) << 1, ((4 | 0 << 4) | (1 << 8)) << 1,
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await send_packet(dut, bit_hold, packet)
    packet = [
        ((1 | 0 << 4) | (1 << 8)) << 1, ((0 | 0 << 4) | (1 << 8)) << 1, 
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await assert_packet(dut, bit_hold, packet)

    packet = [
        ((1 | 1 << 4) | (1 << 8)) << 1, ((0 | 0 << 4) | (1 << 8)) << 1, 
        0b1000100000, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await send_packet(dut, bit_hold, packet)
    packet = [
        ((1 | 1 << 4) | (1 << 8)) << 1, ((4 | 0 << 4) | (1 << 8)) << 1,
        0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await assert_packet(dut, bit_hold, packet)

    ######################
    # OP 2
    ######################
    packet = [
        ((2 | 0 << 4) | (1 << 8)) << 1, ((4 | 1 << 4) | (1 << 8)) << 1,
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await send_packet(dut, bit_hold, packet)
    packet = [
        ((1 | 0 << 4) | (1 << 8)) << 1, ((0 | 0 << 4) | (1 << 8)) << 1, 
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await assert_packet(dut, bit_hold, packet)

    packet = [
        ((1 | 1 << 4) | (1 << 8)) << 1, ((0 | 0 << 4) | (1 << 8)) << 1, 
        0b1000000110, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await send_packet(dut, bit_hold, packet)
    packet = [
        ((2 | 1 << 4) | (1 << 8)) << 1, ((4 | 1 << 4) | (1 << 8)) << 1,
        0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await assert_packet(dut, bit_hold, packet)

    ######################
    # OP 3
    ######################
    packet = [
        ((3 | 0 << 4) | (1 << 8)) << 1, ((4 | 2 << 4) | (1 << 8)) << 1,
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await send_packet(dut, bit_hold, packet)

    packet = [
        ((1 | 0 << 4) | (1 << 8)) << 1, ((0 | 1 << 4) | (1 << 8)) << 1, 
        0b1101001010, 0b1010100110, 0b1001011110, 0b1111111110
    ]
    await assert_packet(dut, bit_hold, packet)
    packet = [
        ((2 | 0 << 4) | (1 << 8)) << 1, ((0 | 1 << 4) | (1 << 8)) << 1, 
        0b1000000100, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await assert_packet(dut, bit_hold, packet)

    packet = [
        ((2 | 1 << 4) | (1 << 8)) << 1, ((0 | 1 << 4) | (1 << 8)) << 1, 
        0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await send_packet(dut, bit_hold, packet)
    packet = [
        ((3 | 1 << 4) | (1 << 8)) << 1, ((4 | 2 << 4) | (1 << 8)) << 1,
        0b1000000000, 0b1000000000, 0b1000000000, 0b1000000000
    ]
    await assert_packet(dut, bit_hold, packet)
    