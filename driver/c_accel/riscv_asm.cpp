#include "riscv_asm.h"

void asm_runtime_error(std::string err)
{
    throw std::runtime_error("Assembler: " + err);
}



RiscvInstr::RiscvInstr(std::string instr)
{
    this->instr = instr;

    std::string line = this->instr.substr(1, this->instr.size());
            
    uint32_t sep_loc = line.find('\t');

    this->process_op(line.substr(0, sep_loc));
    if(this->type == NONE_TYPE) {
        // asm_runtime_error("Could not process instruction '" + instr + "'");
        return;
    }
    
    line = line.substr(sep_loc+1, line.size());
    
    uint32_t op_idx = 0;
    do {
        sep_loc = line.find(',');

        this->process_operand(op_idx, line.substr(0, sep_loc));

        line = line.substr(sep_loc+1, line.size());

        op_idx++;
    } while(line != "" && sep_loc != -1);
}

void RiscvInstr::process_op(std::string opcode)
{
    if(opcode == "addi") {
        this->type = I_TYPE;

        this->instr_bin.init_i_type();
        this->instr_bin.i_type.funct3 = ADDI;
    }
    else if(opcode == "mv") {
        this->type = I_TYPE;

        this->instr_bin.init_i_type();
        this->instr_bin.i_type.funct3 = ADDI;
        this->instr_bin.i_type.imm = 0x0;
    }
    else if(opcode == "add") {
        this->type = R_TYPE;

        this->instr_bin.init_r_type();
        this->instr_bin.i_type.funct3 = ADD;
    }
    else if(opcode == "sub") {
        this->type = R_TYPE;

        this->instr_bin.init_r_type();
        this->instr_bin.i_type.funct3 = SUB;
    }
}

void RiscvInstr::process_operand(uint32_t op_idx, std::string operand)
{
    uint32_t val;
    if(operand == "ra")
        val = RA;
    else if(operand == "sp")
        val = SP;
    else if(operand == "gp")
        val = GP;
    else if(operand == "tp")
        val = TP;
    else if(operand == "t0")
        val = T0;
    else if(operand == "t1")
        val = T1;
    else if(operand == "t2")
        val = T2;
    else if(operand == "s0" || operand == "fp")
        val = S0;
    else if(operand == "s1")
        val = S1;
    else if(operand[0] == 'a') {
        int r_idx = std::atoi(&operand[1]);
        val = A0 + r_idx;
    }
    else if(operand[0] == 's') {
        int r_idx = std::atoi(&operand[1]) - 2;
        val = S2 + r_idx;
    }
    else if(operand[0] == 't') {
        int r_idx = std::atoi(&operand[1]) - 3;
        val = T3 + r_idx;
    }
    else {
        val = std::stoi(operand);
    }

    if(this->type == R_TYPE) {
        if(op_idx == 0)
            this->instr_bin.r_type.rd = val;
        else if(op_idx == 1)
            this->instr_bin.r_type.rs1 = val;
        else if(op_idx == 2)
            this->instr_bin.r_type.rs2 = val;
    }
    else if(this->type == I_TYPE) {
        if(op_idx == 0)
            this->instr_bin.i_type.rd = val;
        else if(op_idx == 1)
            this->instr_bin.i_type.rs1 = val;
        else if(op_idx == 2)
            this->instr_bin.i_type.imm = val;
    }
}




void RiscvAssembler::set_source_file(std::string file_name)
{
    this->source_file = file_name;
    this->source_file_stream = std::ifstream(this->source_file);
}

void RiscvAssembler::assemble()
{
    if(this->source_file == "")
        asm_runtime_error("Souce file is empty!");

    std::string line;
    std::string current_label = "";
    while(std::getline(this->source_file_stream, line)) {
        // Check if line is an instruction
        if(line[0] == '\t' // Should be tabbed ; non-tabbed lines are most likely labels
            && line[1] != '.' // Should not start with '.' - these are assembler directives, which we ignore
        ) {
            if(current_label == "")
                asm_runtime_error("Got empty label for some instructions.");
            
            RiscvInstr instr(line);
            this->asm_instr[current_label].push_back(instr);
        }
        // Check if line is a label
        else if(
            line[0] == '_' && line[1] == 'Z' // Labels start with '_Z'
        ) {
            uint32_t label_len = line.find(":");
            current_label = line.substr(0, label_len);

            std::vector<RiscvInstr> instrs;
            this->asm_instr.insert({current_label, instrs});
        }
    }
}

uint32_t RiscvAssembler::estimated_binary_size_in_bytes()
{
    uint32_t sz = 0;

    std::map<std::string, std::vector<RiscvInstr>>::iterator it;  
    for (it = asm_instr.begin(); it != asm_instr.end(); it++) 
        sz += it->second.size();

    return sz * 4;
}

void RiscvAssembler::print_instructions()
{
    std::map<std::string, std::vector<RiscvInstr>>::iterator it;  
    for (it = asm_instr.begin(); it != asm_instr.end(); it++) {
        std::cout << it->first  << ':' << std::endl;

        for(int i=0; i<it->second.size(); i++) {
            if(it->second[i].get_instr_type() != NONE_TYPE)
                std::cout << "\t" << it->second[i].get_instr_string() << "; " << std::bitset<32>(it->second[i].get_instr_binary()) << std::endl;
        }
    }
}