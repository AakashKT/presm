
module UARTRx
#(
    parameter DELAY_WAIT = 2812
)
(
    // Clock
    input extern_clock,

    // Reset
    input async_reset,

    // UART rx external
    input wire extern_uart_rx,

    // Outgoing
    output reg [7:0] data,
    output reg data_en
);

    localparam HALF_DELAY_WAIT = (DELAY_WAIT / 2);

    // Receiver
    localparam RX_IDLE = 0;
    localparam RX_START = 1;
    localparam RX_READ = 2;
    localparam RX_STOP = 3;

    reg [11:0] rx_counter;
    reg [3:0] rx_bit_number;

    reg [2:0] rx_state;

    (* ASYNC_REG = "TRUE" *) reg rx_r1, rx_r2;
    always @(posedge extern_clock or posedge async_reset) 
    begin
        if (async_reset) 
        begin
            rx_r1 <= 1;
            rx_r2 <= 1;
        end 
        else 
        begin
            rx_r1 <= extern_uart_rx;
            rx_r2 <= rx_r1;
        end
    end

    always @(posedge extern_clock or posedge async_reset)
    begin
        if(async_reset)
        begin
            rx_state <= RX_IDLE;

            data <= 0;
            data_en <= 1;

            rx_counter <= 0;
            rx_bit_number <= 0;
        end
        else
        begin
            case(rx_state)

                RX_IDLE:
                begin
                    if(rx_r2 == 0)
                    begin
                        rx_counter <= 0;
                        rx_bit_number <= 0;
                        data <= 0;
                        data_en <= 0;
                        
                        rx_state <= RX_START;
                    end
                    else
                    begin
                        data_en <= 1;
                        rx_state <= RX_IDLE;
                    end
                end

                RX_START:
                begin
                    if(rx_counter == HALF_DELAY_WAIT)
                    begin
                        rx_counter <= 0;
                        rx_state <= RX_READ;
                    end
                    else
                    begin
                        rx_counter <= rx_counter + 1;
                        rx_state <= RX_START;
                    end
                end

                RX_READ:
                begin
                    if(rx_counter == DELAY_WAIT)
                    begin
                        if(rx_bit_number == 4'b1000)
                        begin
                            rx_state <= RX_STOP;
                        end
                        else
                        begin
                            data <= {rx_r2, data[7:1]};
                            rx_bit_number <= rx_bit_number + 1;
                            rx_counter <= 0;

                            rx_state <= RX_READ;
                        end
                    end
                    else
                    begin
                        rx_counter <= rx_counter + 1;
                        rx_state <= RX_READ;
                    end
                end

                RX_STOP:
                begin
                    if(rx_counter == DELAY_WAIT)
                    begin
                        data_en <= 1;
                        rx_state <= RX_IDLE;
                    end
                    else
                    begin
                        rx_counter <= rx_counter + 1;
                        rx_state <= RX_STOP;
                    end
                end

            endcase
        end
    end

endmodule