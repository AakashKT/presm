`default_nettype none

module UARTRx
#(
    parameter DELAY_WAIT = 234
)
(
    // External pins
    input _extern_clock,
    input _extern_uart_rx,

    // Outgoing
    output [7:0] data,
    output data_en,
);

    localparam HALF_DELAY_WAIT = (DELAY_WAIT / 2);

    // Receiver
    localparam RX_IDLE = 0;
    localparam RX_START = 1;
    localparam RX_READ = 2;
    localparam RX_STOP = 3;

    reg [8:0] rx_counter = 0;
    reg [2:0] rx_bit_number = 0;
    reg [2:0] rx_state = 0;

    always @(posedge _extern_clock)
    begin
        case(rx_state)

            RX_IDLE:
            begin
                if(_extern_uart_rx == 0)
                begin
                    rx_counter <= 0;
                    rx_bit_number <= 0;
                    data <= 0;
                    data_en <= 0;
                    
                    rx_state <= RX_START;
                end
            end

            RX_START:
            begin
                rx_counter <= rx_counter + 1;
                if(rx_counter == HALF_DELAY_WAIT)
                begin
                    rx_counter <= 0;
                    
                    rx_state <= RX_READ;
                end
            end

            RX_READ:
            begin
                rx_counter <= rx_counter + 1;
                if(rx_counter == DELAY_WAIT)
                begin
                    data <= {_extern_uart_rx, data[7:1]};
                    rx_bit_number <= rx_bit_number + 1;
                    rx_counter <= 0;

                    if(rx_bit_number == 3'b111)
                    begin
                        rx_state <= RX_STOP;
                    end
                end
            end

            RX_STOP:
            begin
                rx_counter <= rx_counter + 1;
                if(rx_counter == DELAY_WAIT)
                begin
                    data_en <= 1;

                    rx_state <= RX_IDLE;
                end
            end

        endcase
    end

endmodule