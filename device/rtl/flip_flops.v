module DFlipFlopRst
(
    input _extern_clock,
    input in,
    input async_reset,
    output reg out
);

    always @(posedge _extern_clock or posedge async_reset)
    begin
        if(async_reset == 1'b1)
        begin
            out <= 1'b0;
        end
        else
        begin
            out <= in;
        end
    end
    
endmodule

module DFlipFlopEnClRst
(
    input _extern_clock,
    input in,
    input enable,
    input clear,
    input async_reset,
    output reg out
);

    always @(posedge _extern_clock or posedge async_reset)
    begin
        if(async_reset)
        begin
            out <= 1'b0;
        end
        else if(clear == 1'b0)
        begin
            out <= 1'b0;
        end
        else if(enable)
        begin
            out <= in;
        end
    end
    
endmodule