`include "flip_flops.v"

module RegisterDFF
#(
    parameter NUM_BITS=32
)
(
    input _extern_27mhz_clock,
    input [NUM_BITS-1:0] in,
    input enable,
    input clear,
    input async_reset,
    output [NUM_BITS-1:0] out
);
    
    generate
        genvar i;
        for(i=NUM_BITS-1; i>=0; i--)
        begin
            DFlipFlop d_ff(
                _extern_27mhz_clock,
                in[i],
                enable,
                clear,
                async_reset,
                out[i]
            );
        end
    endgenerate

endmodule