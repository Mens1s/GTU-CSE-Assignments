def is_balanced(root):
    if root is None:
        return True
    
    left_h = height_of_tree(root.left)
    right_h = height_of_tree(root.right)
    
    if( abs(left_h-right_h) <= 1 and is_balanced(root.left) is True and is_balanced(root.right) is True):
        return True
    return False

def height_of_tree(root):
    if root is None:
        return 0
    
    left_h = height_of_tree(root.left)
    right_h = height_of_tree(root.right)
    
    if left_h > right_h:
        return left_h + 1
    return right_h + 1
