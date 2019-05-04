#include "code.h"
#include "panic.h"

#include <string.h>
#include <stdlib.h>

#define CODE_MAX_LINE_LENGTH (19)

static struct code_t compile(const char *lines[CPU_MAX_PRGM_LENGTH]);

void code_init(struct code_t *code, const char *lines[CPU_MAX_PRGM_LENGTH])
{
    *code = compile(lines);
}

enum parse_state_t {
    INITIAL,
    SRC_ARG,
    DST_ARG,
    LABEL_ARG,
};

enum type_t {
    INSTR,
    LABEL_INSTR,
    LABEL,
    CONTINUE,
    ERROR,
};

struct label_t {
    char label[CODE_MAX_LINE_LENGTH];
    int line;
};

struct label_instr_t {
    enum op_t op;
    char label[CODE_MAX_LINE_LENGTH];
    address_t addr;
};

struct parse_t {
    enum type_t type;
    int line;
    union {
        struct instr_t instr;
        struct label_instr_t label_instr;
        struct label_t label;
    };
};

static void copy_line(char *dst, const char *src) {
    strncpy(dst, src, CODE_MAX_LINE_LENGTH);
    dst[CODE_MAX_LINE_LENGTH - 1] = '\0';
}

static bool streq(const char *one, const char *two) {
    return strcmp(one, two) == 0;
}

static struct parse_t error(void) {
    struct parse_t parse = {
        .type = ERROR,
    };
    return parse;
}

static struct parse_t instr(struct instr_t in) {
    struct parse_t parse = {
        .type = INSTR,
        .instr = in,
    };
    return parse;
}

static struct parse_t label_instr(enum op_t op, const char *word) {
    struct parse_t parse = {
        .type = LABEL_INSTR,
        .label_instr = {
            .op = op
        },
    };
    copy_line(parse.label_instr.label, word);
    return parse;
}

static struct parse_t label(const char *word, const char *colon) {
    struct parse_t parse = {
        .type = LABEL,
    };
    copy_line(parse.label.label, word);
    int colon_offset = colon - word;
    parse.label.label[colon_offset] = '\0';
    return parse;
}

static struct parse_t cont(void) {
    struct parse_t parse = {
        .type = CONTINUE
    };
    return parse;
}

#define NUM_INSTRS (8)
static struct {
    const char *mnemonic;
    const enum op_t op;
    const uint8_t num_args;
} instrs[NUM_INSTRS] = {
    {"NOP", OP_NOP, 0},
    {"MOV", OP_MOV, 2},
    {"SWP", OP_SWP, 0},
    {"SAV", OP_SAV, 0},
    {"ADD", OP_ADD, 1},
    {"SUB", OP_SUB, 1},
    {"NEG", OP_NEG, 0},
    {"JRO", OP_JRO, 1},
};

#define NUM_LABEL_INSTRS (5)
static struct {
    const char *mnemonic;
    const enum op_t op;
} label_instrs[NUM_LABEL_INSTRS] = {
    {"JMP", OP_JMP},
    {"JEZ", OP_JEZ},
    {"JNZ", OP_JNZ},
    {"JGZ", OP_JGZ},
    {"JLZ", OP_JLZ},
};

#define NUM_ARGS (8)
static struct {
    const char *mnemonic;
    const enum arg_t arg;
} args[NUM_ARGS] = {
    {"ACC",   ARG_ACC},
    {"NIL",   ARG_NIL},
    {"LEFT",  ARG_LEFT},
    {"RIGHT", ARG_RIGHT},
    {"UP",    ARG_UP},
    {"DOWN",  ARG_DOWN},
    {"ANY",   ARG_ANY},
    {"LAST",  ARG_LAST},
};

static struct parse_t parse(const char *word)
{
    static enum parse_state_t state = INITIAL;
    static enum op_t op;
    static enum arg_t src_arg;
    static int num_args;
    switch (state) {
    case INITIAL:
        if (strchr(word, ':')) {
            const char *colon = strchr(word, ':');
            if (colon[1] == '\0') {
                return label(word, colon);
            } else {
                return error();
            }
        }
        for (int i = 0; i < NUM_INSTRS; ++i) {
            if (streq(word, instrs[i].mnemonic)) {
                if (instrs[i].num_args == 0) {
                    return instr(INSTR0(instrs[i].op));
                } else {
                    op = instrs[i].op;
                    num_args = instrs[i].num_args;
                    state = SRC_ARG;
                    return cont();
                }
            }
        }
        for (int i = 0; i < NUM_LABEL_INSTRS; ++i) {
            if (streq(word, label_instrs[i].mnemonic)) {
                op = label_instrs[i].op;
                state = LABEL_ARG;
                return cont();
            }
        }
        state = INITIAL;
        return error();
        break;
    case SRC_ARG:
        for (int i = 0; i < NUM_ARGS; ++i) {
            if (streq(word, args[i].mnemonic)) {
                if (num_args == 1) {
                    state = INITIAL;
                    return instr(INSTR1(op, args[i].arg));
                } else {
                    src_arg = args[i].arg;
                    state = DST_ARG;
                    return cont();
                }
            }
        }
        char *endptr = 0;
        int value = strtol(word, &endptr, 10);
        if (*endptr == '\0' && value >= -999 && value <= 999) {
            if (num_args == 1) {
                state = INITIAL;
                return instr(INSTR1(op, value));
            } else {
                src_arg = value;
                state = DST_ARG;
                return cont();
            }
        }
        state = INITIAL;
        return error();
        break;
    case DST_ARG:
        for (int i = 0; i < NUM_ARGS; ++i) {
            if (streq(word, args[i].mnemonic)) {
                state = INITIAL;
                return instr(INSTR2(op, src_arg, args[i].arg));
            }
        }
        state = INITIAL;
        return error();
        break;
    case LABEL_ARG:
        state = INITIAL;
        return label_instr(op, word);
        break;
    }
    return error();
}

