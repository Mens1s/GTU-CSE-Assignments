# bst class
class BST:
  """A binary search tree class."""

  def __init__(self, root=None):
    self.root = None
    self.sortedList = []

  def insert(self, key):
    """Inserts a new node into the binary search tree."""
    new_node = BSTNode(key)

    if self.root is None:
      self.root = new_node
    else:
      current_node = self.root
      while current_node is not None:
        if key < current_node.key:
          if current_node.left is None:
            current_node.left = new_node
            break
          else:
            current_node = current_node.left
        else:
          if current_node.right is None:
            current_node.right = new_node
            break
          else:
            current_node = current_node.right

  def search(self, key):
    """Searches for a node with the given key in the binary search tree."""
    current_node = self.root
    while current_node is not None:
      if key < current_node.key:
        current_node = current_node.left
      elif key > current_node.key:
        current_node = current_node.right
      else:
        return current_node

    return None

  def inorder_traversal(self, node=None):
    """Performs an inorder traversal of the binary search tree."""
    if node is None:
      node = self.root

    if node is not None:
      
      if node.left is not None:
        self.inorder_traversal(node.left)
        
      print(node.key)
      self.sortedList.append(node.key)
      if node.right is not None:
        self.inorder_traversal(node.right)
      
  def getSortedList(self, node):
    
    self.sortedList = []
    self.inorder_traversal(node)
    
    return self.sortedList


class BSTNode:
  """A node class for binary search trees."""

  def __init__(self, key):
    self.key = key
    self.left = None
    self.right = None
    
def count_nodes(node):
  if node is None:
    return 0

  return 1 + count_nodes(node.left) + count_nodes(node.right)

# solution of a
def merge_BSTs(bst1, bst2): 
    def inorder_traversal_list(node, result):
        if node:
            inorder_traversal_list(node.left, result)
            result.append(node.key)
            inorder_traversal_list(node.right, result)

    merged_list = []
    inorder_traversal_list(bst1.root, merged_list)
    inorder_traversal_list(bst2.root, merged_list)

    mergedBST = BST()
    for key in merged_list:
        mergedBST.insert(key)

    return mergedBST        
    
# solution of b
def find_kth_smallest(bst, k):

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

# solution of c
def balance_BST(bst):

  def inorder_traversal_list(node, result):
        if node:
            inorder_traversal_list(node.left, result)
            result.append(node.key)
            inorder_traversal_list(node.right, result)

  sorted_array = []
  inorder_traversal_list(bst.root, sorted_array)
  
  balanced_BST = BST()
  balanced_BST.root = build_balanced_BST(sorted_array, 0, len(sorted_array) - 1)

  return balanced_BST

def build_balanced_BST(sorted_array, low, high):

  if low > high:
    return None

  mid = (low + high) // 2

  new_node = BSTNode(sorted_array[mid])

  new_node.left = build_balanced_BST(sorted_array, low, mid - 1)
  new_node.right = build_balanced_BST(sorted_array, mid + 1, high)

  return new_node

# solution of d
def find_elements_in_range(bst, lower_bound, upper_bound):
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

bst = BST()
bst.insert(10)
bst.insert(2)
bst.insert(3)
bst.insert(4)
bst.insert(5)
bst.insert(6)
bst.insert(1)
bst.insert(0)

bst2 = BST()
bst2.insert(100)
bst2.insert(20)
bst2.insert(30)
bst2.insert(40)
bst2.insert(50)
bst2.insert(60)
bst2.insert(10)
bst2.insert(-1)
# TEST CASES

# sol of a
bstmerge = merge_BSTs(bst, bst2)
bstmerge.inorder_traversal()

# sol of b
b = find_kth_smallest(bst, 5)
print(b)

# sol of c
bst2.inorder_traversal()
balance_BST(bst2)
bst2.inorder_traversal()

# sol of d
print(find_elements_in_range(bst, 4, 10))