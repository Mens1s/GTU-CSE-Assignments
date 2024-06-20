# Analysis is under the function
import Question3 as bst

def balance_BST(bst):
  """
  Balances a binary search tree which is BST

  Args:
    bst: The binary search tree to be balanced.

  Returns:
    A balanced binary search tree.
  """
  def inorder_traversal_list(node, result):
        if node:
            inorder_traversal_list(node.left, result)
            result.append(node.key)
            inorder_traversal_list(node.right, result)

  sorted_array = []
  inorder_traversal_list(bst.root, sorted_array)
  
  # balance tree olustur h icin len den 1 cikar
  balanced_BST = bst.BST()
  balanced_BST.root = build_balanced_BST(sorted_array, 0, len(sorted_array) - 1)

  return balanced_BST

def build_balanced_BST(sorted_array, low, high):
  """
  Builds a balanced binary search tree from a sorted array.

  Args:
    sorted_array: The sorted array.
    low: The low index of the array.
    high: The high index of the array.

  Returns:
    The root node of the balanced binary search tree.
  """

  if low > high:
    return None

  mid = (low + high) // 2

  new_node = bst.BSTNode(sorted_array[mid])

  new_node.left = build_balanced_BST(sorted_array, low, mid - 1)
  new_node.right = build_balanced_BST(sorted_array, mid + 1, high)

  return new_node


# Analyzing the balance_BST function

# N is a number of nodes in the tree.

# in_order_traversal function takes O(N) time

# Building the balanced BST from the sorted array, takes O(N) time because each element in the sorted array is proccessed once.

# Total complexity = O(N) +O (N) += O(N)
