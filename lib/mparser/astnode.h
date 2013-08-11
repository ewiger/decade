#ifndef __ASTNODE_H_
#define __ASTNODE_H_
/*
 * Provides a simplified tree structure to host/repack ANTLR AST.
 *
 */

/* Payload is all the useful data each node carries with it */
typedef struct AstPayload_struct {

} AstPayload, *pAstPayload;


/* A tree like structure in plain C */
typedef struct AstNode_struct {
    unsigned int numOfChildren;
    struct AstNode_struct **children;
    pAstPayload payload;
} AstNode, *pAstNode;

/* Methods to work with the tree */
pAstPayload createAstPayload();

pAstNode createAstNode(pAstPayload payload);

void addAstChild(pAstNode tree, pAstNode child);

pAstNode getAstChild(pAstNode tree, unsigned int index);

void freeAst(pAstNode tree);


#endif //__ASTNODE_H

