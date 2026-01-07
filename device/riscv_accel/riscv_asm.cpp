#include "riscv_asm.h"

Assembler* get_assembler()
{
    return new CaAsm();
}

void runtime_error(std::string err)
{
    throw std::runtime_error("Assembler: " + err);
}

void RiscvInstrs::add_instruction(std::string instr)
{
    this->instruction.push_back(instr);
    this->num_ops.push_back(0);

    this->op1.push_back("");
    this->op2.push_back("");
    this->op3.push_back("");
    this->op4.push_back("");
    this->op5.push_back("");

    this->current_idx = this->instruction.size() - 1;
}

void RiscvInstrs::instruction_operand(uint32_t op_num, std::string op)
{
    if(this->current_idx == -1)
        runtime_error("Operands are defined without adding an instruction first");

    this->num_ops[this->current_idx] = this->num_ops[this->current_idx] + 1;

    if(op_num == 1)
        this->op1[this->current_idx] = op;
    else if(op_num == 2)
        this->op2[this->current_idx] = op;
    else if(op_num == 3)
        this->op3[this->current_idx] = op;
    else if(op_num == 4)
        this->op4[this->current_idx] = op;
    else if(op_num == 5)
        this->op5[this->current_idx] = op;
    else
        runtime_error("More operands than available are being set");
}

void RiscvInstrs::clear()
{
    this->current_idx = -1;
    this->instruction.clear();
    this->op1.clear();
    this->op2.clear();
    this->op3.clear();
    this->op4.clear();
    this->op5.clear();
}

void CaAsm::set_source_file(std::string file_name)
{
    this->source_file = file_name;
    this->source_file_stream = std::ifstream(this->source_file);

    this->preprocess();
}

void CaAsm::preprocess()
{
    std::string line;
    std::string current_label = "";
    RiscvInstrs cuurrent_instrs;
    bool is_processing_label = false;
    uint32_t sep_loc = 0;
    while(std::getline(this->source_file_stream, line)) {
        // Check if line is an instruction
        if(line[0] == '\t' // Should be tabbed ; non-tabbed lines are most likely labels
            && line[1] != '.' // Should not start with '.' - these are assembler directives, which we ignore
        ) {
            if(current_label == "")
                runtime_error("Got empty label for some commands.");
            
            line = line.substr(1, line.size());
            
            sep_loc = line.find('\t');
            cuurrent_instrs.add_instruction(line.substr(0, sep_loc));
            
            line = line.substr(sep_loc+1, line.size());
            
            uint32_t op_idx = 1;
            do {
                sep_loc = line.find(',');
                cuurrent_instrs.instruction_operand(op_idx, line.substr(0, sep_loc));
                line = line.substr(sep_loc+1, line.size());

                op_idx++;
            } while(line != "" && sep_loc != -1);
        }

        // Check if line is a label
        else if(
            line[0] == '_' && line[1] == 'Z' // Labels start with '_Z'
        ) {
            if(is_processing_label) {
                this->asm_instr.insert({current_label, cuurrent_instrs});
                cuurrent_instrs.clear();
                current_label = "";
            }

            is_processing_label = true;

            uint32_t label_len = line.find(":");
            current_label = line.substr(0, label_len);
        }
    }

    this->asm_instr.insert({current_label, cuurrent_instrs});
}

uint32_t CaAsm::estimated_binary_size_in_bytes()
{
    uint32_t sz;

    std::map<std::string, RiscvInstrs>::iterator it;  
    for (it = asm_instr.begin(); it != asm_instr.end(); it++) 
        sz += it->second.instruction.size();

    return sz * 4;
}

void CaAsm::assemble()
{
    if(this->source_file == "")
        runtime_error("Souce file is empty!");
}

void CaAsm::print_instructions()
{
    std::map<std::string, RiscvInstrs>::iterator it;  
    for (it = asm_instr.begin(); it != asm_instr.end(); it++) {
        std::cout << it->first  << ':' << std::endl;

        for(int i=0; i<it->second.instruction.size(); i++) {
            std::cout << "\t" << it->second.instruction[i] << " " 
                << it->second.op1[i] << " "
                << it->second.op2[i] << " "
                << it->second.op3[i] << " "
                << it->second.op4[i] << " "
                << it->second.op5[i] << ";" << std::endl;
        }
    }
}