#include "riscv_asm.h"

void asm_runtime_error(std::string err)
{
    throw std::runtime_error("Assembler: " + err);
}

RiscvInstr::RiscvInstr(std::string instr, uint32_t instr_address, std::map<std::string, uint32_t>* label_addresses_ptr)
    : label_addresses_ptr(label_addresses_ptr),
    instr_address(instr_address)
{
    this->instr = instr;

    std::string line = this->instr.substr(1, this->instr.size());
            
    uint32_t sep_loc = line.find('\t');

    this->instr = line.substr(0, sep_loc);
    std::transform(this->instr.begin(), this->instr.end(), this->instr.begin(), ::tolower);
    line = line.substr(sep_loc+1, line.size());
    
    do {
        sep_loc = line.find(',');

        this->operands.push_back(line.substr(0, sep_loc));
        line = line.substr(sep_loc+1, line.size());
    } while(line != "" && sep_loc != -1);

    // Preserve original list of operands, make pre-processed copy
    std::smatch regex_match;
    for(uint32_t i=0; i<this->operands.size(); i++) {
        if(std::regex_search(this->operands[i], regex_match, this->operands_reg_offset_regex)) {
            this->operands_.push_back(regex_match[2]);
            this->operands_.push_back(regex_match[1]);
        }
        else
            this->operands_.push_back(this->operands[i]);
    }
}

void RiscvInstr::assemble_binary()
{
    this->assemble_op();
    if(this->type == NONE_TYPE) {
        asm_runtime_error("Could not assemble instruction " + this->instr);
        return;
    }

    for(uint32_t i=0; i<this->operands_.size(); i++)
        this->assemble_operand(i, this->operands_[i]);
}

