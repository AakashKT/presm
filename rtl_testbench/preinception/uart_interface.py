import cocotb
from cocotb.triggers import RisingEdge, FallingEdge, Timer
from cocotb.clock import Clock

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

    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')

    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
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
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')

    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
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
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')

    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
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
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
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
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')

    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')

    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')

    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
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
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')

    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')

    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')

    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')

    assert dut.dword_write_flag.value == 1
    assert dut.dword_write.value == 5
    
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"

    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"

    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"

    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"


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

    dut.dword_write.value = 0b10101111101011111010111110101111
    dut.dword_write_flag.value = 1

    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"

    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"

    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"

    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"

    dut.dword_write_flag.value = 0
    await Timer(bit_hold, unit='ns')

    dut.dword_write.value = 0b01110010011100100111001001110010
    dut.dword_write_flag.value = 1

    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"

    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"

    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"

    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b0, "UART RX data set failed"
    await Timer(bit_hold, unit='ns')
    assert dut.extern_uart_tx.value == 0b1, "UART RX data set failed"

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

    assert dut.dword_read_flag.value == 1
    assert dut.dword_read.value == 0b10101001101010011010100110101001

    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')

    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')

    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')

    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 0
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')
    dut.extern_uart_rx.value = 1
    await Timer(bit_hold, unit='ns')

    assert dut.dword_read_flag.value == 1
    assert dut.dword_read.value == 0b10111100101111001011110010111100
    
