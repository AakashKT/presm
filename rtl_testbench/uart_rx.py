import cocotb
from cocotb.triggers import RisingEdge, FallingEdge, Timer
from cocotb.clock import Clock

@cocotb.test()
async def uart_rx(dut):
    freq = 27000000 
    baud_rate = 115200

    clk_ns = round(1e9 / freq, 2)
    bit_hold = round(clk_ns * float(freq) / baud_rate, 2)

    dut.async_reset.value = 0
    dut.extern_uart_rx.value = 1

    clk = Clock(dut.extern_clock, clk_ns, unit='ns')
    clk.start(start_high=False)

    await RisingEdge(dut.extern_clock)
    await Timer(clk_ns, unit='ns')

    dut.async_reset.value = 1
    await Timer(clk_ns, unit='ns')
    dut.async_reset.value = 0
    await Timer(clk_ns, unit='ns')
    assert dut.data.value == 0b00000000, "UART RX reset failed"
    
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    assert dut.data_en.value == 1, "UART RX data enable set failed"
    assert dut.data.value == 0b10101001, "UART RX data set failed || %s" % dut.data.value

    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    assert dut.data_en.value == 0, "UART RX data enable reset failed"
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    assert dut.data_en.value == 1, "UART RX data set failed"
    assert dut.data.value == 0b11010011, "UART RX data set failed || %s" % dut.data.value