void RiscvInstr::assemble_op()
{
    // Integer register-immediate
    // https://riscv.github.io/riscv-isa-manual/snapshot/unprivileged/#_integer_register_immediate_instructions
    if(this->instr == "addi") {
        this->type = I_TYPE;
        this->instr_bin.init_opcode(I_OPCODE);
        this->instr_bin.init_funct3(ADDI);
    }
    else if(this->instr == "mv") {
        this->operands_.push_back("0");

        this->type = I_TYPE;
        this->instr_bin.init_opcode(I_OPCODE);
        this->instr_bin.init_funct3(ADDI);
    }
    else if(this->instr == "slti") {
        this->type = I_TYPE;
        this->instr_bin.init_opcode(I_OPCODE);
        this->instr_bin.init_funct3(SLTI);
    }
    else if(this->instr == "sltiu") {
        this->type = I_TYPE;
        this->instr_bin.init_opcode(I_OPCODE);
        this->instr_bin.init_funct3(SLTIU);
    }
    else if(this->instr == "seqz") {
        this->operands_.push_back("1");

        this->type = I_TYPE;
        this->instr_bin.init_opcode(I_OPCODE);
        this->instr_bin.init_funct3(SLTIU);
    }
    else if(this->instr == "andi") {
        this->type = I_TYPE;
        this->instr_bin.init_opcode(I_OPCODE);
        this->instr_bin.init_funct3(ANDI);
    }
    else if(this->instr == "ori") {
        this->type = I_TYPE;
        this->instr_bin.init_opcode(I_OPCODE);
        this->instr_bin.init_funct3(ORI);
    }
    else if(this->instr == "xori") {
        this->type = I_TYPE;
        this->instr_bin.init_opcode(I_OPCODE);
        this->instr_bin.init_funct3(XORI);
    }
    else if(this->instr == "not") {
        this->operands_.push_back(std::to_string(~(0x1) + 0x1));

        this->type = I_TYPE;
        this->instr_bin.init_opcode(I_OPCODE);
        this->instr_bin.init_funct3(XORI);
    }
    else if(this->instr == "slli") {
        this->type = I_TYPE;
        this->instr_bin.init_opcode(I_OPCODE);
        this->instr_bin.init_funct3(SLLI);
    }
    else if(this->instr == "srli") {
        this->type = I_TYPE;
        this->instr_bin.init_opcode(I_OPCODE);
        this->instr_bin.init_funct3(SRLI);
    }
    else if(this->instr == "srai") {
        this->type = I_TYPE;
        this->instr_bin.init_opcode(I_OPCODE);
        this->instr_bin.init_funct3(SRAI);
    }
    else if(this->instr == "lui") {
        this->type = U_TYPE;
        this->instr_bin.init_opcode(LUI);
    }
    else if(this->instr == "auipc") {
        this->type = U_TYPE;
        this->instr_bin.init_opcode(AUIPC);
    }
    // Integer register-register
    // https://riscv.github.io/riscv-isa-manual/snapshot/unprivileged/#_integer_register_register_instructions
    else if(this->instr == "add") {
        this->type = R_TYPE;
        this->instr_bin.init_opcode(R_OPCODE);
        this->instr_bin.init_funct3(ADD);
        this->instr_bin.init_funct7(0x0);
    }
    else if(this->instr == "slt") {
        this->type = R_TYPE;
        this->instr_bin.init_opcode(R_OPCODE);
        this->instr_bin.init_funct3(SLT);
        this->instr_bin.init_funct7(0x0);
    }
    else if(this->instr == "sltu") {
        this->type = R_TYPE;
        this->instr_bin.init_opcode(R_OPCODE);
        this->instr_bin.init_funct3(SLTU);
        this->instr_bin.init_funct7(0x0);
    }
    else if(this->instr == "snez") {
        this->operands_.insert(this->operands_.begin() + 1, "zero");

        this->type = R_TYPE;
        this->instr_bin.init_opcode(R_OPCODE);
        this->instr_bin.init_funct3(SLTU);
        this->instr_bin.init_funct7(0x0);
    }
    else if(this->instr == "and") {
        this->type = R_TYPE;
        this->instr_bin.init_opcode(R_OPCODE);
        this->instr_bin.init_funct3(AND);
        this->instr_bin.init_funct7(0x0);
    }
    else if(this->instr == "or") {
        this->type = R_TYPE;
        this->instr_bin.init_opcode(R_OPCODE);
        this->instr_bin.init_funct3(OR);
        this->instr_bin.init_funct7(0x0);
    }
    else if(this->instr == "xor") {
        this->type = R_TYPE;
        this->instr_bin.init_opcode(R_OPCODE);
        this->instr_bin.init_funct3(XOR);
        this->instr_bin.init_funct7(0x0);
    }
    else if(this->instr == "sub") {
        this->type = R_TYPE;
        this->instr_bin.init_opcode(R_OPCODE);
        this->instr_bin.init_funct3(SUB);
        this->instr_bin.init_funct7(0x0);
    }
    else if(this->instr == "sll") {
        this->type = R_TYPE;
        this->instr_bin.init_opcode(R_OPCODE);
        this->instr_bin.init_funct3(SLL);
        this->instr_bin.init_funct7(0x20);
    }
    else if(this->instr == "srl") {
        this->type = R_TYPE;
        this->instr_bin.init_opcode(R_OPCODE);
        this->instr_bin.init_funct3(SRL);
        this->instr_bin.init_funct7(0x20);
    }
    else if(this->instr == "sra") {
        this->type = R_TYPE;
        this->instr_bin.init_opcode(R_OPCODE);
        this->instr_bin.init_funct3(SRA);
        this->instr_bin.init_funct7(0x20);
    }
    else if(this->instr == "nop") {
        this->operands_.push_back("zero");
        this->operands_.push_back("zero");
        this->operands_.push_back("0");

        this->type = I_TYPE;
        this->instr_bin.init_opcode(I_OPCODE);
        this->instr_bin.init_funct3(ADDI);
    }
    // Control transfer
    // https://riscv.github.io/riscv-isa-manual/snapshot/unprivileged/#_control_transfer_instructions
    else if(this->instr == "jal") {
        this->type = J_TYPE;
        this->instr_bin.init_opcode(JAL);
    }
    else if(this->instr == "call") {
        this->operands_.insert(this->operands_.begin(), "ra");

        this->type = J_TYPE;
        this->instr_bin.init_opcode(JAL);
    }
    else if(this->instr == "j") {
        this->operands_.insert(this->operands_.begin(), "zero");

        this->type = J_TYPE;
        this->instr_bin.init_opcode(JAL);
    }
    else if(this->instr == "jalr") {
        this->type = I_TYPE;
        this->instr_bin.init_opcode(JALR);
        this->instr_bin.init_funct3(0x0);
    }
    else if(this->instr == "jr") {
        this->operands_.insert(this->operands_.begin(), "zero");
        this->operands_.push_back("0");

        this->type = I_TYPE;
        this->instr_bin.init_opcode(JALR);
        this->instr_bin.init_funct3(0x0);
    }
    else if(this->instr == "ret") {
        this->operands_.push_back("zero");
        this->operands_.push_back("ra");
        this->operands_.push_back("0");

        this->type = I_TYPE;
        this->instr_bin.init_opcode(JALR);
        this->instr_bin.init_funct3(0x0);
    }
    else if(this->instr == "beq") {
        this->type = B_TYPE;
        this->instr_bin.init_opcode(BRANCH);
        this->instr_bin.init_funct3(BEQ);
    }
    else if(this->instr == "bne") {
        this->type = B_TYPE;
        this->instr_bin.init_opcode(BRANCH);
        this->instr_bin.init_funct3(BNE);
    }
    else if(this->instr == "blt") {
        this->type = B_TYPE;
        this->instr_bin.init_opcode(BRANCH);
        this->instr_bin.init_funct3(BLT);
    }
    else if(this->instr == "bltu") {
        this->type = B_TYPE;
        this->instr_bin.init_opcode(BRANCH);
        this->instr_bin.init_funct3(BLTU);
    }
    else if(this->instr == "bge") {
        this->type = B_TYPE;
        this->instr_bin.init_opcode(BRANCH);
        this->instr_bin.init_funct3(BGE);
    }
    else if(this->instr == "bgeu") {
        this->type = B_TYPE;
        this->instr_bin.init_opcode(BRANCH);
        this->instr_bin.init_funct3(BGEU);
    }
    // Load
    // https://riscv.github.io/riscv-isa-manual/snapshot/unprivileged/#ldst
    else if(this->instr == "lw") {
        this->type = I_TYPE;
        this->instr_bin.init_opcode(LOAD);
        this->instr_bin.init_funct3(LW);
    }
    else if(this->instr == "lh") {
        this->type = I_TYPE;
        this->instr_bin.init_opcode(LOAD);
        this->instr_bin.init_funct3(LH);
    }
    else if(this->instr == "lhu") {
        this->type = I_TYPE;
        this->instr_bin.init_opcode(LOAD);
        this->instr_bin.init_funct3(LHU);
    }
    else if(this->instr == "lb") {
        this->type = I_TYPE;
        this->instr_bin.init_opcode(LOAD);
        this->instr_bin.init_funct3(LB);
    }
    else if(this->instr == "lbu") {
        this->type = I_TYPE;
        this->instr_bin.init_opcode(LOAD);
        this->instr_bin.init_funct3(LBU);
    }
    // Store
    // https://riscv.github.io/riscv-isa-manual/snapshot/unprivileged/#ldst
    else if(this->instr == "sw") {
        this->type = S_TYPE;
        this->instr_bin.init_opcode(STORE);
        this->instr_bin.init_funct3(SW);
    }
    else if(this->instr == "sh") {
        this->type = S_TYPE;
        this->instr_bin.init_opcode(STORE);
        this->instr_bin.init_funct3(SH);
    }
    else if(this->instr == "shu") {
        this->type = S_TYPE;
        this->instr_bin.init_opcode(STORE);
        this->instr_bin.init_funct3(SHU);
    }
    else if(this->instr == "sb") {
        this->type = S_TYPE;
        this->instr_bin.init_opcode(STORE);
        this->instr_bin.init_funct3(SB);
    }
    else if(this->instr == "sbu") {
        this->type = S_TYPE;
        this->instr_bin.init_opcode(STORE);
        this->instr_bin.init_funct3(SBU);
    }
}

