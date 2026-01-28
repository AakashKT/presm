module test();

    reg clock = 0;
    reg reset = 0;

    reg [7:0] in = 0;
    wire [7:0] out;

    RegisterDFFRst #(4'd8) register(
        clock,
        in,
        reset,
        out
    );

    always
        #1 clock <= ~clock;

    initial
    begin
        $display("Starting test 'Register of D Flip Flop with Reset'");
        $monitor("Out: %b", out);

        #2 reset = 1;
        #2 reset = 0;
        if(out != 8'b00000000)
            $error("Register reset error");

        #10 in = 8'b10101010;
        #2;
        if(out != 8'b10101010)
            $error("Register set error");

        #1000 $finish;
    end

endmodule