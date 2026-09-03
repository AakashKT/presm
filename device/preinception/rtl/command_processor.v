
module CommandProcessor
(
    // Clock
    input extern_clock,

    // Reset
    input extern_reset,

    // Host Communication
    input wire [47:0] rx_packet,
    input wire rx_packet_ready,

    output reg [47:0] tx_packet,
    output reg tx_packet_ready,
    input wire tx_packet_sent
);
    localparam CP_IDLE = 0;
    localparam CP_PACKET_DECODE = 1;
    localparam CP_HANDSHAKE_PREP = 2;
    localparam CP_HANDSHAKE = 3;
    localparam CP_TX_PACKET_SENT_WAIT = 4;
    localparam CP_DELAY = 5;
    localparam CP_STOP = 6;

    localparam CP_MEM_FETCH_PREP = 7;
    localparam CP_MEM_FETCH = 8;
    localparam CP_MEM_FETCH_END = 9;

    localparam CP_MEM_WRITE_ADDR_PREP = 10;
    localparam CP_MEM_WRITE_ADDR = 11;
    localparam CP_MEM_WRITE_VAL_PREP = 12;
    localparam CP_MEM_WRITE_VAL = 13;
    localparam CP_MEM_WRITE_END = 14;

    localparam CP_FETCH_OP1 = 15;
    localparam CP_FETCH_OP1_END = 16;
    localparam CP_FETCH_OP2 = 17;
    localparam CP_FETCH_OP2_END = 18;

    localparam CP_CMD_END_PREP = 19;
    localparam CP_CMD_END = 20;

    localparam CP_ADD = 21;
    localparam CP_MUL = 22;
    localparam CP_DIVP2 = 23;

    reg [2:0] delay_cycles, delay_counter;

    reg [5:0] cp_state, delay_restore_state, wait_restore_state, mem_op_restore_state;
    reg [3:0] pkt_id, pkt_cmd, pkt_sub_cmd;
    reg [7:0] tx_cmd_id;

    reg [31:0] mem_fetch_addr;
    reg [31:0] mem_write_addr;
    reg signed [31:0] mem_val;

    reg signed [31:0] op_1, op_2;

    always @(posedge extern_clock or posedge extern_reset)
    begin
        if(extern_reset)
        begin
            delay_cycles <= 1;
            delay_counter <= 0;

            cp_state <= CP_IDLE;
            mem_op_restore_state <= CP_IDLE;
            wait_restore_state <= CP_IDLE;
            delay_restore_state <= CP_IDLE;

            pkt_id <= 0;
            pkt_cmd <= 0;
            pkt_sub_cmd <= 0;

            tx_cmd_id <= 0;

            tx_packet <= 0;
            tx_packet_ready <= 0;

            mem_fetch_addr <= 0;
            mem_write_addr <= 0;
            mem_val <= 0;

            op_1 <= 0;
            op_2 <= 0;
        end
        else
        begin
            case(cp_state)
                CP_IDLE:
                begin
                    if(rx_packet_ready == 1)
                    begin
                        delay_cycles <= 1;
                        delay_counter <= 0;

                        mem_op_restore_state <= CP_IDLE;
                        wait_restore_state <= CP_IDLE;
                        delay_restore_state <= CP_IDLE;

                        pkt_id <= rx_packet[3:0];
                        // pkt_type <= rx_packet[7:4];
                        pkt_cmd <= rx_packet[11:8];
                        pkt_sub_cmd <= rx_packet[15:12];
                        
                        cp_state <= CP_PACKET_DECODE;
                    end
                    else
                    begin
                        cp_state <= CP_IDLE;
                    end
                end

                CP_PACKET_DECODE:
                begin
                    if(pkt_cmd == 1)
                    begin
                        cp_state <= CP_HANDSHAKE_PREP;
                    end
                    else if(pkt_cmd == 2)
                    begin
                        if(pkt_sub_cmd == 0)
                        begin
                            cp_state <= CP_FETCH_OP1;
                        end
                        else if(pkt_sub_cmd == 1)
                        begin
                            cp_state <= CP_FETCH_OP2;
                        end
                        else if(pkt_sub_cmd == 2)
                        begin
                            cp_state <= CP_ADD;
                        end
                    end
                    else if(pkt_cmd == 3)
                    begin
                        if(pkt_sub_cmd == 0)
                        begin
                            cp_state <= CP_FETCH_OP1;
                        end
                        else if(pkt_sub_cmd == 1)
                        begin
                            cp_state <= CP_FETCH_OP2;
                        end
                        else if(pkt_sub_cmd == 2)
                        begin
                            cp_state <= CP_MUL;
                        end
                    end
                    else if(pkt_cmd == 4)
                    begin
                        if(pkt_sub_cmd == 0)
                        begin
                            cp_state <= CP_FETCH_OP1;
                        end
                        else if(pkt_sub_cmd == 1)
                        begin
                            cp_state <= CP_FETCH_OP2;
                        end
                        else if(pkt_sub_cmd == 2)
                        begin
                            cp_state <= CP_DIVP2;
                        end
                    end
                    else
                    begin
                        cp_state <= CP_PACKET_DECODE;
                    end
                end

                CP_HANDSHAKE_PREP:
                begin
                    tx_packet_ready <= 0;

                    delay_counter <= 0;
                    delay_cycles <= 2;
                    delay_restore_state <= CP_HANDSHAKE;
                    cp_state <= CP_DELAY;
                end

                CP_HANDSHAKE:
                begin
                    tx_packet[3:0] <= pkt_id;
                    tx_packet[7:4] <= 1;
                    tx_packet[11:8] <= pkt_cmd;
                    tx_packet[15:12] <= pkt_sub_cmd;
                    tx_packet[23:16] <= 8'd2;
                    tx_packet[31:24] <= 8'd1;
                    tx_packet[47:32] <= 0;
                    tx_packet_ready <= 1;

                    wait_restore_state <= CP_STOP;
                    cp_state <= CP_TX_PACKET_SENT_WAIT;
                end

                CP_TX_PACKET_SENT_WAIT:
                begin
                    if(tx_packet_sent == 1)
                    begin
                        cp_state <= wait_restore_state;
                    end
                    else
                    begin
                        cp_state <= CP_TX_PACKET_SENT_WAIT;
                    end
                end

                CP_DELAY:
                begin
                    if(delay_counter == delay_cycles)
                    begin
                        cp_state <= delay_restore_state;
                    end
                    else
                    begin
                        delay_counter <= delay_counter + 1;
                    end
                end

                CP_STOP:
                begin
                    if(rx_packet_ready == 0)
                    begin
                        tx_cmd_id <= 0;
                        cp_state <= CP_IDLE;
                    end
                    else
                    begin
                        cp_state <= CP_STOP;
                    end
                end
                
                CP_MEM_FETCH_PREP:
                begin
                    tx_packet_ready <= 0;
                    tx_cmd_id <= tx_cmd_id + 1;

                    delay_counter <= 0;
                    delay_cycles <= 2;
                    delay_restore_state <= CP_MEM_FETCH;
                    cp_state <= CP_DELAY;
                end

                CP_MEM_FETCH:
                begin
                    tx_packet[3:0] <= tx_cmd_id;
                    tx_packet[7:4] <= 0;
                    tx_packet[11:8] <= 0;
                    tx_packet[15:12] <= 0;
                    tx_packet[47:16] <= mem_fetch_addr;
                    tx_packet_ready <= 1;

                    wait_restore_state <= CP_MEM_FETCH_END;
                    cp_state <= CP_TX_PACKET_SENT_WAIT;
                end

                CP_MEM_FETCH_END:
                begin
                    if(rx_packet_ready == 1)
                    begin
                        if(rx_packet[3:0] == tx_cmd_id && rx_packet[7:4] == 8'd1 
                            && rx_packet[11:8] == 8'd0 && rx_packet[15:12] == 8'd0)
                        begin
                            mem_val <= $signed(rx_packet[47:16]);
                            cp_state <= mem_op_restore_state;
                        end
                        else
                        begin
                            cp_state <= CP_MEM_FETCH_END;
                        end
                    end
                    else
                    begin
                        cp_state <= CP_MEM_FETCH_END;
                    end
                end

                CP_MEM_WRITE_ADDR_PREP:
                begin
                    tx_packet_ready <= 0;
                    tx_cmd_id <= tx_cmd_id + 1;

                    delay_counter <= 0;
                    delay_cycles <= 2;
                    delay_restore_state <= CP_MEM_WRITE_ADDR;
                    cp_state <= CP_DELAY;
                end

                CP_MEM_WRITE_ADDR:
                begin
                    tx_packet[3:0] <= tx_cmd_id;
                    tx_packet[7:4] <= 0;
                    tx_packet[11:8] <= 0;
                    tx_packet[15:12] <= 1;
                    tx_packet[47:16] <= mem_write_addr;
                    tx_packet_ready <= 1;

                    cp_state <= CP_MEM_WRITE_VAL_PREP;
                end

                CP_MEM_WRITE_VAL_PREP:
                begin
                    if(tx_packet_sent == 1)
                    begin
                        tx_packet_ready <= 0;
                        tx_cmd_id <= tx_cmd_id + 1;

                        delay_counter <= 0;
                        delay_cycles <= 2;
                        delay_restore_state <= CP_MEM_WRITE_VAL;
                        cp_state <= CP_DELAY;
                    end
                    else
                    begin
                        cp_state <= CP_MEM_WRITE_VAL_PREP;
                    end
                end

                CP_MEM_WRITE_VAL:
                begin
                    tx_packet[3:0] <= tx_cmd_id;
                    tx_packet[7:4] <= 0;
                    tx_packet[11:8] <= 0;
                    tx_packet[15:12] <= 1;
                    tx_packet[47:16] <= $signed(mem_val);
                    tx_packet_ready <= 1;
                    
                    wait_restore_state <= CP_MEM_WRITE_END;
                    cp_state <= CP_TX_PACKET_SENT_WAIT;
                end

                CP_MEM_WRITE_END:
                begin
                    if(rx_packet_ready == 1)
                    begin
                        if(rx_packet[3:0] == tx_cmd_id && rx_packet[7:4] == 1 
                            && rx_packet[11:8] == 0 && rx_packet[15:12] == 1)
                        begin
                            cp_state <= mem_op_restore_state;
                        end
                    end
                    else
                    begin
                        cp_state <= CP_MEM_WRITE_END;
                    end
                end

                CP_FETCH_OP1:
                begin
                    mem_fetch_addr <= rx_packet[47:16];

                    mem_op_restore_state <= CP_FETCH_OP1_END;
                    cp_state <= CP_MEM_FETCH_PREP;
                end

                CP_FETCH_OP1_END:
                begin
                    op_1 <= mem_val;
                    cp_state <= CP_CMD_END_PREP;
                end

                CP_FETCH_OP2:
                begin
                    mem_fetch_addr <= rx_packet[47:16];

                    mem_op_restore_state <= CP_FETCH_OP2_END;
                    cp_state <= CP_MEM_FETCH_PREP;
                end

                CP_FETCH_OP2_END:
                begin
                    op_2 <= mem_val;
                    cp_state <= CP_CMD_END_PREP;
                end

                CP_CMD_END_PREP:
                begin
                    tx_packet_ready <= 0;
                    
                    delay_counter <= 0;
                    delay_cycles <= 2;
                    delay_restore_state <= CP_CMD_END;
                    cp_state <= CP_DELAY;
                end

                CP_CMD_END:
                begin
                    tx_packet[3:0] <= pkt_id;
                    tx_packet[7:4] <= 1;
                    tx_packet[11:8] <= pkt_cmd;
                    tx_packet[15:12] <= pkt_sub_cmd;
                    tx_packet[47:16] <= 0;
                    tx_packet_ready <= 1;

                    wait_restore_state <= CP_STOP;
                    cp_state <= CP_TX_PACKET_SENT_WAIT;
                end

                CP_ADD:
                begin
                    mem_write_addr <= rx_packet[47:16];

                    mem_val <= op_1 + op_2;

                    mem_op_restore_state <= CP_CMD_END_PREP;
                    cp_state <= CP_MEM_WRITE_ADDR_PREP;
                end

                CP_MUL:
                begin
                    mem_write_addr <= rx_packet[47:16];

                    mem_val <= op_1 * op_2;

                    mem_op_restore_state <= CP_CMD_END_PREP;
                    cp_state <= CP_MEM_WRITE_ADDR_PREP;
                end

                CP_DIVP2:
                begin
                    mem_write_addr <= rx_packet[47:16];

                    mem_val <= op_1 >>> op_2;

                    mem_op_restore_state <= CP_CMD_END_PREP;
                    cp_state <= CP_MEM_WRITE_ADDR_PREP;
                end

                default: cp_state <= CP_IDLE;
            endcase 
        end
    end

endmodule