void RiscvInstr::assemble_operand(uint32_t op_idx, std::string operand)
{
    uint32_t val = 0;
    if (operand == "zero")
        val = X0;
    else if(operand == "ra")
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
    else if((operand[0] == '_' && operand[1] == 'Z') ||
            (operand[0] == '.' && operand[1] == 'L')) {
        uint32_t label_address = (*this->label_addresses_ptr)[operand];
        val = static_cast<uint32_t>(
            static_cast<int>(label_address) - 
            static_cast<int>(this->instr_address)
        );
    }
    else {
        try {
            val = static_cast<uint32_t>(std::stoi(operand));
        }
        catch (std::invalid_argument e) {
            asm_runtime_error("Could not process '" + this->instr + "' instruction's operand: " + operand);
        }
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
    else if(this->type == U_TYPE) {
        if(op_idx == 0)
            this->instr_bin.u_type.rd = val;
        else if(op_idx == 1)
            this->instr_bin.u_type.imm = val;
    }
    else if(this->type == J_TYPE) {
        if(op_idx == 0)
            this->instr_bin.j_type.rd = val;
        else if(op_idx == 1) {
            this->instr_bin.j_type.imm_19_12 = val >> 11;
            this->instr_bin.j_type.imm_11 = val >> 10;
            this->instr_bin.j_type.imm_10_1 = val;
            this->instr_bin.j_type.imm_20 = val >> 19;
        }
    }
    else if(this->type == B_TYPE) {
        if(op_idx == 0)
            this->instr_bin.b_type.rs1 = val;
        else if(op_idx == 1)
            this->instr_bin.b_type.rs2 = val;
        else if(op_idx == 2) {
            this->instr_bin.b_type.imm_11 = val >> 10;
            this->instr_bin.b_type.imm_4_1 = val;
            this->instr_bin.b_type.imm_10_5 = val >> 4;
            this->instr_bin.b_type.imm_12 = val >> 11;
        }
    }
    else if(this->type == S_TYPE) {
        if(op_idx == 0)
            this->instr_bin.s_type.rs1 = val;
        else if(op_idx == 1)
            this->instr_bin.s_type.rs2 = val;
        else if(op_idx == 2) {
            this->instr_bin.s_type.imm_4_0 = val;
            this->instr_bin.s_type.imm_11_5 = val >> 4;
        }
    }
}




void RiscvAssembler::set_source_file(std::string file_name)
{
    this->source_file = file_name;
    this->source_file_stream = std::ifstream(this->source_file);
}

bool RiscvAssembler::is_label(std::string line)
{
    return line[0] == '_' && line[1] == 'Z' // Labels start with '_Z'
            || line[0] == '.' && line[1] == 'L'; // Labels start with '.L' tooS
}

bool RiscvAssembler::is_instruction(std::string line)
{
    return line[0] == '\t' // Should be tabbed ; non-tabbed lines are most likely labels
            && line[1] != '.'; // Should not start with '.' - these are assembler directives, which we ignore
}

void RiscvAssembler::preprocess()
{
    // Clear out the clutter - only work with labels or instructions
    std::string line;
    uint32_t address = 0;
    while(std::getline(this->source_file_stream, line)) {
        if(this->is_instruction(line)) {
            this->asm_clean.push_back(std::pair<std::string, uint32_t>(line, address));
            address += 4;
        }
        else if(this->is_label(line)) {
            line = line.substr(0, line.size()-1);
            this->asm_clean.push_back(std::pair<std::string, uint32_t>(line, address));
        }
    }

    // Record addresses corresponding to labels
    for(auto &item : this->asm_clean) {
        if(this->is_label(item.first))
            this->label_addresses.insert({item.first, item.second});
    }
}

void RiscvAssembler::assemble()
{
    if(this->source_file == "")
        asm_runtime_error("Souce file is empty!");

    this->preprocess();

    std::string current_label = "";
    for(auto &item : this->asm_clean) {
        if(this->is_instruction(item.first)) {
            if(current_label == "")
                asm_runtime_error("Got empty label for some instructions.");
            
            RiscvInstr instr(item.first, item.second, &label_addresses);
            instr.assemble_binary();
            this->asm_instr[current_label].push_back(instr);
        }
        // Check if line is a label
        else if(this->is_label(item.first)) {
            uint32_t label_len = item.first.find(":");
            current_label = item.first.substr(0, label_len);

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
        std::cout << it->first  << ": (" << this->label_addresses[it->first] << ")" << std::endl;

        for(int i=0; i<it->second.size(); i++) {
            if(it->second[i].get_instr_type() != NONE_TYPE) {
                std::cout << "\t" << it->second[i].get_instr_string() << " ";
                
                auto operands = it->second[i].get_operand_strings();
                for(auto& op : operands)
                    std::cout << op << " ";

                std::cout << " [" << std::bitset<32>(it->second[i].get_instr_binary()) << "]" << std::endl;
            }
        }
    }
}