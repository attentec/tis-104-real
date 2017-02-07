#include "unity.h"
#include "cpu.h"

#define INSTR0(op)             ((struct instr_t){(op), ARG_NONE, ARG_NONE})
#define INSTR1(op, arg1)       ((struct instr_t){(op), (arg1), ARG_NONE})
#define INSTR2(op, arg1, arg2) ((struct instr_t){(op), (arg1), (arg2)})

static struct cpu_t cpu;
static struct prgm_t prgm;
static struct state_t state;

void setUp(void) {
    prgm.length = 0;
    state.pc = 0;
    state.acc = 0;
    state.bak = 0;
    cpu_init(&cpu, &prgm, &state);
}

void test_Cpu_should_NotIncPcOnEmptyProgram(void) {
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(0, state.pc);
}

void test_Cpu_should_IncPcOnNop(void) {
    prgm.length = 2;
    prgm.instrs[0] = INSTR0(OP_NOP);
    prgm.instrs[1] = INSTR0(OP_NOP);
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(1, state.pc);
}

void test_Cpu_should_IncPcTwiceOnTwoNop(void) {
    prgm.length = 3;
    prgm.instrs[0] = INSTR0(OP_NOP);
    prgm.instrs[1] = INSTR0(OP_NOP);
    prgm.instrs[2] = INSTR0(OP_NOP);
    cpu_step(&cpu);
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(2, state.pc);
}

void test_Cpu_should_WrapAroundPcOnPrgmEnd(void) {
    prgm.length = 2;
    prgm.instrs[0] = INSTR0(OP_NOP);
    prgm.instrs[1] = INSTR0(OP_NOP);
    cpu_step(&cpu);
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(0, state.pc);
}

void test_Cpu_should_SetPcOnJump(void) {
    prgm.length = 2;
    prgm.instrs[0] = INSTR1(OP_JMP, 0);
    prgm.instrs[1] = INSTR0(OP_NOP);
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(0, state.pc);
}

void test_Cpu_should_HaltUsingJro(void) {
    prgm.length = 5;
    prgm.instrs[0] = INSTR0(OP_NOP);
    prgm.instrs[1] = INSTR0(OP_NOP);
    prgm.instrs[2] = INSTR1(OP_JRO, 0);
    prgm.instrs[3] = INSTR0(OP_NOP);
    prgm.instrs[4] = INSTR0(OP_NOP);
    state.pc = 2;
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(2, state.pc);
}

void test_Cpu_should_JumpBackwardUsingJro(void) {
    prgm.length = 5;
    prgm.instrs[0] = INSTR0(OP_NOP);
    prgm.instrs[1] = INSTR0(OP_NOP);
    prgm.instrs[2] = INSTR1(OP_JRO, -1);
    prgm.instrs[3] = INSTR0(OP_NOP);
    prgm.instrs[4] = INSTR0(OP_NOP);
    state.pc = 2;
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(1, state.pc);
}

void test_Cpu_should_JumpForwardUsingJro(void) {
    prgm.length = 5;
    prgm.instrs[0] = INSTR0(OP_NOP);
    prgm.instrs[1] = INSTR0(OP_NOP);
    prgm.instrs[2] = INSTR1(OP_JRO, 1);
    prgm.instrs[3] = INSTR0(OP_NOP);
    prgm.instrs[4] = INSTR0(OP_NOP);
    state.pc = 2;
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(3, state.pc);
}

void test_Cpu_should_ClampPcOnJroToNegative(void) {
    prgm.length = 5;
    prgm.instrs[0] = INSTR0(OP_NOP);
    prgm.instrs[1] = INSTR0(OP_NOP);
    prgm.instrs[2] = INSTR1(OP_JRO, -20);
    prgm.instrs[3] = INSTR0(OP_NOP);
    prgm.instrs[4] = INSTR0(OP_NOP);
    state.pc = 2;
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(0, state.pc);
}

void test_Cpu_should_ClampPcOnJroToFarPositive(void) {
    prgm.length = 5;
    prgm.instrs[0] = INSTR0(OP_NOP);
    prgm.instrs[1] = INSTR0(OP_NOP);
    prgm.instrs[2] = INSTR1(OP_JRO, 20);
    prgm.instrs[3] = INSTR0(OP_NOP);
    prgm.instrs[4] = INSTR0(OP_NOP);
    state.pc = 2;
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(4, state.pc);
}

void test_Cpu_should_JumpOnJezIfEqualToZero(void) {
    prgm.length = 3;
    prgm.instrs[0] = INSTR0(OP_NOP);
    prgm.instrs[1] = INSTR1(OP_JEZ, 0);
    prgm.instrs[2] = INSTR0(OP_NOP);
    state.pc = 1;
    state.acc = 0;
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(0, state.pc);
}

void test_Cpu_should_JumpToArg1OnJezIfEqualToZero(void) {
    prgm.length = 3;
    prgm.instrs[0] = INSTR0(OP_NOP);
    prgm.instrs[1] = INSTR1(OP_JEZ, 1);
    prgm.instrs[2] = INSTR0(OP_NOP);
    state.pc = 1;
    state.acc = 0;
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(1, state.pc);
}

