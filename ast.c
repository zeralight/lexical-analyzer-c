#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "ast.h"
#include "utils.h"

/*
   Creates a new AST from a given type filename and linenum. You should not
   assume that filename will remain a legal pointer after this function
   terminates.
*/
AST* MakeAST(enum NodeType type, char* filename, int linenum) {
  AST* ast = malloc(sizeof *ast);
  if (!ast) allocation_failed();
  ast->linenum = linenum;
  ast->type = type;
  ast->filename = malloc(strlen(filename)+1);
  strcpy(ast->filename, filename);
  ast->data.identifier = NULL;
  ast->data.integer = 0;
  ast->size = ast->capacity = 0;
  ast->children = NULL;

  /* printf("Allocated %p %d\n", ast, linenum); */
  return ast;
}

/*
   Takes in a given AST and mallocs a new AST with the exact same details as the
   original. Again you should assume the original may be freed at any point
   after this function terminates.
*/
AST* CopyAST(AST* original) {
  AST* ast = MakeAST(original->type, original->filename, original->linenum);
  ast->size = original->size;
  ast->capacity = original->capacity;
  ast->children = (AST**)realloc(ast->children, sizeof(AST*) * ast->capacity);
  if (ast->children == NULL) {
    allocation_failed();
  }
  for (int i = 0; i < ast->size; i++) {
    ast->children[i] = CopyAST(original->children[i]);
  }

  /* Start Unique to Copy */
  if (ast->type == NODETYPE_ID) {
    ast->data.identifier =
        (char*)malloc(sizeof(char) * (strlen(original->data.identifier) + 1));
    if (ast->data.identifier == NULL) {
      allocation_failed();
    }
    strcpy(ast->data.identifier, original->data.identifier);
  } else if (ast->type == NODETYPE_CONSTANT_STRING) {
    ast->data.string =
        (char*)malloc(sizeof(char) * (strlen(original->data.string) + 1));
    if (ast->data.string == NULL) {
      allocation_failed();
    }
    strcpy(ast->data.string, original->data.string);
  }
  /* End of Unique to Copy */
  return ast;
}

/*
   Takes in an two ASTs and concatenates the two by making node a child
   of tree.
*/
void AppendAST(AST* tree, AST* node) {
  /* printf("Appending %p %p\n", tree, node); */
  assert (tree != NULL);
  assert (node != NULL);

  if (tree->size == tree->capacity) {
    tree->capacity = 2*tree->capacity+1;
    tree->children = realloc(tree->children, sizeof *(tree->children) * tree->capacity);
  }
  int i = tree->size;
  tree->size++;
  tree->children[i] = node;
  // tree->children[tree->size++] = node;
}

/*
   Frees the memory allocated by a single AST node.
*/
void FreeNode(AST* ast) {
  /* printf("Free Node %p\n", ast); */
  free(ast->filename);
  if (ast->children) free(ast->children);
  if (ast->type == NODETYPE_CONSTANT_STRING) free(ast->data.string);
  else if (ast->type == NODETYPE_ID) free(ast->data.identifier);

  free(ast);
}

/*
   Frees all the memory allocated by an AST.
*/
void FreeAST(AST* ast) {
  if (ast) {
    if (ast->children) {
      for (size_t i = 0; i < ast->size; ++i)
        FreeAST(ast->children[i]);
    }
    FreeNode(ast);
  }
}
