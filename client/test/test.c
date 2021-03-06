#include "acutest.h"

#include "client_args.h"

char** deconstify_argv(int argc, const char **argv_const){
    char **argv = calloc(argc, sizeof(char*));
    for(int i = 0; i < argc; ++i){
        argv[i] = calloc(strlen(argv_const[i])+1, sizeof(char));
        strcpy(argv[i], argv_const[i]);
    }
    return argv;
}

void free_argv(int argc, char **argv){
    for(int i = 0; i < argc; ++i){
        free(argv[i]);
        argv[i] = NULL;
    }
    free(argv);
}

void test_client_args_ctor1(void) {
    int argc = 1;
    const char *argv_const[2] = {"prog", NULL};
    char **argv = deconstify_argv(argc, argv_const);

    client_args_t args;
    TEST_CHECK(client_args_ctor(&args, argc, argv) == EXIT_FAILURE);
    TEST_CHECK(client_args_dtor(&args) == EXIT_SUCCESS);

    free_argv(argc, argv);
}

void test_client_args_ctor2(void) {
    int argc = 2;
    const char *argv_const[3] = {"prog", "-t=2", NULL};
    char **argv = deconstify_argv(argc, argv_const);

    client_args_t args;
    TEST_CHECK(client_args_ctor(&args, argc, argv) == EXIT_FAILURE);
    TEST_CHECK(client_args_dtor(&args) == EXIT_SUCCESS);

    free_argv(argc, argv);
}

void test_client_args_ctor3(void) {
    int argc = 2;
    const char *argv_const[3] = {"prog", "fifopath", NULL};
    char **argv = deconstify_argv(argc, argv_const);

    client_args_t args;
    TEST_CHECK(client_args_ctor(&args, argc, argv) == EXIT_FAILURE);
    TEST_CHECK(client_args_dtor(&args) == EXIT_SUCCESS);

    free_argv(argc, argv);
}

void test_client_args_ctor4(void) {
    int argc = 3;
    const char *argv_const[4] = {"prog", "-t", "2", NULL};
    char **argv = deconstify_argv(argc, argv_const);

    client_args_t args;
    TEST_CHECK(client_args_ctor(&args, argc, argv) == EXIT_FAILURE);
    TEST_CHECK(client_args_dtor(&args) == EXIT_SUCCESS);

    free_argv(argc, argv);
}

void test_client_args_ctor5(void) {
    int argc = 3;
    const char *argv_const[4] = {"prog", "-t=2", "fifopath", NULL};
    char **argv = deconstify_argv(argc, argv_const);

    client_args_t args;
    TEST_CHECK(client_args_ctor(&args, argc, argv) == EXIT_FAILURE);
    TEST_CHECK(client_args_dtor(&args) == EXIT_SUCCESS);

    free_argv(argc, argv);
}

void test_client_args_ctor6(void) {
    int argc = 3;
    const char *argv_const[4] = {"prog", "-t2", "fifopath", NULL};
    char **argv = deconstify_argv(argc, argv_const);

    client_args_t args;
    TEST_CHECK(client_args_ctor(&args, argc, argv) == EXIT_SUCCESS);
    TEST_CHECK(args.nsecs == 2);
    TEST_CHECK(strcmp(args.fifoname, "/tmp/fifopath") == 0);
    TEST_CHECK(client_args_dtor(&args) == EXIT_SUCCESS);

    free_argv(argc, argv);
}

void test_client_args_ctor7(void) {
    int argc = 4;
    const char *argv_const[5] = {"prog", "-t", "2", "fifopath", NULL};
    char **argv = deconstify_argv(argc, argv_const);

    client_args_t args;
    TEST_CHECK(client_args_ctor(&args, argc, argv) == EXIT_SUCCESS);
    TEST_CHECK(args.nsecs == 2);
    TEST_CHECK(strcmp(args.fifoname, "/tmp/fifopath") == 0);
    TEST_CHECK(client_args_dtor(&args) == EXIT_SUCCESS);

    free_argv(argc, argv);
}

TEST_LIST = {
    {"client_args_ctor1"      , test_client_args_ctor1      },
    {"client_args_ctor2"      , test_client_args_ctor2      },
    {"client_args_ctor3"      , test_client_args_ctor3      },
    {"client_args_ctor4"      , test_client_args_ctor4      },
    {"client_args_ctor5"      , test_client_args_ctor5      },
    {"client_args_ctor6"      , test_client_args_ctor6      },
    {"client_args_ctor7"      , test_client_args_ctor7      },
    {NULL, NULL}
};
