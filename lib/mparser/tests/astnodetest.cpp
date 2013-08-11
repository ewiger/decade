#include <gtest/gtest.h>

namespace {

// Hide main
#define TESTING

#define KIDS_NUM 10

#include "../astnode.c"

    class AstNodeTestSuite : public testing::Test
    {
        void SetUp()
        {
        }
     
        void TearDown() {}

    };
     
    TEST_F(AstNodeTestSuite, AstNodeCanCreateNewNodes) {
        pAstNode tree = createAstNode(NULL);
        ASSERT_FALSE(tree == NULL);
        EXPECT_EQ(tree->numOfChildren, 0);
        ASSERT_TRUE(tree->children == NULL);
    }

    TEST_F(AstNodeTestSuite, AstNodeCanFreeRoot) {
        pAstNode tree = createAstNode(NULL);
        ASSERT_FALSE(tree == NULL);
        freeAst(tree);
    }

    TEST_F(AstNodeTestSuite, AstNodeNoPayloadWorkflow) {
        pAstNode kid, kid2, root = createAstNode(NULL);

        // populate with x kids
        for (int i=0;i<KIDS_NUM;i++) {
            pAstNode kid = createAstNode(NULL);
            addAstChild(root, kid);
        }
        ASSERT_TRUE(root->numOfChildren == KIDS_NUM);

        // append at deeper level
        kid = getAstChild(root, 0);
        kid2 = createAstNode(NULL);
        addAstChild(kid, kid2);
        ASSERT_TRUE(kid->numOfChildren == 1);

        freeAst(root);
    }


} // namespace


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

