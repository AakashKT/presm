`default_nettype none

`include "uart_rx.v"
`include "uart_tx.v"
`include "command_processor.v"

module Tangnano20kUart
(
    // Clock
    input extern_clock,

    // UART rx & tx
    input wire extern_uart_rx,
    output wire extern_uart_tx,

    // Hardware reset signal
    input extern_reset,

    output reg [5:0] extern_led
);

    wire [7:0] rx_packet[16];
    wire rx_packet_ready;

endmodule