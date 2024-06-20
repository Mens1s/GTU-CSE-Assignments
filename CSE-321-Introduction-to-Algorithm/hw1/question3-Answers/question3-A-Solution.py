import Question3 as bst
# Analysis is under the function

def merge_BSTs(bst1, bst2):
    """
    merges two binary search trees into 1 bst

    Args:
        bst1 : the first binary search treee
        bst2 : the second binary search treee
    """
    
    def inorder_traversal_list(node, result):
        if node:
            inorder_traversal_list(node.left, result)
            result.append(node.key)
            inorder_traversal_list(node.right, result)

    merged_list = []
    inorder_traversal_list(bst1.root, merged_list)
    inorder_traversal_list(bst2.root, merged_list)

    mergedBST = bst.BST() # BST()
    for key in merged_list:
        mergedBST.insert(key)

    return mergedBST        

# Analyzing the merge_BSTs function

# N is a number of nodes in the tree.
# N1 is a number of nodes in the bst1
# N2 is a number of nodes in the bst2

# inorder_traversal_list of a binary search tree takes O(N) time. 

# Line 19
# In the worst case, the inorder_traversal_list function will visit every node in the tree
# So the time complexity will be O(N1)

# Line 20
# Also line 20 time complextiy is O(N2) desc is same as line 19

# For loop in the 23
# inserting operation, takes O(N1+N2) time in the worst case.

# Total complexity = O(N1) +O (N2) + O(N1+N2) = O(N1+N2)