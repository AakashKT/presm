`default_nettype none

`include "uart_packet.v"
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

    wire [63:0] rx_packet;
    wire rx_packet_ready;

    reg [63:0] tx_packet;
    reg tx_packet_ready;
    reg tx_packet_sent;

    UARTPacket uartPacket(
        extern_clock,
        extern_uart_rx,
        extern_uart_tx,
        extern_reset,
        rx_packet,
        rx_packet_ready,
        tx_packet,
        tx_packet_ready,
        tx_packet_sent
    );

    CommandProcessor cp(
        extern_clock,
        extern_reset,
        rx_packet,
        rx_packet_ready,
        tx_packet,
        tx_packet_ready,
        tx_packet_sent
    );

endmodule