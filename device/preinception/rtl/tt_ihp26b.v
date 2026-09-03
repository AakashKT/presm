`default_nettype none

`include "command_processor.v"
`include "uart_packet.v"

module tt_um_preinception_top (
    input  wire [7:0] ui_in,    // Dedicated inputs
    output wire [7:0] uo_out,   // Dedicated outputs
    input  wire [7:0] uio_in,   // IOs: Input path
    output wire [7:0] uio_out,  // IOs: Output path
    output wire [7:0] uio_oe,   // IOs: Enable path (active high: 0=input, 1=output)
    input  wire       ena,      // will go high when the design is enabled
    input  wire       clk,      // clock
    input  wire       rst_n     // reset_n - low to reset
);

    assign uo_out[0] = 1'b0;
    assign uo_out[1] = 1'b0;
    assign uo_out[2] = 1'b0;
    assign uo_out[3] = 1'b0;
    assign uo_out[5] = 1'b0;
    assign uo_out[6] = 1'b0;
    assign uo_out[7] = 1'b0;

    assign uio_out = 8'b00000000;
    assign uio_oe  = 8'b00000000;

    wire [47:0] rx_packet;
    wire rx_packet_ready;

    wire [47:0] tx_packet;
    wire tx_packet_ready;
    wire tx_packet_sent;

    wire rst_p;
    assign rst_p = ~rst_n;

    wire uart_tx_wire;
    assign uo_out[4] = rst_p ? 0 : uart_tx_wire;

    UARTPacket #(.DELAY_WAIT(5208)) uartPacket(
        .extern_clock(clk),
        .extern_uart_rx(ui_in[3]),
        .extern_uart_tx(uart_tx_wire),
        .extern_reset(rst_p),
        .rx_packet_flat(rx_packet),
        .rx_packet_ready(rx_packet_ready),
        .tx_packet_flat(tx_packet),
        .tx_packet_ready(tx_packet_ready),
        .tx_packet_sent(tx_packet_sent)
    );

    CommandProcessor cp(
        .extern_clock(clk),
        .extern_reset(rst_p),
        .rx_packet(rx_packet),
        .rx_packet_ready(rx_packet_ready),
        .tx_packet(tx_packet),
        .tx_packet_ready(tx_packet_ready),
        .tx_packet_sent(tx_packet_sent)
    );

    wire _unused_pins = &{ui_in, uio_in, ena};

endmodule