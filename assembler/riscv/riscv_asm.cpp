#include "riscv_asm.h"

Assembler* get_assembler()
{
    return new RiscvAssembler();
}



void RiscvAssembler::set_source_file(std::string file_name)
{
    this->source_file = file_name;
    this->source_file_stream = std::ifstream(this->source_file);
    if(this->source_file_stream.fail())
        asm_runtime_error("Source file '" + file_name + "' not found");
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

std::vector<uint32_t> RiscvAssembler::get_binary()
{
    std::vector<uint32_t> bin;
    for(auto& item : this->asm_instr)
        for(auto &instr : item.second)
            bin.push_back(instr.get_instr_binary());

    return bin;
}

void RiscvAssembler::debug_print()
{
    std::map<std::string, std::vector<RiscvInstr>>::iterator it;  
    for (it = this->asm_instr.begin(); it != this->asm_instr.end(); it++) {
        std::cout << it->first  << ": (address " << this->label_addresses[it->first] << ")" << std::endl;

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

void RiscvAssembler::check_instr_eq_either(RiscvInstr instr, std::string s1, std::string s2)
{
    if(!this->check_instr_eq(instr, s1, false) &&
        !this->check_instr_eq(instr, s2))
        this->check_instr_eq(instr, s1);
}

void RiscvAssembler::check_instr_eq_either(RiscvInstr instr, std::string s1, std::string s2, std::string s3)
{
    if(!this->check_instr_eq(instr, s1, false) &&
        !this->check_instr_eq(instr, s2, false) &&
        !this->check_instr_eq(instr, s3, false))
        this->check_instr_eq(instr, s1);

}

bool RiscvAssembler::check_instr_eq(RiscvInstr instr, std::string s1, bool exit)
{
    if(instr.get_instr_string() != s1) {
        std::string err = "Disassembly of instruction: '" + instr.get_instr_string() + "' failed.";

        if(exit)
            dasm_runtime_error(err);
        else
            return false;
    }

    return true;
}

void RiscvAssembler::instr_check(RiscvInstr& instr)
{
    uint32_t bits = instr.get_instr_binary();
    RISCV_INSTR_BIN* bin = (RISCV_INSTR_BIN*)(&bits);

    if(bin->r_type.opcode == R_OPCODE) {
        if(bin->r_type.funct3 == ADD && bin->r_type.funct7 == 0x0)
            check_instr_eq(instr, "add");
        else if(bin->r_type.funct3 == SLT && bin->r_type.funct7 == 0x0)
            check_instr_eq(instr, "slt");
        else if(bin->r_type.funct3 == SLTU && bin->r_type.funct7 == 0x0)
            check_instr_eq_either(instr, "sltu", "snez");
        else if(bin->r_type.funct3 == AND && bin->r_type.funct7 == 0x0)
            check_instr_eq(instr, "and");
        else if(bin->r_type.funct3 == OR && bin->r_type.funct7 == 0x0)
            check_instr_eq(instr, "or");
        else if(bin->r_type.funct3 == XOR && bin->r_type.funct7 == 0x0)
            check_instr_eq(instr, "xor");
        else if(bin->r_type.funct3 == SUB && bin->r_type.funct7 == 0x0)
            check_instr_eq(instr, "sub");
        else if(bin->r_type.funct3 == SLL && bin->r_type.funct7 == 0x20)
            check_instr_eq(instr, "sll");
        else if(bin->r_type.funct3 == SRL && bin->r_type.funct7 == 0x20)
            check_instr_eq(instr, "srl");
        else if(bin->r_type.funct3 == SRA && bin->r_type.funct7 == 0x20)
            check_instr_eq(instr, "sra");
    }
    else if(bin->r_type.opcode == I_OPCODE) {
        if(bin->i_type.funct3 == ADDI)
            check_instr_eq_either(instr, "addi", "mv", "nop");
        else if(bin->i_type.funct3 == SLTI)
            check_instr_eq(instr, "slti");
        else if(bin->i_type.funct3 == SLTIU)
            check_instr_eq_either(instr, "sltiu", "seqz");
        else if(bin->i_type.funct3 == ANDI)
            check_instr_eq(instr, "andi");
        else if(bin->i_type.funct3 == ORI)
            check_instr_eq(instr, "ori");
        else if(bin->i_type.funct3 == XORI)
            check_instr_eq_either(instr, "xori", "not");
        else if(bin->i_type.funct3 == SLLI)
            check_instr_eq(instr, "slli");
        else if(bin->i_type.funct3 == SRLI)
            check_instr_eq(instr, "srli");
        else if(bin->i_type.funct3 == SRAI)
            check_instr_eq(instr, "srai");
    }
    else if(bin->r_type.opcode == LUI)
        check_instr_eq(instr, "lui");
    else if(bin->r_type.opcode == AUIPC)
        check_instr_eq(instr, "auipc");
    else if(bin->r_type.opcode == JAL)
        check_instr_eq_either(instr, "jal", "call", "j");
    else if(bin->r_type.opcode == JALR)
        check_instr_eq_either(instr, "jalr", "jr", "ret");
    else if(bin->r_type.opcode == BRANCH) {
        if(bin->b_type.funct3 == BEQ)
            check_instr_eq(instr, "beq");
        else if(bin->b_type.funct3 == BNE)
            check_instr_eq(instr, "bne");
        else if(bin->b_type.funct3 == BLT)
            check_instr_eq(instr, "blt");
        else if(bin->b_type.funct3 == BLTU)
            check_instr_eq(instr, "bltu");
        else if(bin->b_type.funct3 == BGE)
            check_instr_eq(instr, "bge");
        else if(bin->b_type.funct3 == BGEU)
            check_instr_eq(instr, "bgeu");
    }
    else if(bin->r_type.opcode == LOAD) {
        if(bin->i_type.funct3 == LW)
            check_instr_eq(instr, "lw");
        else if(bin->i_type.funct3 == LH)
            check_instr_eq(instr, "lh");
        else if(bin->i_type.funct3 == LHU)
            check_instr_eq(instr, "lhu");
        else if(bin->i_type.funct3 == LB)
            check_instr_eq(instr, "lb");
        else if(bin->i_type.funct3 == LBU)
            check_instr_eq(instr, "lbu");
    }
    else if(bin->r_type.opcode == STORE) {
        if(bin->s_type.funct3 == SW)
            check_instr_eq(instr, "sw");
        else if(bin->s_type.funct3 == SH)
            check_instr_eq(instr, "sh");
        else if(bin->s_type.funct3 == SHU)
            check_instr_eq(instr, "shu");
        else if(bin->s_type.funct3 == SB)
            check_instr_eq(instr, "sb");
        else if(bin->s_type.funct3 == SBU)
            check_instr_eq(instr, "sbu");
    }
    else if(bin->r_type.opcode == JALR) {
        if(bin->j_type.rd == 0x0)
            check_instr_eq(instr, "jr");
        else
            check_instr_eq(instr, "jalr");
    }
    else {
        std::string err = "No disassembly rule for instruction '" + instr.get_instr_string() + "' found ";
        dasm_runtime_error(err);
    }
}

void RiscvAssembler::operands_check(RiscvInstr& instr)
{
    uint32_t bits = instr.get_instr_binary();
    RISCV_INSTR_BIN* bin = (RISCV_INSTR_BIN*)(&bits);

    if(instr.get_instr_type() == R_TYPE) {

    }
    else if(instr.get_instr_type() == I_TYPE) {
        
    }
    else if(instr.get_instr_type() == B_TYPE) {
        
    }
    else if(instr.get_instr_type() == S_TYPE) {
        
    }
    else if(instr.get_instr_type() == U_TYPE) {
        
    }
    else if(instr.get_instr_type() == J_TYPE) {
        
    }
    else {
        std::string err = "No disassembly rule for instruction '" + instr.get_instr_string() + "' found ";
        dasm_runtime_error(err);
    }
}

void RiscvAssembler::disassembly_check()
{
    std::map<uint32_t, std::string> label_addresses_inv;
    for(auto& item : this->label_addresses)
        label_addresses_inv.insert({item.second, item.first});

    std::map<std::string, std::vector<RiscvInstr>>::iterator it;  
    for (it = asm_instr.begin(); it != asm_instr.end(); it++) {
        for(auto& instr : it->second) {
            this->instr_check(instr);
            this->operands_check(instr);
        }
    }
}