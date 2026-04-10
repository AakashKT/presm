`default_nettype none

`include "uart_rx.v"
`include "uart_tx.v"

module UartInterface
(
    // Clock
    input extern_clock,

    // UART rx & tx
    input wire extern_uart_rx,
    output wire extern_uart_tx,

    // Hardware reset signal
    input extern_reset
);
    // Receive
    localparam DWORD_READ_BEGIN = 0;
    localparam DWORD_READ_WAIT = 1;
    localparam DWORD_READ_END = 2;

    reg [2:0] read_state;

    wire [7:0] rx_data;
    wire rx_data_en;

    reg [3:0] read_byte_num;
    reg [31:0] dword_read;
    reg dword_read_flag;

    // Transmit
    localparam DWORD_WRITE_IDLE = 0;
    localparam DWORD_WRITE_BEGIN = 1;
    localparam DWORD_WRITE_WAIT = 2;
    localparam DWORD_WRITE_END = 3;

    reg [2:0] write_state;

    reg [7:0] tx_data;
    reg tx_data_en;
    wire tx_data_sent;

    reg [3:0] write_byte_num;
    reg [31:0] dword_write;
    reg dword_write_flag;

    // Submodules
    UARTRx receiver(
        extern_clock,
        extern_reset,
        extern_uart_rx,
        rx_data,
        rx_data_en
    );

    UARTTx transmitter(
        extern_clock,
        extern_reset,
        extern_uart_tx,
        tx_data,
        tx_data_en,
        tx_data_sent
    );

    always @(posedge extern_clock or posedge extern_reset)
    begin
        if(extern_reset)
        begin
            read_byte_num <= 0;
            dword_read_flag <= 0;
            read_state <= DWORD_READ_BEGIN;
        end
        else
        begin
            case(read_state)

                DWORD_READ_BEGIN:
                begin
                    if(rx_data_en == 1)
                    begin
                        dword_read_flag <= 0;
                        read_state <= DWORD_READ_WAIT;

                        if(read_byte_num == 0)
                        begin
                            dword_read[7:0] <= rx_data;
                        end
                        else if(read_byte_num == 1)
                        begin
                            dword_read[15:8] <= rx_data;
                        end
                        else if(read_byte_num == 2)
                        begin
                            dword_read[23:16] <= rx_data;
                        end
                        else if(read_byte_num == 3)
                        begin
                            dword_read[31:24] <= rx_data;
                        end

                        read_byte_num <= read_byte_num + 1;
                    end
                end

                DWORD_READ_WAIT:
                begin
                    if(read_byte_num >= 3'd4)
                    begin
                        read_state <= DWORD_READ_END;
                    end
                    else if(rx_data_en == 0)
                    begin
                        read_state <= DWORD_READ_BEGIN;
                    end
                end

                DWORD_READ_END:
                begin
                    read_byte_num <= 0;
                    dword_read_flag <= 1;

                    if(rx_data_en == 0)
                    begin 
                        read_state <= DWORD_READ_BEGIN;
                    end
                end
                
            endcase
        end
    end

    always @(posedge extern_clock or posedge extern_reset)
    begin
        if(extern_reset)
        begin
            write_byte_num <= 0;
            tx_data_en <= 0;
            write_state <= DWORD_WRITE_IDLE;
        end
        else
        begin
            case(write_state)

                DWORD_WRITE_IDLE:
                begin
                    tx_data_en <= 0;
                    if(dword_write_flag == 1)
                    begin
                        write_byte_num <= 0;
                        write_state <= DWORD_WRITE_BEGIN;
                    end
                end

                DWORD_WRITE_BEGIN:
                begin
                    write_state <= DWORD_WRITE_WAIT;

                    if(write_byte_num == 0)
                    begin
                        tx_data <= dword_write[7:0];
                        tx_data_en <= 1;
                    end
                    else if(write_byte_num == 1)
                    begin
                        tx_data <= dword_write[15:8];
                        tx_data_en <= 1;
                    end
                    else if(write_byte_num == 2)
                    begin
                        tx_data <= dword_write[23:16];
                        tx_data_en <= 1;
                    end
                    else if(write_byte_num == 3)
                    begin
                        tx_data <= dword_write[31:24];
                        tx_data_en <= 1;
                    end

                    write_byte_num <= write_byte_num + 1;
                end

                DWORD_WRITE_WAIT:
                begin
                    if(write_byte_num >= 4'd4)
                    begin
                        write_state <= DWORD_WRITE_END;
                    end
                    else if(tx_data_sent == 1)
                    begin
                        tx_data_en <= 0;
                        write_state <= DWORD_WRITE_BEGIN;
                    end
                end

                DWORD_WRITE_END:
                begin
                    if(dword_write_flag == 0)
                    begin
                        write_state <= DWORD_WRITE_IDLE;
                    end
                end

            endcase
        end
    end

endmodule