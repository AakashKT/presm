module test();

    reg clock = 0;
    reg reset = 0;
    wire uart_tx;
    reg [7:0] data = 0;
    reg data_en = 0;

    UARTTx uart_module(
        clock,
        reset,
        uart_tx,
        data,
        data_en
    );

    always
        #1 clock = ~clock;
    
    initial 
    begin
        $display("Starting UART TX");

        #468 reset = 1;
        #468 reset = 0;

        #468 data = 8'b10101001;
        #468 data_en = 1;
        #468 $display("TX Value %b", uart_tx);
        #468 $display("TX Value %b", uart_tx);
        #468 $display("TX Value %b", uart_tx);
        #468 $display("TX Value %b", uart_tx);
        #468 $display("TX Value %b", uart_tx);
        #468 $display("TX Value %b", uart_tx);
        #468 $display("TX Value %b", uart_tx);
        #468 $display("TX Value %b", uart_tx);
        #468 $display("TX Value %b", uart_tx);
        #468 $display("TX Value %b", uart_tx);

        $display("");

        #468 data_en = 0;
        #468 data = 8'b11111111;
        #468 data_en = 1;
        #468 $display("TX Value %b", uart_tx);
        #468 $display("TX Value %b", uart_tx);
        #468 $display("TX Value %b", uart_tx);
        #468 $display("TX Value %b", uart_tx);
        #468 $display("TX Value %b", uart_tx);
        #468 $display("TX Value %b", uart_tx);
        #468 $display("TX Value %b", uart_tx);
        #468 $display("TX Value %b", uart_tx);
        #468 $display("TX Value %b", uart_tx);
        #468 $display("TX Value %b", uart_tx);

        #1000 $finish;
    end

    initial 
    begin
        $dumpfile("output/uart_tx.vcd");
        $dumpvars(0,test);
    end

endmodule