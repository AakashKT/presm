`include "command_processor.v"
`include "uart_packet.v"

module preinception_top (
    input  wire [7:0] ui_in,    // Dedicated inputs
    output wire [7:0] uo_out,   // Dedicated outputs
    input  wire [7:0] uio_in,   // IOs: Input path
    output wire [7:0] uio_out,  // IOs: Output path
    output wire [7:0] uio_oe,   // IOs: Enable path (active high: 0=input, 1=output)
    input  wire       ena,      // will go high when the design is enabled
    input  wire       clk,      // clock
    input  wire       rst_n     // reset_n - low to reset
);

    wire [63:0] rx_packet;
    wire rx_packet_ready;

    wire [63:0] tx_packet;
    wire tx_packet_ready;
    wire tx_packet_sent;

    reg rst_p;
    assign rst_p = ~rst_n;

    reg uart_rx;

    UARTPacket #(.DELAY_WAIT(5208)) uartPacket(
        clk,
        uart_rx,
        uo_out[4],
        rst_p,
        rx_packet,
        rx_packet_ready,
        tx_packet,
        tx_packet_ready,
        tx_packet_sent
    );

    CommandProcessor cp(
        clk,
        rst_p,
        rx_packet,
        rx_packet_ready,
        tx_packet,
        tx_packet_ready,
        tx_packet_sent
    );

    always @(posedge clk or posedge rst_p)
    begin
        if(rst_p)
        begin
            uart_rx <= 1;
        end
        else
        begin
            if(ena)
            begin
                uart_rx <= ui_in[3];
            end
            else
            begin
                uart_rx <= 1;
            end
        end
    end

endmodule