struct compile_state_t {
    struct code_t code;
    struct label_t labels[CPU_MAX_PRGM_LENGTH];
    struct label_instr_t label_instrs[CPU_MAX_PRGM_LENGTH];
    address_t l_count;
    address_t li_count;
};

static bool handle_word(struct compile_state_t *state, const char *word, int line) {
    struct parse_t result = parse(word);
    if (result.type == CONTINUE) {
        return false;
    }
    else if (result.type == INSTR) {
        // finished parsing instruction
        state->code.prgm.instrs[state->code.prgm.length] = result.instr;
        state->code.addr_to_line[state->code.prgm.length] = line;
        state->code.prgm.length += 1;
        return true;
    }
    else if (result.type == LABEL_INSTR) {
        // finished parsing instruction with label arg
        result.label_instr.addr = state->code.prgm.length;
        state->label_instrs[state->li_count] = result.label_instr;
        state->li_count += 1;
        state->code.addr_to_line[state->code.prgm.length] = line;
        state->code.prgm.length += 1;
        return true;
    }
    else if (result.type == LABEL) {
        // found label definition
        result.label.line = line;
        state->labels[state->l_count] = result.label;
        state->l_count += 1;
        // continue parsing things on this line
        return false;
    }
    else if (result.type == ERROR) {
        // this line is bad, mark it and skip to next
        state->code.errors[line] = true;
        return true;
    }
    else {
        panic();
    }
}

static struct code_t compile(const char *lines[CPU_MAX_PRGM_LENGTH])
{
    struct compile_state_t state;
    memset(&state, 0, sizeof(state));
    state.code.lines = lines;
    state.l_count = 0;
    state.li_count = 0;

    for (int i = 0; i < CPU_MAX_PRGM_LENGTH; ++i) {
        if (!lines[i]) {
            break;
        }
        char line[CODE_MAX_LINE_LENGTH];
        copy_line(line, lines[i]);
        char *pch = strtok(line, " ,");
        while (pch) {
            if (strchr(pch, ':')) {
                char *colon = strchr(pch, ':');
                if (colon[1] != '\0') {
                    // there is another word after the colon,
                    // handle the one before first.
                    char tmp = colon[1];
                    colon[1] = '\0';
                    if (handle_word(&state, pch, i)) {
                        break;
                    }
                    colon[1] = tmp;
                    pch = &colon[1];
                }
            }
            if (handle_word(&state, pch, i)) {
                break;
            }
            pch = strtok(0, " ,");
        }
        pch = strtok(0, " ,");
        if (pch) {
            if (*pch == '#') {
                // TODO: Comment
            } else {
                state.code.errors[i] = true;
            }
        }
    }
    for (int i = 0; i < state.l_count - 1; ++i) {
        for (int j = i + 1; j < state.l_count; ++j) {
            if (streq(state.labels[i].label, state.labels[j].label)) {
                state.code.errors[state.labels[j].line] = true;
            }
        }
    }
    for (int i = 0; i < state.li_count; ++i) {
        struct label_instr_t instr = state.label_instrs[i];
        bool found = false;
        for (int j = 0; j < state.l_count; ++j) {
            struct label_t label = state.labels[j];
            if (streq(instr.label, label.label)) {
                for (address_t addr = 0; addr < state.code.prgm.length; ++addr) {
                    if (state.code.addr_to_line[addr] >= label.line) {
                        // found instruction just after label
                        found = true;
                        state.code.prgm.instrs[instr.addr] = INSTR1(instr.op, addr);
                        break;
                    }
                }
                if (!found) {
                    // there is no instruction after the target label,
                    // jump to start of code
                    found = true;
                    state.code.prgm.instrs[instr.addr] = INSTR1(instr.op, 0);
                }
                break;
            }
        }
        if (!found) {
            state.code.errors[state.code.addr_to_line[instr.addr]] = true;
        }
    }
    return state.code;
}
