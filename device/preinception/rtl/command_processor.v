`default_nettype none

`include "adder.v"

module CommandProcessor
#(
    parameter CLOCK_FREQ = 27000000,
    parameter OUTGOING_PACKET_HEADER_DELAY = 10
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
    input wire out_ack,

    output reg [5:0] extern_led
);
    // =================
    // State definitions
    // =================

    // Identity broadcast & handshake
    localparam CP_IDENT_BROADCAST = 0;
    localparam CP_IDENT_BROADCAST_WAIT = 1;

    // Idle
    localparam CP_IDLE = 2;

    // Memory fetch
    localparam CP_MEM_FETCH_BEGIN = 3;
    localparam CP_MEM_FETCH_CMD_HEADER = 4;
    localparam CP_MEM_FETCH_CMD = 5;
    localparam CP_MEM_FETCH_ADDR = 6;
    localparam CP_MEM_FETCH_WAIT = 7;

    // Memory write
    localparam CP_MEM_WRITE_BEGIN = 8;
    localparam CP_MEM_WRITE_CMD = 9;
    localparam CP_MEM_WRITE_ADDR = 10;
    localparam CP_MEM_WRITE_DATA = 11;

    // Input read
    localparam CP_INPUT_READ = 12;
    localparam CP_INPUT_READ_WAIT = 13;

    // ADDI command
    localparam CP_ADDI_CMD_BEGIN = 14;
    localparam CP_ADDI_CMD_INPUT_FETCH = 15;
    localparam CP_ADDI_CMD_EXEC = 16;
    localparam CP_ADDI_CMD_END = 17;

    // Misc
    localparam CP_DELAY_ONE = 18;
    localparam CP_DELAY_HEADER = 19;

    reg [31:0] delay_wait_counter;
    reg [4:0] cp_state;
    reg [4:0] restore_cp_state;
    reg [4:0] delay_restore_cp_state;

    // Memory fetch, write registers
    reg [31:0] mem_addr;
    reg [31:0] mem_data;

    // Arithmetic registers
    reg signed [31:0] ra;
    reg signed [31:0] rb;
    reg signed [31:0] rc;

    // Arithmetic module signals
    reg adder_signal;
    wire signed [31:0] add_out;
    wire add_out_rdy;

    // Misc
    reg [31:0] num_dwords_read;
    reg [31:0] num_dwords_processed;

    // Submodules
    Adder addr1(
        extern_clock,
        async_reset,
        adder_signal,
        ra, rb,
        add_out,
        add_out_rdy
    );

    always @(posedge extern_clock or posedge async_reset)
    begin
        if(async_reset)
        begin
            cp_state <= CP_IDENT_BROADCAST;
            out_host <= 0;
            out_en <= 0;

            mem_addr <= 0;
            mem_data <= 0;

            ra <= 0;
            rb <= 0;
            rc <= 0;

            adder_signal <= 0;

            delay_wait_counter <= 0;
        end
        else
        begin
            case(cp_state)

                CP_IDENT_BROADCAST:
                begin
                    extern_led <= 6'b110000;

                    out_host <= 32'h636e6970; // 'pinc'
                    out_en <= 1;

                    if(out_ack == 1)
                    begin
                        cp_state <= CP_IDENT_BROADCAST_WAIT;
                        out_en <= 0;
                        delay_wait_counter <= 0;
                    end
                    
                    if(in_en == 1 && in_host == 32'h6e656469) // 'iden'
                    begin
                        out_en <= 0;
                        cp_state <= CP_IDLE;
                    end
                end

                CP_IDENT_BROADCAST_WAIT:
                begin
                    if(in_en == 1 && in_host == 32'h6e656469)
                    begin
                        out_en <= 0;
                        cp_state <= CP_IDLE;
                    end

                    if(delay_wait_counter == CLOCK_FREQ)
                    begin
                        cp_state <= CP_IDENT_BROADCAST;
                    end
                    else
                    begin
                        delay_wait_counter <= delay_wait_counter + 1;
                    end
                end

                CP_IDLE:
                begin
                    extern_led <= 6'b001100;

                    if(in_en == 1)
                    begin
                        if(in_host == 32'h69646461) // 'addi'
                        begin
                            cp_state <= CP_ADDI_CMD_BEGIN;
                        end
                        else if(in_host == 32'h74736572) // 'rest'
                        begin
                            cp_state <= CP_IDENT_BROADCAST;
                            out_host <= 0;
                            out_en <= 0;

                            delay_wait_counter <= 0;
                        end
                    end
                end

                CP_MEM_FETCH_BEGIN:
                begin
                    out_en <= 0;

                    delay_restore_cp_state <= CP_MEM_FETCH_CMD_HEADER;
                    cp_state <= CP_DELAY_ONE;
                end

                CP_MEM_FETCH_CMD_HEADER:
                begin
                    out_host <= 32'd8; // size in bytes of the packet
                    out_en <= 1;

                    if(out_ack == 1)
                    begin
                        out_en <= 0;

                        delay_wait_counter <= 0;
                        delay_restore_cp_state <= CP_MEM_FETCH_CMD;
                        cp_state <= CP_DELAY_HEADER;
                    end
                end

                CP_MEM_FETCH_CMD:
                begin
                    extern_led <= 6'b000011;

                    out_host <= 32'h726d656d; // 'memr' --> memory read
                    out_en <= 1;

                    if(out_ack == 1)
                    begin
                        out_en <= 0;

                        delay_restore_cp_state <= CP_MEM_FETCH_ADDR;
                        cp_state <= CP_DELAY_ONE;
                    end
                end

                CP_MEM_FETCH_ADDR:
                begin
                    out_host <= mem_addr;
                    out_en <= 1;

                    if(out_ack == 1 && in_en == 0)
                    begin
                        out_en <= 0;
                        cp_state <= CP_MEM_FETCH_WAIT;
                    end
                end

                CP_MEM_FETCH_WAIT:
                begin
                    if(in_en == 1)
                    begin
                        mem_data <= in_host;
                        cp_state <= restore_cp_state;
                    end
                end

                CP_MEM_WRITE_BEGIN:
                begin
                    out_en <= 0;
                    cp_state <= CP_MEM_WRITE_CMD;
                end

                CP_MEM_WRITE_CMD:
                begin
                    out_host <= 32'h776d656d; // 'memw' --> memory write
                    out_en <= 1;

                    if(out_ack == 1)
                    begin
                        out_en <= 0;
                        delay_restore_cp_state <= CP_MEM_WRITE_ADDR;
                        cp_state <= CP_DELAY_ONE;
                    end
                end

                CP_MEM_WRITE_ADDR:
                begin
                    out_host <= mem_addr;
                    out_en <= 1;

                    if(out_ack == 1)
                    begin
                        out_en <= 0;
                        delay_restore_cp_state <= CP_MEM_WRITE_DATA;
                        cp_state <= CP_DELAY_ONE;
                    end
                end

                CP_MEM_WRITE_DATA:
                begin
                    out_host <= mem_data;
                    out_en <= 1;

                    if(out_ack == 1)
                    begin
                        out_en <= 0;
                        cp_state <= restore_cp_state;
                    end
                end

                CP_INPUT_READ:
                begin
                    if(in_en == 1)
                    begin
                        if(num_dwords_read == 0)
                        begin
                            ra <= in_host;
                            cp_state <= CP_INPUT_READ_WAIT;
                        end
                        else if(num_dwords_read == 1)
                        begin
                            rb <= in_host;
                            cp_state <= CP_INPUT_READ_WAIT;
                        end
                        else if(num_dwords_read == 2)
                        begin
                            rc <= in_host;
                            cp_state <= restore_cp_state;
                        end

                        num_dwords_read <= num_dwords_read + 1;
                    end
                end

                CP_INPUT_READ_WAIT:
                begin
                    if(in_en == 0)
                    begin
                        cp_state <= CP_INPUT_READ;
                    end
                end

                CP_ADDI_CMD_BEGIN:
                begin
                    adder_signal <= 0;
                    if(in_en == 0)
                    begin
                        num_dwords_read <= 0;
                        num_dwords_processed <= 0;
                        restore_cp_state <= CP_ADDI_CMD_INPUT_FETCH;
                        cp_state <= CP_INPUT_READ;
                    end
                end

                CP_ADDI_CMD_INPUT_FETCH:
                begin
                    if(num_dwords_processed == 0)
                    begin
                        mem_addr <= ra;
                        restore_cp_state <= CP_ADDI_CMD_INPUT_FETCH;
                        cp_state <= CP_MEM_FETCH_BEGIN;
                    end
                    else if(num_dwords_processed == 1)
                    begin
                        ra <= mem_data;

                        mem_addr <= rb;
                        restore_cp_state <= CP_ADDI_CMD_INPUT_FETCH;
                        cp_state <= CP_MEM_FETCH_BEGIN;
                    end
                    else if(num_dwords_processed == 2)
                    begin
                        rb <= mem_data;
                        cp_state <= CP_ADDI_CMD_EXEC;
                    end

                    num_dwords_processed <= num_dwords_processed + 1;
                end

                CP_ADDI_CMD_EXEC:
                begin
                    adder_signal <= 1;

                    if(add_out_rdy == 1)
                    begin
                        mem_addr <= rc;
                        mem_data <= add_out;

                        restore_cp_state <= CP_ADDI_CMD_END;
                        cp_state <= CP_MEM_WRITE_BEGIN;
                    end
                end

                CP_ADDI_CMD_END:
                begin
                    if(in_en == 0)
                    begin
                        cp_state <= CP_IDLE;
                    end
                end

                CP_DELAY_ONE:
                begin
                    cp_state <= delay_restore_cp_state;
                end

                CP_DELAY_HEADER:
                begin
                    if(delay_wait_counter == OUTGOING_PACKET_HEADER_DELAY)
                    begin
                        cp_state <= delay_restore_cp_state;
                    end
                    else
                    begin
                        delay_wait_counter <= delay_wait_counter + 1;
                    end
                end

            endcase
        end
    end

endmodule