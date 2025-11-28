#ifndef __PROJECT3A_H__
#define __PROJECT3A_H__

// minimal ISA

enum opcode {
  OPCODE_movb,
  OPCODE_addb,
  MAGIC_fill,
  MAGIC_set,
  MAGIC_scale,
  OPCODE_ERR
};

enum operand_type {
  OP_imm,
  OP_reg,
  OP_mem
};

struct operand {
  enum operand_type type;
  int value;
};

struct instruction {
  enum opcode opcode;
  struct operand src;
  struct operand dst;
};

// the architecture of the project 3A machine

struct cpu {
  unsigned char registers[8];
};

struct cache_line {
  unsigned int valid: 1; // 1 bit
  unsigned int tag:   4; // 4 bits
  unsigned char block[4];
};

struct set {
  struct cache_line lines[2];
};

struct cache {
  struct set sets[4];
};

struct machine {
  unsigned int clock_cycles;
  struct cpu cpu;
  struct cache cache;
  unsigned char memory[256];
};

void zero_machine(struct machine *m);

void apply_movb(struct machine *m, struct operand src, struct operand dst);

void apply_addb(struct machine *m, struct operand src, struct operand dst);

void do_fill(struct machine *m, struct operand src, struct operand dst);

void do_set(struct machine *m, struct operand src, struct operand dst);

void do_scale(struct machine *m, struct operand src, struct operand dst);

#endif
