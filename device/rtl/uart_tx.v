`default_nettype none

module UARTTx
#(
    parameter DELAY_WAIT = 234
)
(
    // External pins
    input _extern_clock,
    output _extern_uart_tx,

    // Incoming
    input reg [7:0] data,
    input reg data_en
);
    // Transmitter
    localparam TX_IDLE = 0;
    localparam TX_START = 1;
    localparam TX_WRITE = 2;
    localparam TX_STOP = 3;

    reg [8:0] tx_counter = 0;
    reg [2:0] tx_bit_number = 0;
    reg [2:0] tx_state = 0;

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
                tx_counter <= tx_counter + 1;
                if(tx_counter == DELAY_WAIT)
                begin
                    tx_counter <= 0;
                    
                    tx_state <= TX_WRITE;
                end
            end

            TX_WRITE:
            begin
                _extern_uart_tx <= data[tx_bit_number];
                tx_counter <= tx_counter + 1;
                if(tx_counter == DELAY_WAIT)
                begin
                    tx_bit_number <= tx_bit_number + 1;
                    tx_counter <= 0;

                    if(tx_bit_number == 3'b111)
                    begin                        
                        tx_state <= TX_STOP;
                    end
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