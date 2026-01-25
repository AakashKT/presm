
`default_nettype none

`include "uart_rx.v"
`include "uart_tx.v"
`include "command_processor.v"

module RiscvUart
(
    // Clock
    input _extern_clock,

    // UART rx & tx
    input wire _extern_uart_rx,
    output wire _extern_uart_tx,

    // Reset hardware button
    input _extern_reset
);

    // Definitions

    // Registers
    wire [7:0] rx_data;
    wire rx_data_en;

    reg [7:0] tx_data;
    reg tx_data_en;

    reg [7:0] cp_in;
    reg cp_in_en;

    wire [7:0] cp_out;
    wire cp_out_en;
    
    wire reset;
    assign reset = ~_extern_reset;

    // Reset signal
    always @(posedge reset)
    begin
        tx_data <= 0;
        tx_data_en <= 0;

        cp_in <= 0;
        cp_in_en <= 0;
    end

    // Submodules
    UARTRx receiver(
        _extern_clock,
        reset,
        _extern_uart_rx,
        rx_data,
        rx_data_en
    );

    UARTTx transmitter(
        _extern_clock,
        reset,
        _extern_uart_tx,
        tx_data,
        tx_data_en
    );

    CommandProcessor cp(
        _extern_clock,
        reset,
        cp_in,
        cp_in_en,
        cp_out,
        cp_out_en
    );

    always @(posedge _extern_clock)
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