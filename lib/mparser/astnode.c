#include <stdio.h>
#include <stdlib.h>
#include "astnode.h"


void die(const char *msg) {
    fputs(msg, stderr);
    exit(EXIT_FAILURE);
}

pAstPayload createAstPayload()
{
    pAstPayload newPayload = (pAstPayload) malloc(sizeof(AstPayload));
    //newPayload->

    return newPayload;
}

pAstNode createAstNode(pAstPayload payload)
{
    pAstNode newAstNode = (pAstNode) malloc(sizeof(AstNode));
    newAstNode->numOfChildren = 0;
    newAstNode->children = NULL;
    newAstNode->payload = payload;

    return newAstNode;
}

void addAstChild(pAstNode tree, pAstNode child)
{
    size_t size = sizeof(pAstNode) * ++(tree->numOfChildren);
    pAstNode * newChildren = (pAstNode *) realloc(tree->children, size);
    if (!newChildren) {
        die("addAstChild: failed to reallocate memory for more children");
    }
    tree->children = newChildren;
    tree->children[tree->numOfChildren - 1] = child;
}

pAstNode getAstChild(pAstNode tree, unsigned int index)
{
    if (index >= tree->numOfChildren) {
        die("getAstChild: index is out of range.\n");
    }
    return (pAstNode) tree->children[index];
}

void freePayload(pAstPayload payload)
{
    //
}

void freeAst(pAstNode tree)
{
    if (!tree) {
        die("freeAst: argument can not point to NULL.");
    }
    if (tree->payload != NULL) {
        freePayload(tree->payload);
    }
    for (unsigned int index = 0; index < tree->numOfChildren; index++) {
        freeAst(getAstChild(tree, index));
    }
    free(tree);
}

