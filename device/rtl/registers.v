`include "flip_flops.v"


module RegisterDFFRst
#(
    parameter NUM_BITS=32
)
(
    input extern_clock,
    input [NUM_BITS-1:0] inp,
    input async_reset,
    output wire [NUM_BITS-1:0] op
);
    
    generate
        genvar i;
        for(i=NUM_BITS-1; i>=0; i=i-1)
        begin
            DFlipFlopRst d_ff(
                extern_clock,
                inp[i],
                async_reset,
                op[i]
            );
        end
    endgenerate

endmodule

module RegisterDFFEnClRst
#(
    parameter NUM_BITS=32
)
(
    input extern_clock,
    input [NUM_BITS-1:0] inp,
    input enable,
    input clear,
    input async_reset,
    output wire [NUM_BITS-1:0] op
);
    
    generate
        genvar i;
        for(i=NUM_BITS-1; i>=0; i=i-1)
        begin
            DFlipFlopEnClRst d_ff(
                extern_clock,
                inp[i],
                enable,
                clear,
                async_reset,
                op[i]
            );
        end
    endgenerate

endmodule
