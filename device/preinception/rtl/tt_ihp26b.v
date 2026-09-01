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

    wire [63:0] rx_packet;
    wire rx_packet_ready;

    wire [63:0] tx_packet;
    wire tx_packet_ready;
    wire tx_packet_sent;

    reg rst_p;
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

    always @(posedge clk or negedge rst_n)
    begin
        if(!rst_n)
        begin
            uart_rx <= 1;
            rst_p <= 1;
        end
        else
        begin
            rst_p <= 0;
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