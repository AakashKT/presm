`default_nettype none

`include "uart_packet.v"

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

    UARTPacket uartPacket(
        extern_clock,
        extern_uart_rx,
        extern_uart_tx,
        extern_reset,
        rx_packet,
        rx_packet_ready,
        tx_packet,
        tx_packet_ready
    );

    always @(posedge extern_clock or posedge extern_reset)
    begin
        if(extern_reset)
        begin
            tx_packet_ready <= 0;
        end
        else
        begin
            if(rx_packet_ready == 1)
            begin
                tx_packet[7:0] <= 8'd1;
                tx_packet[15:8] <= 8'd2;
                tx_packet[23:16] <= 0;
                tx_packet[31:24] <= 0;
                tx_packet[39:32] <= 8'd6;
                tx_packet[47:40] <= 8'd3;

                tx_packet_ready <= 1;
            end
            else
            begin
                tx_packet_ready <= 0;
            end
        end
    end

endmodule