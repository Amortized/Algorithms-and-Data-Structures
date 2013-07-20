""" Author : Rahul Manghwani. NYU Courant rrm333@nyu.edu
    Binomial Heaps Implementation
    Insert ele, Merge Heaps,Del ele, Find Min ele, Decrease Key ele,Del Min Ele all in O(log n)
"""

def test():
    data = [500,20,30,400,580,60,700,80,90]
    H = BinomialHeap()
    tempNode = None
    for d in data:
        n = HeapNode(d)
        if d == max(data):
            tempNode = n
        insert(H, n)
        print_heap(H.head, 'o', '->')


    decreaseKey(H, tempNode, 1)
    print_heap(H.head, 'o', '->')


    for i in xrange(len(data)/2):
        n = removeMin(H)
        d = n.key
        print "Min Element (%2d) ---"%(d)
        print_heap(H.head, 'o', '->')




    n = removeMin(H)
    d = n.key
    print "-Min Now (%2d) ---"%(d)
    print_heap(H.head, 'o', '->')
    

""" Test Ends"""



class HeapNode:
    def __init__(self,key):
        self.key = key
        self.parent = None
        self.child = None
        self.sibling = None
        self.degree = 0

class BinomialHeap:
    def __init__(self):
        self.head = None

def linkNodes(x,y):
    """ Links Binomial Tree rooted at x to y """
    x.sibling = y.child
    x.parent = y
    y.child = x
    y.degree = y.degree + 1

def findMin(H):
    x = H.head
    y = x
    if x != None:
        minKey = x.key
        x = x.sibling
        while x!= None:
            if x.key < minKey:
                y = x
                minKey = x.key
            x = x.sibling
    return y

def merge(Hx,Hy):
    """ Orders the nodes of two heaps in increasing order of degrees"""
    x = Hx.head
    y = Hy.head
    
    class SiblingGen:
        sibling = None

    """Head of New List"""
    h = SiblingGen()
    """Set the tail to head"""
    t = h
    while x!=None and y!=None:
        if(x.degree <= y.degree):
            t.sibling = x
            t = x
            x = x.sibling
        else :
            t.sibling = y
            t = y
            y = y.sibling 
                
    if x!=None:
        t.sibling = x
    else :
        t.sibling = y

    return h.sibling

def union(Hx,Hy):
    """Make a New Heap"""
    Hz = BinomialHeap()
    Hz.head = merge(Hx,Hy)
    """If both heaps are empty"""
    if Hz.head == None:
        return Hz
    prev = None
    x = Hz.head
    while x.sibling != None:
        if (x.degree != x.sibling.degree) or (x.sibling.sibling != None and x.sibling.sibling.degree == x.degree):
            """ If the roots have different order or nodes siblings siblings has same deg as node its safe to move ahead"""
            prev = x
            x = x.sibling
        else :
            if x.key <= x.sibling.key:
                """x will be the root. x.sibling will be linked into it. x does not move head here"""
                y = x.sibling
                x.sibling = x.sibling.sibling
                linkNodes(y,x)
            else :
                """ x.sibling will be the root"""
                y = x.sibling
                """Set the previous pointer"""
                if prev != None:
                    prev.sibling = y
                else :
                    Hz.head = y
                linkNodes(x,y)
                x = y #Change x for next stage
    return Hz

def insert(H,newNode):
    """Make a New Heap"""
    H1 = BinomialHeap()
    H1.head = newNode
    T = union(H,H1)
    """Change the Old head"""
    H.head = T.head 

def decreaseKey(H,node,newKey):
    assert(newKey <= node.key)
    if node.key == newKey:
        return
    node.key = newKey
    """Now Swap with parent till tree is adjusted"""
    y = node
    z = y.parent
    while z!=None and z.key > y.key:
        """Go up"""
        """Swap the keys"""
        temp = y.key
        y.key = z.key
        z.key = temp
        """Swap the pointers"""
        y = z
        z = y.parent
        
def removeMin(H):

    if H.head == None:
        return None
    
    """Find the Tree which has the min node and remove it"""
    minNode = H.head
    prev = None
    cur = minNode
    
    x = minNode.sibling
    while x!=None:
        if x.key < minNode.key:
            prev = cur
            minNode = x
        cur = x
        x = x.sibling
        
            
    if prev == None:
        H.head = minNode.sibling
    else :
        prev.sibling = minNode.sibling

    """Now prepare a heap from Min Nodes children in increasing order of degrees"""
    newHeap = BinomialHeap()
    x = minNode.child
    while x!=None:
        x.parent = None
        y = x.sibling #Next Child
        x.sibling = newHeap.head  #Reverse Order. Changed Here
        newHeap.head = x
        x = y


    """Now take the Union of two heaps"""
    T = union(H,newHeap)
    H.head = T.head
    
    """Free the Minnode"""
    minNode.sibling = None
    minNode.child = None
    minNode.degree = 0
    minNode.parent = None
    
    """Return the Min Node"""
    return minNode
    
def deleteEle(H,node):
    """Get the Min"""
    minNode = findMin(H)
    """Decrease the key of the node to less than min"""
    decreaseKey(H,node,minNode.key - 1)
    """That Tree has been adjusted by decreaseKey by [putting this at the root of the tree,Now adjust the Heap"""
    #Heap Extracts this min
    removeMin(H)


def print_heap(x, indent='#', char='#'):
    while x != None:
        print "%s %d [%d]" %(indent, x.key, x.degree)
        if x.child != None:
            print_heap(x.child, indent+char, char)
        x = x.sibling


if __name__ == '__main__': test()

    
                               
            
        
    
