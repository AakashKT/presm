`default_nettype none

module UARTTx
#(
    parameter DELAY_WAIT = 234
)
(
    // Clock
    input _extern_clock,

    // Reset
    input async_reset,

    // UART tx external
    output reg _extern_uart_tx,

    // Incoming
    input wire [7:0] data,
    input wire data_en
);
    // Transmitter
    localparam TX_IDLE = 0;
    localparam TX_START = 1;
    localparam TX_WRITE = 2;
    localparam TX_STOP = 3;

    reg [8:0] tx_counter;
    reg [2:0] tx_bit_number;
    reg [2:0] tx_state;

    // Reset signal
    always @(posedge async_reset)
    begin
        tx_counter <= 0;
        tx_bit_number <= 0;
        tx_state <= TX_IDLE;

        _extern_uart_tx <= 1;
    end

    always @(posedge _extern_clock)
    begin
        case(tx_state) 

            TX_IDLE:
            begin
                if(data_en == 1)
                begin
                    tx_bit_number <= 0;
                    tx_counter <= 0;

                    tx_state <= TX_START;
                end
                else
                begin
                    _extern_uart_tx <= 1;
                end
            end

            TX_START:
            begin
                _extern_uart_tx <= 0;
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
                _extern_uart_tx <= data[tx_bit_number];
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
                _extern_uart_tx <= 1;
                if(data_en == 0)
                begin
                    tx_state <= TX_IDLE;
                end
            end

        endcase
    end

endmodule