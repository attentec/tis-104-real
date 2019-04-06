#include "cpu.h"
#include "code.h"
#include "unity.h"

void compare_code(struct code_t expected, struct code_t actual) {
    TEST_ASSERT_EQUAL_INT8_MESSAGE(expected.prgm.length,
                                   actual.prgm.length,
                                   "Mismatch in program length.");
    for (int i = 0; i < expected.prgm.length; ++i) {
        char buf[256];
        snprintf(buf, sizeof(buf), "Mismatch in instruction %d op.", i);
        TEST_ASSERT_EQUAL_INT8_MESSAGE(expected.prgm.instrs[i].op,
                                       actual.prgm.instrs[i].op,
                                       buf);
        snprintf(buf, sizeof(buf), "Mismatch in instruction %d arg1.", i);
        TEST_ASSERT_EQUAL_INT8_MESSAGE(expected.prgm.instrs[i].arg1,
                                       actual.prgm.instrs[i].arg1,
                                       buf);
        snprintf(buf, sizeof(buf), "Mismatch in instruction %d arg2.", i);
        TEST_ASSERT_EQUAL_INT8_MESSAGE(expected.prgm.instrs[i].arg2,
                                       actual.prgm.instrs[i].arg2,
                                       buf);
    }
    TEST_ASSERT_EQUAL_PTR_MESSAGE(expected.lines,
                                  actual.lines,
                                  "Mismatch in code lines.");
    TEST_ASSERT_EQUAL_INT8_ARRAY_MESSAGE(expected.addr_to_line,
                                         actual.addr_to_line,
                                         CPU_MAX_PRGM_LENGTH,
                                         "Mismatch in address to line.");
    TEST_ASSERT_EQUAL_INT8_ARRAY_MESSAGE(expected.addr_to_line,
                                         actual.addr_to_line,
                                         CPU_MAX_PRGM_LENGTH,
                                         "Mismatch in address to line.");
    TEST_ASSERT_EQUAL_INT8_ARRAY_MESSAGE(expected.errors,
                                         actual.errors,
                                         CPU_MAX_PRGM_LENGTH,
                                         "Mismatch in errors.");
}

void test_Code_should_parse_empty_program(void) {
    struct code_t code;
    const char *lines[CPU_MAX_PRGM_LENGTH] = {0};
    code_init(&code, lines);
    struct code_t expected = (struct code_t){
        .prgm = {
            .length = 0,
        },
        .lines = lines,
    };
    compare_code(expected, code);
}

void test_Code_should_parse_single_instruction(void) {
    struct code_t code;
    const char *lines[CPU_MAX_PRGM_LENGTH] = {"MOV 1 ACC"};
    code_init(&code, lines);
    struct code_t expected = (struct code_t) {
        .prgm = {
            .length = 1,
            .instrs = {
                INSTR2(OP_MOV, 1, ARG_ACC),
            },
        },
        .lines = lines,
        .addr_to_line = {0},
    };
    compare_code(expected, code);
}

void test_Code_should_parse_example_program(void) {
    struct code_t code;
    const char *lines[CPU_MAX_PRGM_LENGTH] = {
        "  MOV 10, ACC",
        "LOOP:",
        "  SUB 1",
        "  JNZ LOOP",
        "  SWP",
        "  ADD 1",
        "  SWP"
    };
    code_init(&code, lines);
    struct code_t expected = (struct code_t) {
        .prgm = {
            .length = 6,
            .instrs = {
                INSTR2(OP_MOV, 10, ARG_ACC),
                INSTR1(OP_SUB, 1),
                INSTR1(OP_JNZ, 1),
                INSTR0(OP_SWP),
                INSTR1(OP_ADD, 1),
                INSTR0(OP_SWP),
            },
        },
        .lines = lines,
        .addr_to_line = {
            0, 2, 3, 4, 5, 6,
        }
    };
    compare_code(expected, code);
}

void test_Code_should_handle_label_with_instr(void) {
    struct code_t code;
    const char *lines[CPU_MAX_PRGM_LENGTH] = {
        "LOOP:ADD 1",
        "  JMP LOOP",
    };
    code_init(&code, lines);
    struct code_t expected = (struct code_t) {
        .prgm = {
            .length = 2,
            .instrs = {
                INSTR1(OP_ADD, 1),
                INSTR1(OP_JMP, 0),
            },
        },
        .lines = lines,
        .addr_to_line = {
            0, 1,
        }
    };
    compare_code(expected, code);
}

void test_Code_should_mark_error_on_unknown_instruction(void) {
    struct code_t code;
    const char *lines[CPU_MAX_PRGM_LENGTH] = {
        "FOO 1, 2",
    };
    code_init(&code, lines);
    struct code_t expected = (struct code_t) {
        .prgm = {
            .length = 0,
        },
        .lines = lines,
        .errors = {true},
    };
    compare_code(expected, code);
}

void test_Code_should_mark_error_on_missing_label(void) {
    struct code_t code;
    const char *lines[CPU_MAX_PRGM_LENGTH] = {
        "JMP FOO",
    };
    code_init(&code, lines);
    struct code_t expected = (struct code_t) {
        .prgm = {
            .length = 1,
        },
        .lines = lines,
        .errors = {true},
    };
    compare_code(expected, code);
}

void test_Code_should_mark_error_on_junk_after_instr(void) {
    struct code_t code;
    const char *lines[CPU_MAX_PRGM_LENGTH] = {
        "ADD 1 BAR",
    };
    code_init(&code, lines);
    struct code_t expected = (struct code_t) {
        .prgm = {
            .length = 1,
            .instrs = {
                INSTR1(OP_ADD, 1),
            },
        },
        .lines = lines,
        .errors = {true},
    };
    compare_code(expected, code);
}
