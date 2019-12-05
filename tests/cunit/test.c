#define _DEFAULT_SOURCE

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../../string-helpers.h"
#include <CUnit/Basic.h>
#include <CUnit/TestDB.h>
#include "test.h"

/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int init_suite1(void) {
  return 0;
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite1(void) {
  return 0;
}

/* The test initialization function.
 * Opens the temporary file used by the test.
 */
int init_test1(void) {
  return 0;
}

/* The test cleanup function.
 * Closes the temporary file used by the test in particular.
 */
int clean_test1(void) {
  return 0;
}

/* Simple test of is_digit().
 */
void testISDIGIT_Integers(void) {
  CU_ASSERT_FALSE(is_digit(0));
  CU_ASSERT_FALSE(is_digit(9));
  CU_ASSERT_FALSE(is_digit(-1));
}

void testISDIGIT_Digits(void) {
  CU_ASSERT_TRUE(is_digit('1'));
  CU_ASSERT_TRUE(is_digit('0'));
  CU_ASSERT_TRUE(is_digit('9'));
}

void testISDIGIT_EscChars(void) {
  CU_ASSERT_FALSE(is_digit('\0'));
  CU_ASSERT_FALSE(is_digit('\"'));
  CU_ASSERT_FALSE(is_digit('\n'));
}

// make 3 tests per fuc:
/* Test1 of IS_ALPHA()
*/
void testISALPHA_LowerCase(void) {
  CU_ASSERT_TRUE(is_alpha('a'));
  CU_ASSERT_TRUE(is_alpha('z'));
  CU_ASSERT_TRUE(is_alpha('f'));
  }



/* Test2 of IS_ALPHA()
*/
void testISALPHA_UpperCase(void) {
  CU_ASSERT_TRUE(is_alpha('A'));
  CU_ASSERT_TRUE(is_alpha('Z'));
  CU_ASSERT_TRUE(is_alpha('H'));
  }


/* Test3 of IS_ALPHA()
*/
void testISALPHA_DigitOrSpace(void) {
  CU_ASSERT_FALSE(is_alpha('1'));
  CU_ASSERT_FALSE(is_alpha('9'));
  CU_ASSERT_FALSE(is_alpha(' '));
  CU_ASSERT_FALSE(is_alpha('\t'));
  }


/* Test1 of IS_SPACE()
*/
void testISSPACE_Real(void) {
  CU_ASSERT_TRUE(is_space(' '));
  CU_ASSERT_TRUE(is_space('\t'));
  CU_ASSERT_TRUE(is_space('\n'));
  CU_ASSERT_FALSE(is_space('\''));
  }


/* Test2 of IS_SPACE()
*/
void testISSPACE_ALPHA(void) {
  CU_ASSERT_TRUE(is_space(' '));
  CU_ASSERT_FALSE(is_space('a'));
  CU_ASSERT_FALSE(is_space('X'));
  }


/* Test3 of IS_SPACE()
*/
void testISSPACE_Digit(void) {
  CU_ASSERT_TRUE(is_space(' ') && !is_alpha(' ') && !is_digit(' '));
  CU_ASSERT_FALSE(is_space('1'));
  CU_ASSERT_FALSE(is_space('3'));
  }

/* Test1 of IS_IDENTIFIER_COMPONENT()
*/
void testISIDENTIFIERCOMPONENT_Real(void) {
  CU_ASSERT_TRUE(is_identifier_component('B'));
  CU_ASSERT_TRUE(is_identifier_component('j'));
  CU_ASSERT_TRUE(is_identifier_component('7'));
  CU_ASSERT_TRUE(is_identifier_component('0'));
  CU_ASSERT_TRUE(is_identifier_component('_'));
  }



/* Test2 of IS_IDENTIFIER_COMPONENT()
*/
void testISIDENTIFIERCOMPONENT_TabORMultipleSpaces(void) {
  CU_ASSERT_FALSE(is_identifier_component(' '));
  CU_ASSERT_FALSE(is_identifier_component('\t'));
  }


/* Test3 of IS_IDENTIFIER_COMPONENT()
  */
void testISIDENTIFIERCOMPONENT_Not(void) {
  CU_ASSERT_FALSE(is_identifier_component('*'));
  CU_ASSERT_FALSE(is_identifier_component('\n'));
  }


/* Test1 of IS_VALID_IDENTIFIER()
  */
void testISVALIDIDENTIFIER_Nothing(void) {
  CU_ASSERT_FALSE(is_valid_identifier(""));
  }


/* Test2 of IS_VALID_IDENTIFIER()
  */
void testISVALIDIDENTIFIER_String(void) {
  CU_ASSERT_TRUE(is_valid_identifier("ngfgfd"));
  CU_ASSERT_TRUE(is_valid_identifier("g"));
  }

void test_replace_escape_character_correct(void) {
  CU_ASSERT_EQUAL(replace_escape_in_character("\\n"), '\n');
  CU_ASSERT_EQUAL(replace_escape_in_character("\\t"), '\t');
  CU_ASSERT_EQUAL(replace_escape_in_character("\\0"), '\0');
  CU_ASSERT_EQUAL(replace_escape_in_character("\\'"), '\'');
  CU_ASSERT_EQUAL(replace_escape_in_character("\\\\"), '\\');
}

void test_replace_escape_character_incorrect(void) {
  CU_ASSERT_EQUAL(replace_escape_in_character("a"), -1);
  CU_ASSERT_EQUAL(replace_escape_in_character("b"), -1);
  CU_ASSERT_EQUAL(replace_escape_in_character("\\1"), -1);
  CU_ASSERT_EQUAL(replace_escape_in_character("\\a"), -1);
  CU_ASSERT_EQUAL(replace_escape_in_character("\\ "), -1);
  CU_ASSERT_EQUAL(replace_escape_in_character("\\\""), -1);
}

void test_replace_escape_string_correct(void) {
  CU_ASSERT_EQUAL(replace_escape_in_string("\\n"), '\n');
  CU_ASSERT_EQUAL(replace_escape_in_string("\\t"), '\t');
  CU_ASSERT_EQUAL(replace_escape_in_string("\\0"), '\0');
  CU_ASSERT_EQUAL(replace_escape_in_string("\\\\"), '\\');
  CU_ASSERT_EQUAL(replace_escape_in_string("\\\""), '"');
}

void test_replace_escape_string_incorrect(void) {
  CU_ASSERT_EQUAL(replace_escape_in_string("a"), -1);
  CU_ASSERT_EQUAL(replace_escape_in_string("b"), -1);
  CU_ASSERT_EQUAL(replace_escape_in_string("\\1"), -1);
  CU_ASSERT_EQUAL(replace_escape_in_string("\\a"), -1);
  CU_ASSERT_EQUAL(replace_escape_in_string("\\ "), -1);
  CU_ASSERT_EQUAL(replace_escape_in_string("\\'"), -1);
}

/* Test3 of IS_VALID_IDENTIFIER()
  */
void testISVALIDIDENTIFIER_IllegalChar(void) {
  CU_ASSERT_FALSE(is_valid_identifier("@"));
  CU_ASSERT_FALSE(is_valid_identifier("()))"));
  CU_ASSERT_FALSE(is_valid_identifier("+-"));
  CU_ASSERT_FALSE(is_valid_identifier("="));
}


char input_filename[BUFSIZ];
const char* lex_tests_path = "../lexer/hw2/";
char output[10*1024*1024];
char expected_output[10*1024*1024];

void test_lex_file(void) {
  char input_path[BUFSIZ];
  char output_path[BUFSIZ];
  char cmd[BUFSIZ];

  strcpy(input_path, lex_tests_path);
  strcat(input_path, input_filename);

  strcpy(output_path, input_path);
  strcpy(output_path + strlen(output_path) - 4, "_expected_output");

  FILE *in = fopen(input_path, "r");
  CU_ASSERT_PTR_NOT_NULL_FATAL(in);
  fclose(in);

  FILE *out = fopen(output_path, "rb");

  CU_ASSERT_PTR_NOT_NULL_FATAL(out);
  fseek(out, 0, SEEK_END);
  size_t size = ftell(out);
  rewind(out);
  size_t p = fread(expected_output, 1, size, out);
  expected_output[p] = '\0';
  fclose(out);

  getcwd(cmd, sizeof cmd);
  strcat(cmd, "/../../61ccc -t ");
  strcat(cmd, input_path);
  FILE *fp = popen(cmd, "r");
  CU_ASSERT_PTR_NOT_NULL_FATAL(fp);
  p = fread(output, 1, sizeof output - 1, fp);
  output[p] = '\0';
  fclose(fp);

  CU_ASSERT_STRING_EQUAL(output, expected_output);
}

void test_lex(void) {
  strcpy(input_filename, "already_implemented.61c");
  test_lex_file();

  strcpy(input_filename, "comments.61c");
  test_lex_file();

  strcpy(input_filename, "chars.61c");
  test_lex_file();

  strcpy(input_filename, "int.61c");
  test_lex_file();

  strcpy(input_filename, "identifiers.61c");
  test_lex_file();

  strcpy(input_filename, "integration_test_1.61c");
  test_lex_file();

  strcpy(input_filename, "integration_test_2.61c");
  test_lex_file();
}

/* int init_lex_already_implemented(void) {
  strcpy(input_filename, "already_implemented.61c");
  return 0;
}

int init_lex_comments(void) {
  strcpy(input_filename, "comments.61c");
  return 0;
}

int init_lex_chars(void) {
  strcpy(input_filename, "chars.61c");
  return 0;
} */


/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main() {
  CU_TestInfo isdigit_tests[] = {
                                 {"Test actual digits", testISDIGIT_Digits},
                                 {"Test esc chars", testISDIGIT_EscChars},
                                 {"Test numbers", testISDIGIT_Integers},
                                 CU_TEST_INFO_NULL,
                                 };


     // make 1 suite per func
    CU_TestInfo isalpha_tests[] = {{"Test alpha lower case", testISALPHA_LowerCase},
                             {"Test Upper Case", testISALPHA_UpperCase},
                             {"Test Alpha on digit or Space", testISALPHA_DigitOrSpace}};

    CU_TestInfo isspace_tests[] = {{"Test Space on Digit", testISSPACE_Digit},
                                   {"Test Space on Char", testISSPACE_ALPHA},
                                   {"Test Space on Space", testISSPACE_Real}};

    CU_TestInfo isidentifiercomponent_tests[] = {{"Test Identifier on Not", testISIDENTIFIERCOMPONENT_Not},
                                                 {"Test Identifier on Mult Spaces", testISIDENTIFIERCOMPONENT_TabORMultipleSpaces},
                                                 {"Test Identif on Identif",testISIDENTIFIERCOMPONENT_Real}};

    CU_TestInfo isvalididentifier_tests[] = {{"Test String on 0", testISVALIDIDENTIFIER_Nothing},
                                             {"Test String on String", testISVALIDIDENTIFIER_String},
                                             {"Test String on Illegal char",testISVALIDIDENTIFIER_IllegalChar}};
    
    CU_TestInfo isescape_char_tests[] = {{"Tests on correct", test_replace_escape_character_correct},
                                             {"Test on incorrect", test_replace_escape_character_incorrect}};
    
    
    CU_TestInfo isescape_string_tests[] = {{"Tests on correct", test_replace_escape_string_correct},
                                             {"Tests on incorrect", test_replace_escape_string_incorrect}};

    CU_TestInfo lex_tests[] = {{"Tests Integration of Lex", test_lex}};

    CU_SuiteInfo suites[] = {
                          {"is_digit testing", init_suite1, clean_suite1, NULL, NULL, isdigit_tests},
                          {"is_alpha testing", init_suite1, clean_suite1, NULL, NULL, isalpha_tests},
                          {"is_space testing", init_suite1, clean_suite1, NULL, NULL, isspace_tests},
                          {"is_identif_compon testing", init_suite1, clean_suite1, NULL, NULL, isidentifiercomponent_tests},
                          {"is_valid_identif testing", init_suite1, clean_suite1, NULL, NULL, isvalididentifier_tests},
                          {"is_replace_escape_chars", init_suite1, clean_suite1, NULL, NULL, isescape_char_tests},
                          {"is_replace_escape_strings", init_suite1, clean_suite1, NULL, NULL, isescape_string_tests},
                          {"lex_integration testing", init_suite1, NULL, NULL, NULL, lex_tests},
                           CU_SUITE_INFO_NULL}; // append my suites in here




  /* initialize the CUnit test registry */
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  if (CU_register_suites(suites)) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
