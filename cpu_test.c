#include "unity.h"
#include "cpu.h"

static struct cpu_t cpu;
static struct prgm_t prgm;
static struct state_t state;

void setUp(void) {
    prgm.length = 0;
    state.pc = 0;
    cpu_init(&cpu, &prgm, &state);
}

void test_Cpu_should_NotIncPcOnEmptyProgram(void) {
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(0, state.pc);
}

void test_Cpu_should_IncPcOnNop(void) {
    prgm.length = 2;
    prgm.instrs[0] = (struct instr_t){OP_NOP, ARG_NONE, ARG_NONE};
    prgm.instrs[1] = (struct instr_t){OP_NOP, ARG_NONE, ARG_NONE};
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(1, state.pc);
}

void test_Cpu_should_IncPcTwiceOnTwoNop(void) {
    prgm.length = 3;
    prgm.instrs[0] = (struct instr_t){OP_NOP, ARG_NONE, ARG_NONE};
    prgm.instrs[1] = (struct instr_t){OP_NOP, ARG_NONE, ARG_NONE};
    prgm.instrs[2] = (struct instr_t){OP_NOP, ARG_NONE, ARG_NONE};
    cpu_step(&cpu);
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(2, state.pc);
}

void test_Cpu_should_WrapAroundPcOnPrgmEnd(void) {
    prgm.length = 2;
    prgm.instrs[0] = (struct instr_t){OP_NOP, ARG_NONE, ARG_NONE};
    prgm.instrs[1] = (struct instr_t){OP_NOP, ARG_NONE, ARG_NONE};
    cpu_step(&cpu);
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(0, state.pc);
}

void test_Cpu_should_SetPcOnJump(void) {
    prgm.length = 2;
    prgm.instrs[0] = (struct instr_t){OP_JMP, 0, ARG_NONE};
    prgm.instrs[1] = (struct instr_t){OP_NOP, ARG_NONE, ARG_NONE};
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(0, state.pc);
}

void test_Cpu_should_ClampPcOnJumpToNegative(void) {
    prgm.length = 2;
    prgm.instrs[0] = (struct instr_t){OP_JMP, -5, ARG_NONE};
    prgm.instrs[1] = (struct instr_t){OP_NOP, ARG_NONE, ARG_NONE};
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(0, state.pc);
}
