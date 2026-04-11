`default_nettype none

module UARTTx
#(
    parameter DELAY_WAIT = 234
)
(
    // Clock
    input extern_clock,

    // Reset
    input async_reset,

    // UART tx external
    output reg extern_uart_tx,

    // Incoming
    input wire [7:0] data,
    input wire data_en,

    // Sent signal
    output reg data_sent
);
    // Transmitter
    localparam TX_IDLE = 0;
    localparam TX_START = 1;
    localparam TX_WRITE = 2;
    localparam TX_STOP = 3;

    reg [8:0] tx_counter;
    reg [2:0] tx_bit_number;
    reg [2:0] tx_state;
    
    always @(posedge extern_clock or posedge async_reset)
    begin
        if(async_reset)
        begin
            tx_state <= TX_IDLE;
        end
        else
        begin
            case(tx_state) 

                TX_IDLE:
                begin
                    if(data_en == 1)
                    begin
                        tx_bit_number <= 0;
                        tx_counter <= 0;
                        data_sent <= 0;

                        tx_state <= TX_START;
                    end
                    else
                    begin
                        data_sent <= 0;
                        extern_uart_tx <= 1;
                    end
                end

                TX_START:
                begin
                    extern_uart_tx <= 0;
                    if(tx_counter == DELAY_WAIT)
                    begin
                        tx_counter <= 0;
                        
                        tx_state <= TX_WRITE;
                    end
                    else
                    begin
                        tx_counter <= tx_counter + 1;
                    end
                end

                TX_WRITE:
                begin
                    extern_uart_tx <= data[tx_bit_number];
                    if(tx_counter == DELAY_WAIT)
                    begin
                        if(tx_bit_number == 3'b111)
                        begin                        
                            tx_state <= TX_STOP;
                        end
                        else
                        begin
                            tx_bit_number <= tx_bit_number + 1;
                            tx_state <= TX_WRITE;
                        end
                        tx_counter <= 0;
                    end
                    else
                    begin
                        tx_counter <= tx_counter + 1;
                    end
                end

                TX_STOP:
                begin
                    extern_uart_tx <= 1;
                    data_sent <= 1;

                    if(tx_counter == DELAY_WAIT)
                    begin
                        tx_state <= TX_IDLE;
                        tx_counter <= 0;
                    end
                    else
                    begin
                        tx_counter <= tx_counter + 1;
                    end
                end

            endcase
        end
    end

endmodule