void test_Cpu_should_NotJumpOnJezIfNotEqualToZero(void) {
    prgm.length = 3;
    prgm.instrs[0] = INSTR0(OP_NOP);
    prgm.instrs[1] = INSTR1(OP_JEZ, 0);
    prgm.instrs[2] = INSTR0(OP_NOP);
    state.pc = 1;
    state.acc = 5;
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(2, state.pc);
}

void test_Cpu_should_JumpOnJnzIfNotEqualToZero(void) {
    prgm.length = 3;
    prgm.instrs[0] = INSTR0(OP_NOP);
    prgm.instrs[1] = INSTR1(OP_JNZ, 0);
    prgm.instrs[2] = INSTR0(OP_NOP);
    state.pc = 1;
    state.acc = 5;
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(0, state.pc);
}

void test_Cpu_should_JumpToArg1OnJnzIfNotEqualToZero(void) {
    prgm.length = 3;
    prgm.instrs[0] = INSTR0(OP_NOP);
    prgm.instrs[1] = INSTR1(OP_JNZ, 1);
    prgm.instrs[2] = INSTR0(OP_NOP);
    state.pc = 1;
    state.acc = 5;
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(1, state.pc);
}

void test_Cpu_should_NotJumpOnJnzIfEqualToZero(void) {
    prgm.length = 3;
    prgm.instrs[0] = INSTR0(OP_NOP);
    prgm.instrs[1] = INSTR1(OP_JNZ, 0);
    prgm.instrs[2] = INSTR0(OP_NOP);
    state.pc = 1;
    state.acc = 0;
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(2, state.pc);
}

void test_Cpu_should_JumpOnJgzIfGreaterThanZero(void) {
    prgm.length = 3;
    prgm.instrs[0] = INSTR0(OP_NOP);
    prgm.instrs[1] = INSTR1(OP_JGZ, 0);
    prgm.instrs[2] = INSTR0(OP_NOP);
    state.pc = 1;
    state.acc = 5;
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(0, state.pc);
}

void test_Cpu_should_JumpToArg1OnJgzIfGreaterThanZero(void) {
    prgm.length = 3;
    prgm.instrs[0] = INSTR0(OP_NOP);
    prgm.instrs[1] = INSTR1(OP_JGZ, 1);
    prgm.instrs[2] = INSTR0(OP_NOP);
    state.pc = 1;
    state.acc = 5;
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(1, state.pc);
}

void test_Cpu_should_NotJumpOnJgzIfLessThanOrEqualToZero(void) {
    prgm.length = 3;
    prgm.instrs[0] = INSTR0(OP_NOP);
    prgm.instrs[1] = INSTR1(OP_JGZ, 0);
    prgm.instrs[2] = INSTR0(OP_NOP);
    state.pc = 1;
    state.acc = 0;
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(2, state.pc);
}

void test_Cpu_should_JumpOnJlzIfLessThanZero(void) {
    prgm.length = 3;
    prgm.instrs[0] = INSTR0(OP_NOP);
    prgm.instrs[1] = INSTR1(OP_JLZ, 0);
    prgm.instrs[2] = INSTR0(OP_NOP);
    state.pc = 1;
    state.acc = -5;
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(0, state.pc);
}

void test_Cpu_should_JumpToArg1OnJlzIfLessThanZero(void) {
    prgm.length = 3;
    prgm.instrs[0] = INSTR0(OP_NOP);
    prgm.instrs[1] = INSTR1(OP_JLZ, 1);
    prgm.instrs[2] = INSTR0(OP_NOP);
    state.pc = 1;
    state.acc = -5;
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(1, state.pc);
}

void test_Cpu_should_NotJumpOnJlzIfGreaterThanOrEqualToZero(void) {
    prgm.length = 3;
    prgm.instrs[0] = INSTR0(OP_NOP);
    prgm.instrs[1] = INSTR1(OP_JLZ, 0);
    prgm.instrs[2] = INSTR0(OP_NOP);
    state.pc = 1;
    state.acc = 0;
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(2, state.pc);
}

void test_Cpu_should_Swap(void) {
    prgm.length = 1;
    prgm.instrs[0] = INSTR0(OP_SWP);
    state.acc = 100;
    state.bak = 200;
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(200, state.acc);
    TEST_ASSERT_EQUAL_INT(100, state.bak);
}

void test_Cpu_should_IncPcOnSwap(void) {
    prgm.length = 2;
    prgm.instrs[0] = INSTR0(OP_SWP);
    prgm.instrs[1] = INSTR0(OP_NOP);
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(1, state.pc);
}

void test_Cpu_should_Save(void) {
    prgm.length = 1;
    prgm.instrs[0] = INSTR0(OP_SAV);
    state.acc = 123;
    state.bak = 456;
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(123, state.bak);
}
