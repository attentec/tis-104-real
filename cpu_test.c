#include "unity.h"
#include "cpu.h"

void test_Cpu_should_IncPcOnNop(void)
{
    struct cpu_t cpu;
    struct prgm_t prgm = {
        1,
        {{OP_NOP, ARG_NONE, ARG_NONE}},
    };
    struct state_t state = {
        0,
    };
    cpu_init(&cpu, &prgm, &state);
    cpu_step(&cpu);
    TEST_ASSERT_EQUAL_INT(state.pc, 1);
}
