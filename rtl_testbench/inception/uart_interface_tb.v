module test();
    
    reg clock = 0;
    reg reset = 0;

    reg uart_rx = 1;
    wire uart_tx;

    wire [5:0] led;

    UartInterface riscv_uart(
        clock,
        uart_rx,
        uart_tx,
        reset
    );

    always
        #1 clock = ~clock;
    
    initial 
    begin
        #468 reset = 1;
        #468 reset = 0;

        #468 uart_rx = 0;

        #468 uart_rx = 1;
        #468 uart_rx = 0;
        #468 uart_rx = 0;
        #468 uart_rx = 1;
        #468 uart_rx = 0;
        #468 uart_rx = 1;
        #468 uart_rx = 0;
        #468 uart_rx = 1;

        #468 uart_rx = 1;

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

        #468 uart_rx = 1;

        #10000;
        #10000 $finish;
    end

    initial 
    begin
        $dumpfile("output/command_processor.vcd");
        $dumpvars(0,test);
    end

endmodule