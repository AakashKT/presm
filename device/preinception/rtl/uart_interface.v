`default_nettype none

`include "uart_rx.v"
`include "uart_tx.v"

module UartInterface
(
    // Clock
    input extern_clock,

    // UART rx & tx
    input wire extern_uart_rx,
    output wire extern_uart_tx,

    // Hardware reset signal
    input extern_reset
);

    // Registers
    wire [7:0] rx_data;
    wire rx_data_en;

    reg [7:0] tx_data;
    reg tx_data_en;
    wire tx_data_sent;

    // Submodules
    UARTRx receiver(
        extern_clock,
        extern_reset,
        extern_uart_rx,
        rx_data,
        rx_data_en
    );

    UARTTx transmitter(
        extern_clock,
        extern_reset,
        extern_uart_tx,
        tx_data,
        tx_data_en,
        tx_data_sent
    );

    always @(posedge extern_clock)
    begin
        if(rx_data_en == 1 && tx_data_sent == 0)
        begin
            tx_data = rx_data;
            tx_data_en = 1;
        end
        else
        begin
            tx_data <= 0;
            tx_data_en <= 0;
        end
    end

endmodule