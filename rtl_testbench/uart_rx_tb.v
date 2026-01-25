module test();

    reg clock = 0;
    reg reset = 0;
    reg uart_rx = 1;
    wire [7:0] data;
    wire data_en;

    UARTRx uart_module(
        clock,
        reset,
        uart_rx,
        data,
        data_en
    );

    always
        #1 clock = ~clock;
    
    initial 
    begin
        $display("Starting UART RX");
        $monitor("Data Value %b", data);

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

        $display("");

        #468 uart_rx = 0;

        #468 uart_rx = 1;
        #468 uart_rx = 1;
        #468 uart_rx = 0;
        #468 uart_rx = 0;
        #468 uart_rx = 1;
        #468 uart_rx = 0;
        #468 uart_rx = 1;
        #468 uart_rx = 1;

        #468 uart_rx = 1;

        #1000 $finish;
    end

    initial 
    begin
        $dumpfile("output/uart_rx.vcd");
        $dumpvars(0,test);
    end

endmodule