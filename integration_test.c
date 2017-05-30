#include "unity.h"
#include "cpu.h"
#include "pipe_mock.h"
#include "grid.h"

#define INSTR0(op)             ((struct instr_t){(op), ARG_NONE, ARG_NONE})
#define INSTR1(op, arg1)       ((struct instr_t){(op), (arg1),   ARG_NONE})
#define INSTR2(op, arg1, arg2) ((struct instr_t){(op), (arg1),   (arg2)})

enum {
    CPU_UL = 0,
    CPU_UR = 1,
    CPU_LL = 4,
    CPU_LR = 5,
};

void setUp(void) {
    grid_init();
}

void test_Integration_should_FollowBasicCommunicationTiming(void) {
    grid_prgms[CPU_UL].length = 2;
    grid_prgms[CPU_UL].instrs[0] = INSTR2(OP_MOV, 1, ARG_RIGHT);
    grid_prgms[CPU_UL].instrs[1] = INSTR1(OP_ADD, ARG_RIGHT);
    grid_prgms[CPU_UR].length = 1;
    grid_prgms[CPU_UR].instrs[0] = INSTR2(OP_MOV, ARG_LEFT, ARG_LEFT);
    grid_prgms[CPU_LL].length = 0;
    grid_prgms[CPU_LR].length = 0;

    grid_states[CPU_UL].acc = 0;

    grid_step();
    grid_step();
    grid_step();

    TEST_ASSERT_EQUAL_INT(1, grid_states[CPU_UL].acc);
}

void test_Integration_should_ProcessAnyInCorrectOrder(void) {
    grid_prgms[CPU_UL].length = 1;
    grid_prgms[CPU_UL].instrs[0] = INSTR2(OP_MOV, ARG_ANY, ARG_ACC);
    grid_prgms[CPU_UR].length = 1;
    grid_prgms[CPU_UR].instrs[0] = INSTR2(OP_MOV, 1, ARG_ANY);
    grid_prgms[CPU_LL].length = 1;
    grid_prgms[CPU_LL].instrs[0] = INSTR2(OP_MOV, 2, ARG_ANY);
    grid_prgms[CPU_LR].length = 1;
    grid_prgms[CPU_LR].instrs[0] = INSTR2(OP_MOV, ARG_ANY, ARG_ACC);

    grid_step();
    grid_step();

    TEST_ASSERT_EQUAL_INT(1, grid_states[CPU_UL].acc);
    TEST_ASSERT_EQUAL_INT(2, grid_states[CPU_LR].acc);
}

void test_Integration_should_RevokeOutputOffers(void) {
    grid_prgms[CPU_UL].length = 3;
    grid_prgms[CPU_UL].instrs[0] = INSTR2(OP_MOV, 5, ARG_RIGHT);
    grid_prgms[CPU_UL].instrs[1] = INSTR2(OP_MOV, 7, ARG_LEFT);
    grid_prgms[CPU_UL].instrs[2] = INSTR1(OP_JRO, 0);
    grid_prgms[CPU_UR].length = 1;
    grid_prgms[CPU_UR].instrs[0] = INSTR2(OP_MOV, ARG_LEFT, ARG_ACC);
    grid_prgms[CPU_LL].length = 0;
    grid_prgms[CPU_LR].length = 0;

    grid_step();
    grid_step();
    grid_step();
    grid_step();

    TEST_ASSERT_EQUAL_INT(5, grid_states[CPU_UR].acc);
    TEST_ASSERT_EQUAL_INT(1, grid_states[CPU_UL].pc);
}

