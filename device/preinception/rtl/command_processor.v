`default_nettype none

module CommandProcessor
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

    output reg [5:0] extern_led
);
    localparam CP_IDLE = 0;
    localparam CP_ADD_CMD_BEGIN = 1;
    localparam CP_ADD_CMD_WAIT = 2;
    localparam CP_ADD_CMD_READ = 3;
    localparam CP_ADD_CMD_END = 4;

    reg [3:0] cp_state;

    reg [31:0] num1;
    reg [31:0] num2;
    reg [31:0] num_dwords_read;

    always @(posedge extern_clock or posedge async_reset)
    begin
        if(async_reset)
        begin
            cp_state <= CP_IDLE;
            out_host <= 0;
            out_en <= 0;
            
            extern_led <= 0;
        end
        else
        begin
            case(cp_state)

                CP_IDLE:
                begin
                    if(in_en == 1)
                    begin
                        out_en <= 0;
                        if(in_host == 32'h64646461)
                        begin
                            extern_led <= 6'b111000;
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
                            extern_led <= 6'b111001;
                            num1 <= in_host;
                        end
                        else if(num_dwords_read == 1)
                        begin
                            extern_led <= 6'b111010;
                            num2 <= in_host;
                        end

                        num_dwords_read <= num_dwords_read + 1;
                    end
                end

                CP_ADD_CMD_END:
                begin
                    extern_led <= 6'b000001;
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