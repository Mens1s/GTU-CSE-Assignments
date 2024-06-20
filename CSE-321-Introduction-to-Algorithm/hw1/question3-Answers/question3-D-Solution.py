# Analysis is under the function
def find_elements_in_range(bst, lower_bound, upper_bound):
  """
  Finds all elements in a binary search tree that are within a args ranged.

  Args:
    bst: The binary search tree.
    lower_bound: The lower bound of the value range.
    upper_bound: The upper bound of the value range.

  Returns:
    A list of all elements in the binary search tree that are within the args ranged.
  """

  results = []

  def search_subtrees(node, lower_bound, upper_bound):
    if node is None:
      return

    if lower_bound <= node.key <= upper_bound:
      results.append(node.key)

    search_subtrees(node.left, lower_bound, upper_bound)

    search_subtrees(node.right, lower_bound, upper_bound)

  search_subtrees(bst.root, lower_bound, upper_bound)

  return results

# Analyzing the find_elements_in_range function

# Line 13
# Takes constant time O(1)

# search_subtrees function 
# Takes O(N) time in worst case because it visit all N nodes.
# Adding and checking values is contant time O(1)
# So function time complexity is = O(1) + O(N) = O(N)

# Time complexity = O(1) + O(N) = O(N) 