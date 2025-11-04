#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project3a.h"

void TODO(char *thing)
{
  fprintf(stderr,"TODO: %s\n",thing);
  exit(1);
}

void zero_machine(struct machine *m)
{
  TODO("zero_machine");
}

void apply_movb(struct machine *m, struct operand src, struct operand dst)
{
  TODO("apply_movb");
}

void apply_addb(struct machine *m, struct operand src, struct operand dst)
{
  TODO("apply_addb");
}

void do_fill(struct machine *m, struct operand src, struct operand dst)
{
  TODO("do_fill");
}

void do_set(struct machine *m, struct operand src, struct operand dst)
{
  TODO("do_set");
}

void do_scale(struct machine *m, struct operand src, struct operand dst)
{
  TODO("do_scale");
}

void apply_instruction(struct machine *m, struct instruction i)
{
  switch (i.opcode) {
  case OPCODE_movb:
    apply_movb(m,i.src,i.dst);
    return;
  case OPCODE_addb:
    apply_addb(m,i.src,i.dst);
    return;
  case MAGIC_fill:
    do_fill(m,i.src,i.dst);
    return;
  case MAGIC_set:
    do_set(m,i.src,i.dst);
    return;
  case MAGIC_scale:
    do_scale(m,i.src,i.dst);
    return;
  default:
    fprintf(stderr,"(apply_instruction) bad opcode %d\n",i.opcode);
    exit(1);
  }
}

/* ==== repl ==== */

char *triml(char *s)
{
  char *t = s;
  while (s[0]==' ' || s[0]=='\t' || s[0]=='\n')
    ++t;
  return t;
}

char *trimr(char *s)
{
  int n = strlen(s);
  for (int i=n-1; i>=0; i--)
    if (s[i]==' ' || s[i]=='\t' || s[i]=='\n')
      s[i] = '\0';
    else
      return s;
  return s;
}

char *trim(char *s)
{
  return triml(trimr(s));
}

char *opcode_tos(enum opcode c)
{
  switch (c) {
  case OPCODE_movb: return "movb";
  case OPCODE_addb: return "addb";
  case MAGIC_fill: return ".fill";
  case MAGIC_set: return ".set";
  case MAGIC_scale: return ".scale";
  case OPCODE_ERR: return "OPCODE_ERR";
  }
  fprintf(stderr,"opcode_tos: bad value %d\n",c);
  exit(1);
}

char *operand_tos(struct operand op)
{
 char buf[64] = {0};
  if (op.type==OP_imm)
    snprintf(buf,64,"$%d",op.value);
  else if (op.type==OP_reg)
    snprintf(buf,64,"r%d",op.value);
  else if (op.type==OP_mem)
    snprintf(buf,64,"mem[%d]",op.value);
  else {
    fprintf(stderr, "operand_tos: bad value %d\n", op.type);
    exit(1);
  }
  char *r = strdup(buf);
  return r;
}

void print_instruction(struct instruction i)
{
  char *src_str = operand_tos(i.src);
  char *dst_str = operand_tos(i.dst);
  printf("%s %s %s\n",opcode_tos(i.opcode),src_str,dst_str);
  free(src_str);
  free(dst_str);
}

enum opcode parse_opcode(char *s)
{
  if (!strcmp(s,"movb"))
    return OPCODE_movb;
  else if (!strcmp(s,"addb"))
    return OPCODE_addb;
  else if (!strcmp(s,".fill"))
    return MAGIC_fill;
  else if (!strcmp(s,".set"))
    return MAGIC_set;
  else if (!strcmp(s,".scale"))
    return MAGIC_scale;
  else
    return OPCODE_ERR;
}

struct operand parse_operand(char *s)
{
  struct operand result;
  if (s && s[0]) {
    if (s[0]=='r') {
      result.type = OP_reg;
      result.value = atoi(s+1);
      return result;
    } else if (s[0]=='$') {
      result.type = OP_imm;
      result.value = atoi(s+1);
      return result;
    } else if ('0'<=s[0] && s[0]<='9') {
      result.type = OP_mem;
      result.value = atoi(s);
      return result;
    }
  }
  fprintf(stderr,"cannot parse operand \"%s\"\n",s?s:"NULL");
  exit(1);
}

