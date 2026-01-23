module test();
    
    reg clock = 0;
    reg reset = 0;

    reg [31:0] in = 0;
    reg in_en = 0;

    wire [31:0] out;
    wire out_en;

    CommandProcessor cp(
        clock,
        reset,
        in,
        in_en,
        out,
        out_en
    );

    always
        #1 clock = ~clock;
    
    initial 
    begin
        $monitor("Input: %b, Output: %b, Output Enable: %b", 
                    in, out, out_en);

        #2 reset = 1;
        #2 reset = 0;

        #2 in = 32'd2832662527;
        #2 in_en = 1;

        #100;
        #100 $finish;
    end

    initial 
    begin
        $dumpfile("output/command_processor.vcd");
        $dumpvars(0,test);
    end

endmodule