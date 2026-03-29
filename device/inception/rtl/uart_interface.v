
`default_nettype none

`include "uart_rx.v"
`include "uart_tx.v"
`include "command_processor.v"

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

    reg [7:0] cp_in;
    reg cp_in_en;

    wire [7:0] cp_out;
    wire cp_out_en;

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
        tx_data_en
    );

    CommandProcessor cp(
        extern_clock,
        extern_reset,
        cp_in,
        cp_in_en,
        cp_out,
        cp_out_en
    );

    always @(posedge extern_clock)
    begin
        if(rx_data_en == 1)
        begin
            cp_in = rx_data;
            cp_in_en = 1;
        end
        else
        begin
            cp_in <= 0;
            cp_in_en <= 0;
        end

        if(cp_out_en == 1)
        begin
            tx_data = cp_out;
            tx_data_en = 1;
        end
        else
        begin
            tx_data <= 0;
            tx_data_en <= 0;
        end
    end

endmodule