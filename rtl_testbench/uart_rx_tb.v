module test();

    reg clock = 0;
    reg uart_rx = 1;
    wire [7:0] data;
    wire data_en;

    UARTRx #(8'd8) uart_module(
        clock,
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
        #10 uart_rx=0;
        #16 uart_rx=1;
        #16 uart_rx=0;
        #16 uart_rx=0;
        #16 uart_rx=0;
        #16 uart_rx=0;
        #16 uart_rx=1;
        #16 uart_rx=1;
        #16 uart_rx=0;
        #16 uart_rx=1;
        #1000 $finish;
    end

    initial 
    begin
        $dumpfile("output/uart_rx.vcd");
        $dumpvars(0,test);
    end

endmodule