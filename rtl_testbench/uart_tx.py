import cocotb
from cocotb.triggers import RisingEdge, FallingEdge, Timer
from cocotb.clock import Clock

@cocotb.test()
async def uart_tx(dut):
    freq = 27000000 
    baud_rate = 115200

    clk_ns = round(1e9 / freq, 2)
    bit_hold = round(clk_ns * float(freq) / baud_rate, 2)

    dut.async_reset.value = 0
    dut.data_en.value = 0

    clk = Clock(dut.extern_clock, clk_ns, unit='ns')
    clk.start(start_high=False)

    await RisingEdge(dut.extern_clock)
    await Timer(clk_ns, unit='ns')

    dut.async_reset.value = 1
    await Timer(clk_ns, unit='ns')
    dut.async_reset.value = 0
    await Timer(clk_ns, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART TX reset failed"
    assert dut.data_sent.value == 0b1, "UART TX reset failed"

    dut.data.value = 0b10101100
    dut.data_en.value = 0b1
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART TX data set failed"
    dut.data_en.value = 0b0
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART TX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART TX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART TX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART TX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART TX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART TX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART TX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART TX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART TX data set failed"
    assert dut.data_sent.value == 0b1, "data_sent not set"
    await Timer(bit_hold, unit='ns')

    dut.data_en.value = 0b0
    await Timer(bit_hold, unit='ns')

    dut.data.value = 0b11011101
    dut.data_en.value = 0b1
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART TX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART TX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART TX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART TX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART TX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART TX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART TX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART TX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART TX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART TX data set failed"
    assert dut.data_sent.value == 0b1, "data_sent not set"
    await Timer(bit_hold, unit='ns')
