`default_nettype none

`include "../../verilog/uart.v"

module CommandProcessor
#(
    parameter CP_DATALINE_SIZE = 32
)
(
    // Clock
    input clk,
    // Host input
    input [CP_DATALINE_SIZE-1:0] in_host,
    // Host output
    output [CP_DATALINE_SIZE-1:0] out_host
);



endmodule