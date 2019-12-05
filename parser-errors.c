#include <stdio.h>
#include <string.h>
#include "parser-errors.h"

// Helper Functions
int CheckImproperStatements(AST* ast, int is_for, int* incorrect_returns);

/*
  Tracks if when we have run out of tokens. Used to prevent redundant error
  messages.
*/
int eof_reached = 0;

/*
  Used to track the name of the current file. This is show if an unexpect eof
  occurs we can determine in what file it occurred.
*/
char* curr_filename;

/*
  Function that takes in an AST and determines if any error has occurred.
  Returns 0 if no errors are in the AST and otherwise returns a nonzero value.
*/
int CheckErrors(AST* ast) {
  int incorrect_returns = 0;
  return CheckImproperStatements(ast, 0, &incorrect_returns);
}

/*
  Function that takes in an AST and determines if any errors
  occurred in its creation. In addition to the AST, the function also contains
  parameters 'is_for', which is true if the current node is a descendent of a
  For Loop node, and 'incorrect_returns' which is pointer to an integer. This
  value is only useful for descendent of a func decl node that contains a
  body (a function definition). It should point to 0 if the function always
  successfully returns and should be nonzero otherwise.

  This function shouldreturn 0 if there are no errors and otherwise a nonzero
  value if there are ANY ERRORS. (Note that 'incorrect_returns' must be used to
  determine the return value of the function and simply setting it to the
  correct value will not be sufficient).

  There are 4 types of errors:

  1. An Error Node is present in the AST. This means an error occurred in
  parsing. There is no need to print any output message as one as already
  occurred during parsing.

  2. A Break statement outside a for loop. If a Break statement is encountered
  while
  'is_for' is false then an error has occurred. In a perfect world, our grammar
  would've
  been strict enough to only allow break statements to occur within for loops,
  but this would've complicated the grammar.
  You should display an output message to STDERR indicating the problem. This is
  not
  quired for grading but will be essential for debugging any tests you write.


  3. A Continue statement outside a for loop. As with Break statements, we
  could've
  prevented this from occuring with a more complex grammar.
  If a Continue statement is encountered while is_for is false then
  an error has occurred. You should display an output message to STDERR
  indicating the problem. This is not required for grading but will be
  essential for debugging any tests you write.

  4. A function does not always result in a return. Our language requires that
  all functions always return some value. You do not need to worry about type
  checking but all possible execution paths must always result in a return.
  Note that we do not analyze under what circumstances a function is taken and
  so we must always return without knowing what any expression evaluates to.
  If any function does not always return then an error has occurred. You should
  display an output message to STDERR indicating the problem. This is not
  required for grading but will be essential for debugging any tests you write.

  Hint 1: Checking if a function returns is complicated by if statements. For a
  function to always return as a result of an if statement both an if and an
  else must always return.

  Hint 2: Is it possible to know if you will enter a for loop without doing
  any evaluation?

*/
int CheckImproperStatements(AST* ast, int is_for, int* incorrect_returns) {
  // Set these nodes as incorrectly returning and try to validate them later. All the others nodes do not need checking
  *incorrect_returns  = (ast->type == NODETYPE_FUNC_DECL && ast->size == 4) // case 1: function definition
                      || ast->type == NODETYPE_CONTROL_IF_ELSE            //  case 2: If_ELse
                      || ast->type == NODETYPE_BLOCK;                    //   case 3: Block  


  int ret = 0;
  do
  {
    if (ast->type == NODETYPE_ERR || (!is_for && (ast->type == NODETYPE_BREAK || ast->type == NODETYPE_CONTINUE)))
    {
      ret = 1;
      break;
    }

    int if_returns = 0;
    for (size_t i = 0; i < ast->size; ++i)
    {
      int child_inc_ret;
      AST* child = ast->children[i];
      ret |= CheckImproperStatements(child, is_for || (ast->type == NODETYPE_CONTROL_FOR), &child_inc_ret);
      if (ret) break;

      // try to validate the returns flag 
      if (*incorrect_returns && !child_inc_ret) // check only if the parent node is invalidated and the child is
      {
        if (ast->type == NODETYPE_FUNC_DECL && child->type == NODETYPE_BLOCK) // func always return <=> internal block always returns
          *incorrect_returns = 0;
        else if (ast->type == NODETYPE_BLOCK && child->type == NODETYPE_RETURN) // Block always return if a direct child is a return
          *incorrect_returns = 0;
        else if (ast->type == NODETYPE_BLOCK && child->type == NODETYPE_CONTROL_IF_ELSE) // Block always return if IF_ELSE returns
          *incorrect_returns = 0;
        // Block doesn't return in any other case
        else if (ast->type == NODETYPE_CONTROL_IF_ELSE && i == 2 && if_returns) // if_else returns <=> IF returns && ELSE returns 
          *incorrect_returns = 0;
        else if (ast->type == NODETYPE_CONTROL_IF_ELSE && i == 1) // mark IF as a returning 
          if_returns = 1;
      }
      if (ast->type == NODETYPE_CONTROL_IF_ELSE && i+1 == ast->size) // reset the IF flag above
        if_returns = 0;
    }

  } while (0);

  // invalidate the return value with the incorrect_returns flag only on the function level
  ret |= *incorrect_returns && ast->type == NODETYPE_FUNC_DECL && ast->size == 4;
  
  /* char name[128];
  if (ast->type == NODETYPE_FUNC_DECL) sprintf(name, "NODETYPE_FUNC_DECL: %s", ast->children[1]->data.identifier);
  else if (ast->type == NODETYPE_BLOCK) strcpy(name, "NODETYPE_BLOCK");
  else if (ast->type == NODETYPE_CONTROL_IF_ELSE) strcpy(name, "NODETYPE_IF_ELSE");
  else if (ast->type == NODETYPE_GLOBAL) strcpy(name, "NODETYPE_GLOBAL");
  else strcpy(name, "NODETYPE_SOMETHING");
  fprintf(stderr, "AST type: %s, incorrect_returns = %d | ret = %d\n", name, *incorrect_returns, ret); */

  return ret;
}

/*
  Generates the error message for when there are not enough tokens to continue
  parsing rules. It switches eof_reached to 1 to prevent redundant print
  statements.
*/
void generate_eof_error() {
  if (!eof_reached) {
    eof_reached = 1;
    fprintf(stderr, "Error: Unexpected end of file reached in %s.\n",
            curr_filename);
  }
}

/*
  Function to assist the output of error messages when a token is needed to
  complete a rule in the grammar but is not found.
*/
void generate_error_message(char* missing, char* filename, int linenum) {
  fprintf(stderr, "Error: Expected %s on line %d of %s, but none found.\n",
          missing, linenum, filename);
}
