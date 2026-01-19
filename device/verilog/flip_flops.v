module DFlipFlop
(
    input _extern_27mhz_clock,
    input in,
    input enable,
    input clear,
    input async_reset,
    output out
);

    reg out;

    always @(posedge _extern_27mhz_clock or posedge async_reset)
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