void test_Integration_should_RevokeOutputOffersToAny(void) {
    grid_prgms[CPU_UL].length = 3;
    grid_prgms[CPU_UL].instrs[0] = INSTR2(OP_MOV, 5, ARG_ANY);
    grid_prgms[CPU_UL].instrs[1] = INSTR2(OP_MOV, 7, ARG_LEFT);
    grid_prgms[CPU_UL].instrs[2] = INSTR1(OP_JRO, 0);
    grid_prgms[CPU_UR].length = 1;
    grid_prgms[CPU_UR].instrs[0] = INSTR2(OP_MOV, ARG_LEFT, ARG_ACC);
    grid_prgms[CPU_LL].length = 0;
    grid_prgms[CPU_LR].length = 0;

    grid_step();
    grid_step();
    grid_step();
    grid_step();

    TEST_ASSERT_EQUAL_INT(5, grid_states[CPU_UR].acc);
    TEST_ASSERT_EQUAL_INT(1, grid_states[CPU_UL].pc);
}

void test_Integration_should_ReadFromLast(void) {
    grid_prgms[CPU_UL].length = 3;
    grid_prgms[CPU_UL].instrs[0] = INSTR2(OP_MOV, ARG_ANY, ARG_NIL);
    grid_prgms[CPU_UL].instrs[1] = INSTR2(OP_MOV, ARG_LAST, ARG_ACC);
    grid_prgms[CPU_UL].instrs[2] = INSTR1(OP_JRO, 0);
    grid_prgms[CPU_UR].length = 3;
    grid_prgms[CPU_UR].instrs[0] = INSTR2(OP_MOV, 5, ARG_LEFT);
    grid_prgms[CPU_UR].instrs[1] = INSTR2(OP_MOV, 7, ARG_LEFT);
    grid_prgms[CPU_UR].instrs[2] = INSTR1(OP_JRO, 0);
    grid_prgms[CPU_LL].length = 0;
    grid_prgms[CPU_LR].length = 0;

    grid_step();
    grid_step();
    grid_step();
    grid_step();

    TEST_ASSERT_EQUAL_INT(2, grid_states[CPU_UL].pc);
    TEST_ASSERT_EQUAL_INT(7, grid_states[CPU_UL].acc);
}

void test_Integration_should_SetLastOnWriteToAny(void) {
    grid_prgms[CPU_UL].length = 2;
    grid_prgms[CPU_UL].instrs[0] = INSTR2(OP_MOV, 5, ARG_ANY);
    grid_prgms[CPU_UL].instrs[1] = INSTR1(OP_JRO, 0);
    grid_prgms[CPU_UR].length = 2;
    grid_prgms[CPU_UR].instrs[0] = INSTR2(OP_MOV, ARG_LEFT, ARG_ACC);
    grid_prgms[CPU_UR].instrs[1] = INSTR1(OP_JRO, 0);
    grid_prgms[CPU_LL].length = 0;
    grid_prgms[CPU_LR].length = 0;

    grid_step();
    grid_step();

    TEST_ASSERT_TRUE(grid_states[CPU_UL].has_last);
    TEST_ASSERT_EQUAL_INT(DIR_RIGHT, grid_states[CPU_UL].last);
}

void test_Integration_should_WriteToLast(void) {
    grid_prgms[CPU_UL].length = 3;
    grid_prgms[CPU_UL].instrs[0] = INSTR2(OP_MOV, 5, ARG_ANY);
    grid_prgms[CPU_UL].instrs[1] = INSTR2(OP_MOV, 7, ARG_LAST);
    grid_prgms[CPU_UL].instrs[2] = INSTR1(OP_JRO, 0);
    grid_prgms[CPU_UR].length = 3;
    grid_prgms[CPU_UR].instrs[0] = INSTR2(OP_MOV, ARG_LEFT, ARG_ACC);
    grid_prgms[CPU_UR].instrs[1] = INSTR2(OP_MOV, ARG_LEFT, ARG_ACC);
    grid_prgms[CPU_UR].instrs[2] = INSTR1(OP_JRO, 0);
    grid_prgms[CPU_LL].length = 0;
    grid_prgms[CPU_LR].length = 0;

    grid_step();
    grid_step();
    grid_step();
    grid_step();

    TEST_ASSERT_EQUAL_INT(2, grid_states[CPU_UL].pc);
    TEST_ASSERT_EQUAL_INT(7, grid_states[CPU_UR].acc);
}
