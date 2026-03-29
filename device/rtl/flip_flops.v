module DFlipFlopRst
(
    input extern_clock,
    input inp,
    input async_reset,
    output reg op
);

    always @(posedge extern_clock or posedge async_reset)
    begin
        if(async_reset == 1'b1)
        begin
            op <= 1'b0;
        end
        else
        begin
            op <= inp;
        end
    end
    
endmodule

module DFlipFlopEnClRst
(
    input extern_clock,
    input inp,
    input enable,
    input clear,
    input async_reset,
    output reg op
);

    always @(posedge extern_clock or posedge async_reset)
    begin
        if(async_reset)
        begin
            op <= 1'b0;
        end
        else if(clear == 1'b0)
        begin
            op <= 1'b0;
        end
        else if(enable)
        begin
            op <= inp;
        end
    end
    
endmodule