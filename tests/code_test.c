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

void test_Code_should_parse_empty_program_lines(void) {
    struct code_t code;
    const char *lines[CPU_MAX_PRGM_LENGTH] = {"", "", "", ""};
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

void test_Code_should_mark_error_on_duplicate_label(void) {
    struct code_t code;
    const char *lines[CPU_MAX_PRGM_LENGTH] = {
        "LOOP:",
        "LOOP:",
    };
    code_init(&code, lines);
    struct code_t expected = (struct code_t) {
        .prgm = {
            .length = 0,
        },
        .lines = lines,
        .errors = {false, true},
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

void test_Code_should_accept_all_instructions(void) {
    struct code_t code;
    const char *lines[CPU_MAX_PRGM_LENGTH] = {
        "START:",
        "NOP",
        "MOV 0, LEFT",
        "SWP",
        "SAV",
        "ADD RIGHT",
        "SUB UP",
        "NEG",
        "JRO DOWN",
        "JMP START",
        "JEZ START",
        "JNZ START",
        "JGZ START",
        "JLZ START",
    };
    code_init(&code, lines);
    struct code_t expected = (struct code_t) {
        .prgm = {
            .length = 13,
            .instrs = {
                INSTR0(OP_NOP),
                INSTR2(OP_MOV, 0, ARG_LEFT),
                INSTR0(OP_SWP),
                INSTR0(OP_SAV),
                INSTR1(OP_ADD, ARG_RIGHT),
                INSTR1(OP_SUB, ARG_UP),
                INSTR0(OP_NEG),
                INSTR1(OP_JRO, ARG_DOWN),
                INSTR1(OP_JMP, 0),
                INSTR1(OP_JEZ, 0),
                INSTR1(OP_JNZ, 0),
                INSTR1(OP_JGZ, 0),
                INSTR1(OP_JLZ, 0),
            },
        },
        .lines = lines,
        .addr_to_line = {
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
        }
    };
    compare_code(expected, code);
}

void test_Code_should_accept_all_arg_keywords(void) {
    struct code_t code;
    const char *lines[CPU_MAX_PRGM_LENGTH] = {
        "MOV ACC, NIL",
        "MOV LEFT, RIGHT",
        "MOV UP, DOWN",
        "MOV ANY, LAST",
    };
    code_init(&code, lines);
    struct code_t expected = (struct code_t) {
        .prgm = {
            .length = 4,
            .instrs = {
                INSTR2(OP_MOV, ARG_ACC, ARG_NIL),
                INSTR2(OP_MOV, ARG_LEFT, ARG_RIGHT),
                INSTR2(OP_MOV, ARG_UP, ARG_DOWN),
                INSTR2(OP_MOV, ARG_ANY, ARG_LAST),
            },
        },
        .lines = lines,
        .addr_to_line = {
            0, 1, 2, 3,
        }
    };
    compare_code(expected, code);
}
