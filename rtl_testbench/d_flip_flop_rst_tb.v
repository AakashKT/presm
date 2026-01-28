


module test();

    reg clock = 0;
    reg reset = 0;

    reg in = 0;
    wire out;

    DFlipFlopRst dff(
        clock,
        in,
        reset,
        out
    );

    always
        #1 clock <= ~clock;

    initial
    begin
        $display("Starting test 'D Flip Flop with Reset'");
        $monitor("Out: %b", out);

        #2 reset = 1;
        #2 reset = 0;

        if(out != 0)
            $error("Reset failed");
        
        #2 in = 1;
        #2;
        if(out != 1)
            $error("Set '1' failed");

        #2 in = 0;
        #2;
        if(out != 0)
            $error("Set '0' failed");
        
        #2 in = 1;
        #2;
        if(out != 1)
            $error("Set '1' failed");
        
        #1000 $finish;
    end

endmodule