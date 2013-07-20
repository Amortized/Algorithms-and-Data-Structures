""" Author : Rahul Manghwani. NYU Courant rrm333@nyu.edu
    Fibonacci Heaps Implementation
    Insert ele, Merge Heaps,Find Min ele and decrease key in O(1)
    Dont use this data structure if there are a lot of extract min or delete operations as they take O(log n) amortized time
"""


class FibonacciHeap:
    def __init__(self):
        self.min = None
        self.noNodes = 0

class FibonacciNode:
    def __init__(self,key):
        self.key = key
        self.parent = None
        self.mark = False
        self.degree = 0
        self.child = None
        self.left = self
        self.right = self

def findMin(H):
    return H.min

def torange(a, b, step=1):
    return xrange(a, b+1, step)

def insert(H,newNode):
    newNode.parent = None
    newNode.left = newNode
    newNode.right = newNode
    newNode.degree = 0
    newNode.child = None
    newNode.mark = False

    if H.min == None:
        H.min = newNode
    else:
        temp1 = H.min.left
        temp2 = H.min
        """ Put it in between """
        temp1.right = newNode
        temp2.left = newNode
        newNode.left = temp1
        newNode.right = temp2
        """Check if its min"""
        if newNode.key < H.min.key:
            H.min = newNode
    H.noNodes = H.noNodes + 1

def addChildren(x,y):
    """Adds List from x to y"""
    if x == None or y == None:
        return
    z = x
    #Set the Parents
    while z.right != x:
        z.parent = y.parent
        z = z.right
    z.parent = y.parent
    #Build the 1st Bridge
    x.left = y.left
    y.left.right = x
    #Build the 2nd Bridge
    z.right = y
    y.left = z
    

def union(H1,H2):
    H = FibonacciHeap()
    if H1 == None and H2 != None:
        H.min = H2.min
        H.noNodes = H2.noNodes
    elif H1 != None and H2 == None:
        H.min = H1.min
        H.noNodes = H1.noNodes
    elif H1.min != None and H2.min != None:
        """Just Concatenate for now"""
        addChildren(H2.min, H1.min)
        """Set the min"""
        if H1.min.key <= H2.min.key:
            H.min = H1.min
        else:
            H.min = H2.min
        """Set the No of Nodes"""
        h.noNodes = h1.noNodes + h2.noNodes
    return H

def extract(H):
    x = H.min
    if x != None:
        """Min Exists"""
        """Stage1 Add the children of the min Node"""
        if x.child != None:
            addChildren(x.child,x)
        """Stage2 Remove the Node"""
        x.left.right = x.right
        x.right.left = x.left
        """Get the Min"""
        """If there is only one node"""
        if x == x.right:
            H.min = None
        else:
            H.min = x.right
            adjustHeap(H)
        H.noNodes = H.noNodes - 1
        """Clear the pointers in the deleted Node"""
        x.left = x
        x.right = x
        x.child = None
        x.degree = 0
        x.parent = None
        x.mark = False
    return x


def maxDegree(n):
    """Maximum Deg is floor(lg(n))"""
    lg = 0
    while n/2 > 0:
        lg = lg + 1
        n = n/2
    return lg
    
def link(y,x):
    """Make Tree at Root y as a child of Root x"""
    y.left.right =  y.right
    y.right.left =  y.left 
    y.parent = x
    if x.child != None:
        x.child.left.right = y
        y.left = x.child.left
        y.right = x.child
        x.child.left = y
    else:
        x.child = y
        y.left = y
        y.right = y
    x.degree = x.degree + 1
    y.mark = False
    
    
def adjustHeap(H):
    """Makes sure no two nodes are of the same degree"""
    maxDeg = maxDegree(H.noNodes)
    """We use to array to keep track of which degrees have already been seen"""
    Arr = []
    for i in torange(0, maxDeg):
        Arr.append(None)

    roots = []
    x = H.min
    #Break the Chain
    x.left.right = None
    """ Add all the roots list to a temp array"""
    while x != None:
        nextNode = x.right
        x.left = x
        x.right = x
        roots.append(x)
        x = nextNode
    
    """Now Go through all the nodes, if a node of that degree exists then combine them by adding the one"""
    """making larger key node as child of smaller key node"""
    for k in roots:
        d = k.degree
        while Arr[d] != None:
            y = Arr[d]
            if y.key < k.degree:
                #Swap
                k,y = y,k
            link(y,k)
            Arr[d] = None
            d = d + 1
        Arr[d] = k

    """ Now set the Min Node accordingly"""
    H.min = None
    for i in Arr:
        if i != None:
            i.left = i
            i.right = i
            i.parent = None
            if H.min == None:
                H.min = i
            else:
                #add this root node
                addChildren(i, H.min)
                if i.key < H.min.key:
                    H.min = i
                
    
def decreaseKey(H,node,key):
    if key == node.key:
        return
    node.key = key
    nodeParent = node.parent
    if nodeParent != None and node.key < nodeParent.key:
        """ Heap Property is disturbed. so need to rebalance"""
        cut(H,node, nodeParent)
        keepcutting(H,nodeParent)
    if node.key < H.min.key:
        H.min = node

def cut(H, node, nodeParent):
    # Remove node as a child of nodeParent
    node.left.right = node.right
    node.right.left = node.left
    nodeParent.degree = nodeParent.degree - 1
    nodeParent.child = node.right
    if node == node.right:
        #No child
        nodeParent.child = None
    #Add this Node as one of the roots
    node.parent = None
    node.mark = False
    node.left = H.min.left
    node.right = H.min
    node.left.right = x
    node.right.left = x

def keepcutting(H,nodeParent):
    p = nodeParent.parent
    if p != None:
        if p.mark == False:
            #We keep cutting till we hit a node which hasn't been marked
            p.mark = True
        else:
            cut(H, nodeParent, p)
            keepcutting(H,p)


    
    
        
            
    
    
        