int parse_instruction(char *s, struct instruction *inst)
{
  char *d = strdup(s);
  char *op = strtok(d," ");
  char *ssrc = strtok(NULL," ");
  char *sdst = strtok(NULL," ");
  inst->opcode = parse_opcode(op);
  if (inst->opcode==OPCODE_ERR)
    return 0;
  inst->src = parse_operand(ssrc);
  inst->dst = parse_operand(sdst);
  free(d);
  return 1;
}

int starts_with(char *prefix, char *s)
{
  int plen = strlen(prefix);
  int slen = strlen(s);
  if (slen<plen)
    return 0;
  for (int i=0; i<plen; ++i)
    if (prefix[i]!=s[i])
      return 0;
  return 1;
}

struct machine machine_new()
{
  struct machine m;
  zero_machine(&m);
  return m;
}

void show_registers(struct machine *m)
{
  printf("reg:  ");
  for (int i=0; i<8; i++)
    printf("0X%02X ", m->cpu.registers[i]);
  printf("\n");
}

void show_memory(struct machine *m)
{
  printf("mem\n");
  for (int i=0; i<32; i++) {
    printf("0X%02X: ", i*8);
    for (int j=0; j<8; j++)
      printf("0X%02X ", m->memory[i*8+j]);
    putchar('\n');
  }
}

void show_cache(struct machine *m)
{
  printf("cache\n");
  for (int i=0; i<4; i++) {
    printf("set %d --------\n",i);
    for (int j=0; j<2; j++) {
      struct cache_line line = m->cache.sets[i].lines[j];
      if (line.valid) {
        printf(" (%d) valid(1) tag(0X%X) [0X%02X 0X%02X 0X%02X 0X%02X]\n",
               j,
               line.tag,
               line.block[0],
               line.block[1],
               line.block[2],
               line.block[3]);
      } else {
        printf(" (%d) valid(0) ...\n",j);
      }
    }
  }
}

void help()
{
  printf("--- shell commands\n");
  printf(".h help\n");
  printf(".q quit\n");
  printf(".r show registers\n");
  printf(".c show cache\n");
  printf(".m show memory\n");
  printf(".z (\"zero\") reset machine\n");
  putchar('\n');
  printf("--- machine instructions\n");
  printf("movb [operand1] [operand2]\n");
  printf("addb [operand1] [operand2]\n");
  putchar('\n');
  printf("--- magic\n");
  printf(".fill  [operand1] [operand2]\n");
  printf(".set   [operand1] [operand2]\n");
  printf(".scale [operand1] [operand2]\n");
  putchar('\n');
  printf("An operand is either\n");
  printf("- a literal of the form $12 (decimal),\n");
  printf("- a register r0, r1, ..., r7, or\n");
  printf("- a memory address 0, 1, ..., 255.\n");
}

int main() {
  printf("*** WELCOME TO THE PROJECT 3A MACHINE ***\n");
  printf("type .h for help\n");
  struct machine m = machine_new();
  char buffer[80]={0};
  while (1) {
    printf("[%05d] $$ ",m.clock_cycles);
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
      char *t = trim(buffer);
      if (!strcmp(".q",t)) {
        printf("Bye!\n");
        return 0;
      } else if (!strcmp(".m",t)) {
        show_memory(&m);
        putchar('\n');
      } else if (!strcmp(".h",t)) {
        help();
      } else if (!strcmp(".r",t)) {
        show_registers(&m);
        putchar('\n');
      } else if (!strcmp(".c",t)) {
        show_cache(&m);
        putchar('\n');
      } else if (!strcmp(".z",t)) {
        zero_machine(&m);
        putchar('\n');
      } else {
        struct instruction inst;
        if (parse_instruction(t,&inst)) {
          //print_instruction(inst);
          apply_instruction(&m,inst);
        } else {
          printf("unrecognized command\n");
        }
      }
    }
  }
  return 0;
}
