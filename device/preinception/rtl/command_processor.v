`default_nettype none

module CommandProcessor
#(
    parameter IDENT_INTERVAL = 27000000
)
(
    // Clock
    input extern_clock,

    // Reset
    input async_reset,

    // Host input
    input wire [31:0] in_host,
    input wire in_en,

    // Host output
    output reg [31:0] out_host,
    output reg out_en,
    input wire out_written
);
    localparam CP_IDENT_BROADCAST = 0;
    localparam CP_IDENT_BROADCAST_WAIT = 1;
    localparam CP_IDLE = 2;
    localparam CP_ADD_CMD_BEGIN = 3;
    localparam CP_ADD_CMD_WAIT = 4;
    localparam CP_ADD_CMD_READ = 5;
    localparam CP_ADD_CMD_END = 6;

    reg [31:0] wait_counter;

    reg [3:0] cp_state;

    reg [31:0] num1;
    reg [31:0] num2;
    reg [31:0] num_dwords_read;

    always @(posedge extern_clock or posedge async_reset)
    begin
        if(async_reset)
        begin
            cp_state <= CP_IDENT_BROADCAST;
            out_host <= 0;
            out_en <= 0;

            wait_counter <= 0;
        end
        else
        begin
            case(cp_state)

                CP_IDENT_BROADCAST:
                begin
                    out_host <= 32'h636e6970;
                    out_en <= 1;

                    if(out_written == 1)
                    begin
                        cp_state <= CP_IDENT_BROADCAST_WAIT;
                        out_en <= 0;
                        wait_counter <= 0;
                    end
                    
                    if(in_en == 1 && in_host == 32'h6e656469)
                    begin
                        cp_state <= CP_IDLE;
                    end
                end

                CP_IDENT_BROADCAST_WAIT:
                begin
                    if(in_en == 1 && in_host == 32'h6e656469)
                    begin
                        cp_state <= CP_IDLE;
                    end

                    if(wait_counter == IDENT_INTERVAL)
                    begin
                        cp_state <= CP_IDENT_BROADCAST;
                    end
                    else
                    begin
                        wait_counter <= wait_counter + 1;
                    end
                end

                CP_IDLE:
                begin                    
                    if(in_en == 1)
                    begin
                        out_en <= 0;
                        if(in_host == 32'h64646461)
                        begin
                            cp_state <= CP_ADD_CMD_BEGIN;
                        end
                    end
                end

                CP_ADD_CMD_BEGIN:
                begin
                    if(in_en == 0)
                    begin
                        num_dwords_read <= 0;
                        cp_state <= CP_ADD_CMD_READ;
                    end
                end

                CP_ADD_CMD_WAIT:
                begin
                    if(num_dwords_read == 2)
                    begin
                        cp_state <= CP_ADD_CMD_END;
                    end
                    else if(in_en == 0)
                    begin
                        cp_state <= CP_ADD_CMD_READ;
                    end
                end

                CP_ADD_CMD_READ:
                begin
                    if(in_en == 1)
                    begin
                        cp_state <= CP_ADD_CMD_WAIT;

                        if(num_dwords_read == 0)
                        begin
                            num1 <= in_host;
                        end
                        else if(num_dwords_read == 1)
                        begin
                            num2 <= in_host;
                        end

                        num_dwords_read <= num_dwords_read + 1;
                    end
                end

                CP_ADD_CMD_END:
                begin
                    out_host <= num1 + num2;
                    out_en <= 1;

                    if(in_en == 0)
                    begin
                        cp_state <= CP_IDLE;
                    end
                end

            endcase
        end
    end

endmodule