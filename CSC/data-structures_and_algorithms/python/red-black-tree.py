from enum import Enum
from typing import Optional
Color = Enum('Color', [('RED', 1), ('GREEN', 2), ('BLUE', 3)])
class RBNode:
    def __init__(self, value, color = Color.RED):
        self.value = value
        self.color = color
        self.left: Optional[RBNode] = None
        self.right: Optional[RBNode] = None
        self.parent: Optional[RBNode] = None
    def grandparent(self):
        if self.parent and self.parent.parent:
            return self.parent.parent
        return None

    def sibling(self):
        if self.parent is None:
            return None

        if self.parent.left and self.parent.left.value == self.value:
            return self.parent.right
        return self.parent.left

    def uncle(self):
        if self.parent is None or self.parent.parent is None:
            return None
        if self.parent.parent.left and self.parent.parent.left.value == self.parent.value:
            return self.parent.parent.right
        return self.parent.parent.left

class RBTree:
    def __init__(self):
        self.root = None

    def insert_fix(self, node):

        while node.parent and node.parent.color == Color.RED:
        
            if node.grandparent() and node.parent == node.grandparent().left:
                uncle = node.uncle() 
                if uncle and uncle.color == Color.RED:
                    node.parent.color = Color.BLACK
                    uncle.color = Color.BLACK
                    node.grandparent().color = Color.RED
                    node = node.grandparent()
                else: # node has no granparent or parent is black
                    if node == node.parent.right:
                        node = node.parent
                    node.parent.color = Color.BLACK
                    node.grapndparent().color = Color.RED
                    self.roate_right(nof


    def insert(self, value):

        node = RBNode(value)

        if self.root is None:
            self.root = node
            return

        curr = self.root
       
        while curr is not None:


            if curr and value < curr.value:
                if curr.left is None:
                    curr.left = node
                    break
                else: 
                    curr = curr.left
                    continue

            else:
                if curr.right is None:
                    curr.right = node
                    break
                else:
                    curr = curr.right
                    continue
            
        self.insert_fix(node)



items = [ 1, 69, 420, 0, 5, 30, 4567, 10, 100, 2, 100 ]
rbt = RBTree()

for item in items:
    rbt.insert(item)

