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
    input [7:0] data,
    input data_en
);
    // // Transmitter
    // localparam TX_IDLE = 0;
    // localparam TX_START = 1;
    // localparam TX_WRITE = 2;
    // localparam TX_STOP = 3;

    // reg [8:0] tx_counter = 0;
    // reg [2:0] tx_bit_number = 0;
    // reg [2:0] tx_state = 0;

    // always @(posedge _extern_clock)
    // begin
    //     case(tx_state) 

    //         TX_IDLE:
    //         begin
    //             if(data_en == 1)
    //             begin
    //                 tx_bit_number <= 0;
    //                 tx_counter <= 0;

    //                 tx_state <= TX_START;
    //             end
    //             else
    //             begin
    //                 _extern_uart_tx <= 1;
    //             end
    //         end

    //         TX_START:
    //         begin
    //             _extern_uart_tx <= 0;
    //             tx_counter <= tx_counter + 1;
    //             if(tx_counter == DELAY_WAIT)
    //             begin
    //                 tx_counter <= 0;
                    
    //                 tx_state <= TX_WRITE;
    //             end
    //         end

    //         TX_WRITE:
    //         begin
    //             _extern_uart_tx <= data[tx_bit_number];
    //             tx_counter <= tx_counter + 1;
    //             if(tx_counter == DELAY_WAIT)
    //             begin
    //                 tx_bit_number <= tx_bit_number + 1;
    //                 tx_counter <= 0;

    //                 if(tx_bit_number == 3'b111)
    //                 begin                        
    //                     tx_state <= TX_STOP;
    //                 end
    //             end
    //         end

    //         TX_STOP:
    //         begin
    //             _extern_uart_tx <= 1;
    //             tx_counter <= tx_counter + 1;
    //             if(tx_counter == DELAY_WAIT)
    //             begin
    //                 tx_state <= TX_IDLE;
    //             end
    //         end

    //     endcase
    // end






    reg [3:0] txState = 0;
    reg [24:0] txCounter = 0;
    reg [7:0] dataOut = 0;
    reg txPinRegister = 1;
    reg [2:0] txBitNumber = 0;
    reg [3:0] txByteCounter = 0;

    assign _extern_uart_tx = txPinRegister;

    localparam MEMORY_LENGTH = 12;
    reg [7:0] testMemory [MEMORY_LENGTH-1:0];

    initial begin
        testMemory[0] = "L";
        testMemory[1] = "u";
        testMemory[2] = "s";
        testMemory[3] = "h";
        testMemory[4] = "a";
        testMemory[5] = "y";
        testMemory[6] = " ";
        testMemory[7] = "L";
        testMemory[8] = "a";
        testMemory[9] = "b";
        testMemory[10] = "s";
        testMemory[11] = " ";
    end

    localparam TX_STATE_IDLE = 0;
    localparam TX_STATE_START_BIT = 1;
    localparam TX_STATE_WRITE = 2;
    localparam TX_STATE_STOP_BIT = 3;
    localparam TX_STATE_DEBOUNCE = 4;

    always @(posedge _extern_clock) begin
        case (txState)
            TX_STATE_IDLE: begin
                if (data_en == 1) begin
                    txState <= TX_STATE_START_BIT;
                    txCounter <= 0;
                    txByteCounter <= 0;
                end
                else begin
                    txPinRegister <= 1;
                end
            end 
            TX_STATE_START_BIT: begin
                txPinRegister <= 0;
                if ((txCounter + 1) == DELAY_WAIT) begin
                    txState <= TX_STATE_WRITE;
                    dataOut <= testMemory[txByteCounter];
                    txBitNumber <= 0;
                    txCounter <= 0;
                end else 
                    txCounter <= txCounter + 1;
            end
            TX_STATE_WRITE: begin
                txPinRegister <= dataOut[txBitNumber];
                if ((txCounter + 1) == DELAY_WAIT) begin
                    if (txBitNumber == 3'b111) begin
                        txState <= TX_STATE_STOP_BIT;
                    end else begin
                        txState <= TX_STATE_WRITE;
                        txBitNumber <= txBitNumber + 1;
                    end
                    txCounter <= 0;
                end else 
                    txCounter <= txCounter + 1;
            end
            TX_STATE_STOP_BIT: begin
                txPinRegister <= 1;
                if ((txCounter + 1) == DELAY_WAIT) begin
                    if (txByteCounter == MEMORY_LENGTH - 1) begin
                        txState <= TX_STATE_DEBOUNCE;
                    end else begin
                        txByteCounter <= txByteCounter + 1;
                        txState <= TX_STATE_START_BIT;
                    end
                    txCounter <= 0;
                end else 
                    txCounter <= txCounter + 1;
            end
            TX_STATE_DEBOUNCE: begin
                if (txCounter == 23'b111111111111111111) begin
                    if (data_en == 0) 
                        txState <= TX_STATE_IDLE;
                end else
                    txCounter <= txCounter + 1;
            end
        endcase      
    end

endmodule