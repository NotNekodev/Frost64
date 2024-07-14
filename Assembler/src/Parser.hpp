#ifndef _PARSER_HPP
#define _PARSER_HPP

#include <stddef.h>

#include <LinkedList.hpp>

#include "Lexer.hpp"

enum class Register {
    r0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7,
    r8,
    r9,
    r10,
    r11,
    r12,
    r13,
    r14,
    r15,
    scp,
    sbp,
    stp,
    cr0,
    cr1,
    cr2,
    cr3,
    flags,
    i0,
    i1,
    unknown
};

struct RegisterOffsetData {
    Register reg;
    uint64_t offset;
};

enum class OperandType {
    REGISTER,
    IMMEDIATE,
    MEMORY,
    REGISTER_OFFSET,
    POTENTIAL_MEMORY, // not sure if it is memory or register+offset
    LABEL,
    SUBLABEL
};

enum class OperandSize {
    BYTE,
    WORD,
    DWORD,
    QWORD
};

struct Operand {
    OperandType type;
    OperandSize size;
    void* data;
    bool complete;
};



enum class Opcode {
    PUSH,
    POP,
    PUSHA,
    POPA,
    ADD,
    MUL,
    SUB,
    DIV,
    OR,
    XOR,
    NOR,
    AND,
    NAND,
    NOT,
    CMP,
    INC,
    DEC,
    SHL,
    SHR,
    RET,
    CALL,
    JMP,
    JC,
    JNC,
    JZ,
    JNZ,
    INB,
    OUTB,
    INW,
    OUTW,
    IND,
    OUTD,
    INQ,
    OUTQ,
    INT,
    LIDT,
    IRET,
    MOV,
    NOP,
    HLT,
    UNKNOWN
};

struct Instruction {
    Opcode opcode;
    LinkedList::SimpleLinkedList<Operand> operands;
};

enum class RawDataType {
    RAW,
    LABEL,
    SUBLABEL
};

struct RawData {
    void* data;
    size_t data_size;
    RawDataType type;
};

struct Data {
    Data() {}
    ~Data() {}
    bool type; // true for instruction, false for raw data
    void* data;
};

struct Block {
    char* name;
    size_t name_size;
    LinkedList::SimpleLinkedList<Data> data_blocks;
    LinkedList::SimpleLinkedList<uint64_t> jumps_to_here;
};


struct Label {
    char* name;
    size_t name_size;
    LinkedList::SimpleLinkedList<Block> blocks;
};

class Parser {
public:
    Parser();
    ~Parser();

    void parse(const LinkedList::SimpleLinkedList<Token>& tokens);

    void PrintSections(FILE* fd) const;

    const LinkedList::SimpleLinkedList<Label>& GetLabels() const;

private:
    Opcode GetOpcode(const char* name, size_t name_size) const;
    Register GetRegister(const char* name, size_t name_size) const;
    void error(const char* message);

    const char* GetInstructionName(Opcode opcode) const;
    const char* GetRegisterName(Register reg) const;

private:
    LinkedList::SimpleLinkedList<Label> m_labels;
};

#endif /* _PARSER_HPP */