`include "flip_flops.v"


module RegisterDFFRst
#(
    parameter NUM_BITS=32
)
(
    input _extern_clock,
    input [NUM_BITS-1:0] in,
    input async_reset,
    output wire [NUM_BITS-1:0] out
);
    
    generate
        genvar i;
        for(i=NUM_BITS-1; i>=0; i=i-1)
        begin
            DFlipFlopRst d_ff(
                _extern_clock,
                in[i],
                async_reset,
                out[i]
            );
        end
    endgenerate

endmodule

module RegisterDFFEnClRst
#(
    parameter NUM_BITS=32
)
(
    input _extern_clock,
    input [NUM_BITS-1:0] in,
    input enable,
    input clear,
    input async_reset,
    output wire [NUM_BITS-1:0] out
);
    
    generate
        genvar i;
        for(i=NUM_BITS-1; i>=0; i=i-1)
        begin
            DFlipFlopEnClRst d_ff(
                _extern_clock,
                in[i],
                enable,
                clear,
                async_reset,
                out[i]
            );
        end
    endgenerate

endmodule
