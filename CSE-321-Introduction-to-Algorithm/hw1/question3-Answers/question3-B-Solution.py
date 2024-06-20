# Analysis is under the function

import Question3 as bst

def find_kth_smallest(bst, k):
  """
  Finds the k-th smallest element in a binary search tree.

  Args:
      bst: The binary search tree.
      k: The index of the k-th smallest element.

  Returns:
      The k-th smallest element in the binary search tree.
  """

  if bst is None or k <= 0:
    return None

  stack = []
  node = bst.root
  count = 0

  while node is not None or stack:
    if node is not None:
      stack.append(node)
      node = node.left
    else:
      node = stack.pop()

    count += 1

    if count == k:
      return node.key

    if node is None:
      node = None
    else:
      node = node.right or None

  return None
# Analyzing the find_kth_smallest function

# N is a number of nodes in the tree.

# Line 14
# Checking is none or not takes only O(1)

# Line 17-19
# Initializing a stack and variables take constant time, O(1)

# While loop
# In the worst case, this loop will traverse all N nodes of the BST, so it contributes O(N) to time complexity.

# Conditions-Pop-Push in while loop
# These constant time operations do not effect the time complexity bcs these are bounded by the O(N) traversal

# Total complexity = O(1) +O (N) = O(N)
