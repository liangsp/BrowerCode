/*!
 * @file    AVLTree.h
 * @brief   AVL平衡二叉树
 *
 * Copyright (c) 2015 chinasoft Tech.Co.,Ltd
 *
 * @author	  lsp74391 
 * @date      2011/5/26
 * @version   1.0.0
 *
*******************************************************************************
 */

#ifndef __AVL_TREE_H__
#define __AVL_TREE_H__

#include <assert.h>
#include <stdlib.h>

namespace lspublic {

#ifndef max
    #define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

template <class T> class CAVLTree;
template <class T> class CAVLTreeIterator;

//template <class T> 
//class CAVLTreeDialog;


/*!
** @name      CAVLNode
** @brief     AVL-TREE平衡二叉树节点类
**
******************************************************************************* 
*/
template <class T>
class CAVLNode
{
public:
    // construction and destruction
    CAVLNode(T* data,
			 int balance = 0,
			 CAVLNode<T>* parent = NULL,
			 CAVLNode<T>* left = NULL,
			 CAVLNode<T>* right = NULL
	);
    virtual ~CAVLNode();

    // searching
    CAVLNode<T>* Search(const T* data);

    // comparing
    static int Compare(const T& t1, const T& t2);

    // information about the position in the tree
    bool IsRoot() const;
    bool IsLeftSibling() const;
    bool IsRightSibling() const;
    bool HasLeftSibling() const;
    bool HasRightSibling() const;

    // further information
    int GetDepth() const;
    int GetLevel() const;
    int NodesInTree() const;

    // navigation in the tree
    CAVLNode<T>* GetRoot();

    CAVLNode<T>* GetLeftSibling();
    CAVLNode<T>* GetRightSibling();

    CAVLNode<T>* GetFirstNodeInOrder();
    CAVLNode<T>* GetLastNodeInOrder();

    CAVLNode<T>* GetPrevNodeInOrder();
    CAVLNode<T>* GetNextNodeInOrder();

    // restructuration
    bool LeftRotation();
    bool RightRotation();
    bool DoubleRotationLeft();
    bool DoubleRotationRight();

    // Diagnostics
    bool CheckBalances(const char * buffer = NULL, unsigned int buflen = 0) const;

// drawing into a device context, not fully implemented
// #ifdef _MSC_VER
//     void Draw(CDC* dc, CRect& rect);
// #endif

    // item-data
    T* Data;

    // informationen about the height of the subtrees
    // -1: left subtree is by 1 higher than the right one
    //  0: both subtrees have the same height
    //  1: right subtree is by 1 higher than the left one
    int Balance;

    // parent node
    CAVLNode<T>* Parent;

    // left and right subtree
    CAVLNode<T>* Left;
    CAVLNode<T>* Right;
    
private:
    CAVLNode<T>* GetInsertPosition(const T* data);
    bool RestructureInsert();
    bool RestructureDelete();

    CAVLNode(const CAVLNode<T>& /*tree*/) {};
    CAVLNode& operator= (const CAVLNode<T>& /*tree*/) {return *this;};

    friend class CAVLTree<T>;
};

template <class T>
CAVLNode<T>::CAVLNode(T* data, int balance, CAVLNode<T>* parent, CAVLNode<T>* left, CAVLNode<T>* right)
    : Data(data),
      Balance(balance),
      Parent(parent),
      Left(left),
      Right(right)
{
};

template <class T>
CAVLNode<T>::~CAVLNode()
{
    delete Left;
    delete Right;
    delete Data;
};

template <class T>
bool CAVLNode<T>::RestructureInsert()
{
    CAVLNode<T>* item = this;
    while (!item->IsRoot())
    {
        // rule 1
        if (item->IsLeftSibling() && item->Parent->Balance == 0)
        {
            item->Parent->Balance = -1;
            item = item->Parent;
            continue;
        }
        if (item->IsRightSibling() && item->Parent->Balance == 0)
        {
            item->Parent->Balance = 1;
            item = item->Parent;
            continue;
        }
        // rule 2
        if (item->IsLeftSibling() && item->Parent->Balance == 1)
        {
            item->Parent->Balance = 0;
            break;
        }
        if (item->IsRightSibling() && item->Parent->Balance == -1)
        {
            item->Parent->Balance = 0;
            break;
        }
        // rule 3
        if (item->IsLeftSibling() && item->Parent->Balance == -1)
        {
            if (item->Balance == 1)
                item->Parent->DoubleRotationLeft();
            else
                item->Parent->RightRotation();
            break;
        }
        if (item->IsRightSibling() && item->Parent->Balance == 1)
        {
            if (item->Balance == -1)
                item->Parent->DoubleRotationRight();
            else
                item->Parent->LeftRotation();
            break;
        }
    }
    return true;
};

template <class T>
bool CAVLNode<T>::RestructureDelete()
{
    CAVLNode<T>* item = this;
    // Regel 1
    if (item->Balance == 0 && item->Left == NULL)
    {
        item->Balance = 1;
        return true;
    }
    if (item->Balance == 0 && item->Right == NULL)
    {
        item->Balance = -1;
        return true;
    }
    // Regel 2
    if (item->Balance == -1 && item->Left == NULL)
    {
        item->Balance = 0;
    }
    if (item->Balance == 1 && item->Right == NULL)
    {
        item->Balance = 0;
    }
    // Regel 3
    if (item->Balance == -1 && item->Right == NULL)
    {
        if (item->Left->Balance == 1)
        {
            item->DoubleRotationLeft();
            item = item->Parent; 
        }
        else
        {
            item->RightRotation();
            item = item->Parent; 
        }
        if (item->Balance == 1)
            return true;
    }
    if (item->Balance == 1 && item->Left == NULL)
    {
        if (item->Right->Balance == -1)
        {
            item->DoubleRotationRight();
            item = item->Parent;
        }
        else
        {
            item->LeftRotation();
            item = item->Parent; 
        }
        if (item->Balance == -1)
            return true;
    }

    while (!item->IsRoot())
    {
        // Regel 1
        if (item->IsLeftSibling() && item->Parent->Balance == 0)
        {
            item->Parent->Balance = 1;
            break;
        }
        if (item->IsRightSibling() && item->Parent->Balance == 0)
        {
            item->Parent->Balance = -1;
            break;
        }
        // Regel 2
        if (item->IsLeftSibling() && item->Parent->Balance == -1)
        {
            item->Parent->Balance = 0;
            item = item->Parent;
            continue;
        }
        if (item->IsRightSibling() && item->Parent->Balance == 1)
        {
            item->Parent->Balance = 0;
            item = item->Parent;
            continue;
        }
        // Regel 3
        if (item->IsRightSibling() && item->Parent->Balance == -1)
        {
            if (item->Parent->Left->Balance == 1)
            {
                item->Parent->DoubleRotationLeft();
                item = item->Parent->Parent; 
            }
            else
            {
                item->Parent->RightRotation();
                item = item->Parent->Parent; 
            }
            if (item->Balance == 1)
                return true;
            continue;
        }
        if (item->IsLeftSibling() && item->Parent->Balance == 1)
        {
            if (item->Parent->Right->Balance == -1)
            {
                item->Parent->DoubleRotationRight();
                item = item->Parent->Parent; 
            }
            else
            {
                item->Parent->LeftRotation();
                item = item->Parent->Parent; 
            }
            if (item->Balance == -1)
                return true;
            continue;
        }
        // Never appears...
        break;
    }
    return true;
};

template <class T>
CAVLNode<T>* CAVLNode<T>::Search(const T* data)
{
    switch (Compare(*Data, *data))
    {
    case -1:
        if (!Right) return NULL;
        return Right->Search(data);
    case  0: return this;
    case  1:
        if (!Left) return NULL;
        return Left->Search(data);
    }
    return NULL;
};

template <class T>
int CAVLNode<T>::Compare(const T& t1, const T& t2)
{
    return t1.Compare(t2);
};

template <class T>
bool CAVLNode<T>::IsRoot() const
{
    return Parent == NULL;
};

template <class T>
bool CAVLNode<T>::IsLeftSibling() const
{
    return !IsRoot() && Parent->Left == this;
};

template <class T>
bool CAVLNode<T>::IsRightSibling() const
{
    return !IsRoot() && Parent->Right == this;
};

template <class T>
bool CAVLNode<T>::HasLeftSibling() const
{
    return !IsRoot() && IsRightSibling() && Parent->Left != NULL;
};

template <class T>
bool CAVLNode<T>::HasRightSibling() const
{
    return !IsRoot() && IsLeftSibling() && Parent->Right != NULL;
};

template <class T>
int CAVLNode<T>::GetDepth() const
{
    int i = 0;
    if (Left != NULL)
        i = Left->GetDepth();
    if (Right != NULL)
        i = max(i, Right->GetDepth());
    return i+1;
};

template <class T>
int CAVLNode<T>::GetLevel() const
{
    const CAVLNode<T>* item = this;
    int level = 0;
    while (item->Parent != NULL)
    {
        item = item->Parent;
        level++;
    }
    return level;
};

template <class T>
int CAVLNode<T>::NodesInTree() const
{
    int Nodes = 1;
    if (Left != NULL)
        Nodes += Left->NodesInTree();
    if (Right != NULL)
        Nodes += Right->NodesInTree();
    return Nodes;
};

template <class T>
CAVLNode<T>* CAVLNode<T>::GetRoot()
{
    CAVLNode<T>* item = this;
    while (item->Parent != NULL) item = item->Parent;
    return item;
};

template <class T>
CAVLNode<T>* CAVLNode<T>::GetLeftSibling()
{
    if (IsRoot() || IsLeftSibling()) return NULL;
    return Parent->Left;
};

template <class T>
CAVLNode<T>* CAVLNode<T>::GetRightSibling()
{
    if (IsRoot() || IsRightSibling()) return NULL;
    return Parent->Right;
};

template <class T>
CAVLNode<T>* CAVLNode<T>::GetFirstNodeInOrder()
{
    CAVLNode<T>* item = this;
    while (item->Left != NULL)
    {
        item = item->Left;
    }
    return item;
};

template <class T>
CAVLNode<T>* CAVLNode<T>::GetLastNodeInOrder()
{
    CAVLNode<T>* item = this;
    while (item->Right != NULL)
    {
        item = item->Right;
    }
    return item;
};

template <class T>
CAVLNode<T>* CAVLNode<T>::GetPrevNodeInOrder()
{
    if (Left != NULL)
    {
        return Left->GetLastNodeInOrder();
    }
    if (IsRightSibling()) return Parent;
    CAVLNode<T>* item = this;
    while (!item->IsRoot())
    {
        if (item->IsLeftSibling())
            item = item->Parent;
        else
            return item->Parent;
    }
    return NULL;
};

template <class T>
CAVLNode<T>* CAVLNode<T>::GetNextNodeInOrder()
{
    if (Right != NULL)
    {
        return Right->GetFirstNodeInOrder();
    }
    if (IsLeftSibling()) return Parent;
    CAVLNode<T>* item = this;
    while (!item->IsRoot())
    {
        if (item->IsRightSibling())
            item = item->Parent;
        else
            return item->Parent;
    }
    return NULL;
};

template <class T>
CAVLNode<T>* CAVLNode<T>::GetInsertPosition(const T* data)
{
    switch (Compare(*Data, *data))
    {
    case -1:
        if (!Right) return this;
        return Right->GetInsertPosition(data);
    case  0:
        return NULL;
    case  1:
        if (!Left) return this;
        return Left->GetInsertPosition(data);
    }
    return NULL; 
};

template <class T>
bool CAVLNode<T>::LeftRotation()
{
    assert(Right != NULL);

    if (Right == NULL) return false;

    CAVLNode<T>* b = Right;
    if (!IsRoot())
    {
        if (IsLeftSibling())
            Parent->Left = b;
        else
            Parent->Right = b;
        b->Parent = Parent;
    }
    else
    {
        b->Parent = NULL;
    }

    Right = b->Left;
    b->Left = this;

    Parent = b;
    if (Right != NULL) Right->Parent = this;

    if (b->Balance == 0)
    {
        Balance = 1;
        b->Balance = -1;
    }
    else
    {
        Balance = 0;
        b->Balance = 0;
    }

    return true;
};

template <class T>
bool CAVLNode<T>::RightRotation()
{
    assert(Left != NULL);

    if (Left == NULL) return false;

    CAVLNode<T>* b = Left;
    if (!IsRoot())
    {
        if (IsLeftSibling())
            Parent->Left = b;
        else
            Parent->Right = b;
        b->Parent = Parent;
    }
    else
    {
        b->Parent = NULL;
    }

    Left = b->Right;
    b->Right = this;

    Parent = b;
    if (Left != NULL) Left->Parent = this;

    if (b->Balance == 0)
    {
        Balance = -1;
        b->Balance = 1;
    }
    else
    {
        Balance = 0;
        b->Balance = 0;
    }

    return true;
};

template <class T>
bool CAVLNode<T>::DoubleRotationLeft()
{
    assert(Left != NULL && Left->Right != NULL);

    if (Left == NULL || Left->Right == NULL) return false;

    CAVLNode<T>* b = Left;
    CAVLNode<T>* c = Left->Right;

    if (!IsRoot())
    {
        if (IsLeftSibling())
            Parent->Left = c;
        else
            Parent->Right = c;
    }

    b->Right = c->Left;
    Left = c->Right;
    c->Left = b;
    c->Right = this;

    c->Parent = Parent;
    Parent = c;
    b->Parent = c;
    if (b->Right != NULL) b->Right->Parent = b;
    if (Left != NULL) Left->Parent = this;

    switch (c->Balance)
    {
    case -1:
        Balance = 1;
        b->Balance = 0;
        break;
    case 0:
        Balance = 0;
        b->Balance = 0;
        break;
    case 1:
        Balance = 0;
        b->Balance = -1;
        break;
    }
    c->Balance = 0;

    return true;
};

template <class T>
bool CAVLNode<T>::DoubleRotationRight()
{
    assert(Right != NULL && Right->Left != NULL);

    if (Right == NULL || Right->Left == NULL) return false;

    CAVLNode<T>* b = Right;
    CAVLNode<T>* c = Right->Left;

    if (!IsRoot())
    {
        if (IsLeftSibling())
            Parent->Left = c;
        else
            Parent->Right = c;
    }

    Right = c->Left;
    b->Left = c->Right;
    c->Left = this;
    c->Right = b;

    c->Parent = Parent;
    Parent = c;
    b->Parent = c;
    if (Right != NULL) Right->Parent = this;
    if (b->Left != NULL) b->Left->Parent = b;

    switch (c->Balance)
    {
    case -1:
        Balance = 0;
        b->Balance = 1;
        break;
    case 0:
        Balance = 0;
        b->Balance = 0;
        break;
    case 1:
        Balance = -1;
        b->Balance = 0;
        break;
    }
    c->Balance = 0;

    return true;
};

//#ifdef _MSC_VER
//template <class T>
//void CAVLNode<T>::Draw(CDC* dc, CRect& rect)
//{
//    int depth = GetDepth();
//    CSize Size = dc->GetTextExtent(*Data);
//    dc->TextOut((rect.left+rect.right)/2-Size.cx/2, rect.top, *Data);
//
//    if (Left != NULL)
//    {
//        CRect rect1 = rect;
//        rect1.top += 2*Size.cy;
//        rect1.right = (rect.right+rect.left)/2;
//        dc->MoveTo((rect.left+rect.right)/2, rect.top+Size.cy);
//        dc->LineTo((rect1.left+rect1.right)/2, rect1.top);
//        Left->Draw(dc, rect1);
//    }
//    if (Right != NULL)
//    {
//        CRect rect2 = rect;
//        rect2.top += 2*Size.cy;
//        rect2.left = (rect.right+rect.left)/2;
//        dc->MoveTo((rect.left+rect.right)/2, rect.top+Size.cy);
//        dc->LineTo((rect2.left+rect2.right)/2, rect2.top);
//        Right->Draw(dc, rect2);
//    }
//};
//#endif


/*!
** @name      CAVLTree
** @brief     AVL-TREE平衡二叉树类
**
** Requirements for the class T:
** Objects of class T must be comparable.
** A function
** int T::Compare(const T& t) const
** must be implemented.
** Return values:
**    -1, if *this <  t
**     0, if *this == t
**     1, if *this >  t
**
******************************************************************************* 
*/
template <class T>
class CAVLTree
{
public:
    // construction and destruction
    CAVLTree();
    virtual ~CAVLTree();

    // information
    bool IsEmpty() const;
    int GetDepth() const;
    long NodesInTree() const;

    // access to the root of the tree
    CAVLNode<T>* GetRoot();

    // insert and delete
    CAVLNode<T>* Insert(T* data, bool autodelete = true);
    bool Delete(T* data);

    bool DeleteAll();

    // delete current node of an iterator
    bool Delete(CAVLTreeIterator<T>& iter, bool movenext = true);

    // search
    virtual CAVLNode<T>* Search(const T* data);
    virtual CAVLNode<T>* Search(T& data);
	
	virtual T* SearchObject(const T* data);
	virtual T* SearchObject(T& data);
	
//#ifdef _MSC_VER
//    void Draw(CDC* dc, CRect& rect);
//#endif

private:
    CAVLNode<T>* Tree;
    friend class CAVLTreeIterator<T>;
    //friend class CAVLTreeDialog<T>;
};

template <class T>
CAVLTree<T>::CAVLTree() : Tree(NULL)
{
};

template <class T>
CAVLTree<T>::~CAVLTree()
{
    delete Tree;
};

template <class T>
bool CAVLTree<T>::IsEmpty() const
{
    return Tree == NULL;
};

template <class T>
int CAVLTree<T>::GetDepth() const
{
    if (Tree == NULL) return 0;
    return Tree->GetDepth();
};

template <class T>
CAVLNode<T>* CAVLTree<T>::Insert(T* data, bool autodelete)
{
    // if the tree is empty, the root is used to store the entry
    if (IsEmpty())
    {
        Tree = new CAVLNode<T>(data, 0, NULL, NULL, NULL);
        return Tree;
    }
    
    // detect insert position
    CAVLNode<T>* item = Tree->GetInsertPosition(data);
    if (item == NULL)
    {
        if (autodelete)
            delete data;
        return NULL;
    }
    
    // create a new node
    CAVLNode<T>* newitem = new CAVLNode<T>(data, 0, item, NULL, NULL);
    
    // link with parents
    if (CAVLNode<T>::Compare(*(item->Data), *data) == -1)
        item->Right = newitem;
    else
        item->Left = newitem;
    
    // restructuration
    newitem->RestructureInsert();
    Tree = Tree->GetRoot();
    
    // return the new inserted node
    return newitem;
};

template <class T>
bool CAVLTree<T>::Delete(T* data)
{
    // if the tree is empty there is nothing to delete
    if (IsEmpty()) return false;
    
    CAVLNode<T>* item = Tree->Search(data);
    if (item == NULL) return false; 
    // element not in the tree

    // if we want to delete the root item, we have to do some extra
    // operation the preserve some pointers...
    if (item == Tree)
    {
        // the root is the only one node in the tree
        if (item->Left == NULL && item->Right == NULL)
        {
            delete Tree;
            Tree = NULL;
            return true;
        }
    }
    // start node for restructuration
    CAVLNode<T>* startitem = NULL;
    // node to delete has no children
    if (item->Left == NULL && item->Right == NULL)
    {
        if (item->IsLeftSibling())
            item->Parent->Left = NULL;
        else
            item->Parent->Right = NULL;
        startitem = item->Parent;
        delete item;
        item = NULL;
    }
    // node to delete has only right son
    if (item != NULL && item->Left == NULL && item->Right != NULL)
    {
        delete item->Data;
        item->Data = item->Right->Data;
        item->Right->Data = NULL;
        delete item->Right;
        item->Right = NULL;
        startitem = item;
    }
    // node to delete has only left son
    if (item != NULL && item->Left != NULL && item->Right == NULL)
    {
        delete item->Data;
        item->Data = item->Left->Data;
        item->Left->Data = NULL;
        delete item->Left;
        item->Left = NULL;
        startitem = item;
    }
    // node to delete has both sons
    if (item != NULL && item->Left != NULL && item->Right != NULL)
    {
        CAVLNode<T>* y = item->Left->GetLastNodeInOrder();
        CAVLNode<T>* z = y->Left;
        delete item->Data;
        item->Data = y->Data;
        y->Data = NULL;

        if (z != NULL)
        {
            y->Data = z->Data;
            z->Data = NULL;
            y->Left = NULL;
            delete z;
            startitem = y;
        }
        else
        {
            if (y->IsLeftSibling())
                y->Parent->Left = NULL;
            else
                y->Parent->Right = NULL;
            startitem = y->Parent;
            delete y;
        }
    }
    startitem->RestructureDelete();
    Tree = Tree->GetRoot();
    return true;
};

template <class T>
CAVLNode<T>* CAVLTree<T>::Search(const T* data)
{
    CAVLNode<T>* root = GetRoot();
    if (root==NULL)
        return NULL;
    return root->Search(data);
};

template <class T>
CAVLNode<T>* CAVLTree<T>::Search(T& data)
{
    CAVLNode<T>* root = GetRoot();
    if (root==NULL)
        return NULL;
    return root->Search(&data);
};

template <class T>
T* CAVLTree<T>::SearchObject(const T* data)
{
    CAVLNode<T>* root = GetRoot();
    if (root==NULL)
        return NULL;
	CAVLNode<T>* node = root->Search(data);
    return node ? node->Data : NULL;
};

template <class T>
T* CAVLTree<T>::SearchObject(T& data)
{
    CAVLNode<T>* root = GetRoot();
    if (root==NULL)
        return NULL;
	CAVLNode<T>* node = root->Search(&data);
    return node ? node->Data : NULL;
};

template <class T>
CAVLNode<T>* CAVLTree<T>::GetRoot()
{
    return Tree;
};

template <class T>
long CAVLTree<T>::NodesInTree() const
{
    if (Tree)
        return Tree->NodesInTree();
    return 0;
};

template <class T>
bool CAVLNode<T>::CheckBalances(const char * buffer, unsigned int buflen) const
{
    int d1 = 0;
    int d2 = 0;
    if (Left)
    {
        d1 = Left->GetDepth();
        if (!Left->CheckBalances(buffer, buflen)) return false;
    }
    if (Right)
    {
        d2 = Right->GetDepth();
        if (!Right->CheckBalances(buffer, buflen)) return false;
    }
    if (abs(d1-d2)>1)
    {
        if (buffer)
            sprintf(buffer, "Depths are incorrect for AVL-Tree (Left %d, Right %d), level: %d", d1, d2, GetLevel());
        return false;
    }
    if (Balance == -1 && d1 >  d2) return true;
    if (Balance ==  0 && d1 == d2) return true;
    if (Balance ==  1 && d1 <  d2) return true;
    if (buffer)
        sprintf(buffer, "Balance does not match depths: (Left %d, Right %d, Balance %d), level: %d", d1, d2, Balance, GetLevel());
    return false;
};

template <class T>
bool CAVLTree<T>::DeleteAll()
{
    delete Tree;
    Tree = NULL;
    return true;
};

template <class T>
bool CAVLTree<T>::Delete(CAVLTreeIterator<T>& iter, bool movenext)
{
    if (!iter)
        return false;
    CAVLNode<T>* item = iter.Current;
    CAVLNode<T>* item1 = NULL;
    if (movenext)
    {
        item1 = item->GetPrevNodeInOrder();
        iter.MoveNext();
    }
    else
    {
        item1 = item->GetNextNodeInOrder();
        iter.MovePrev();
    }
    T* t = (iter ? iter.Current->Data : NULL);
    bool ret = Delete(item->Data);
    if (t)
    {
        iter.Current = Search(t);
    }
    iter.Tree = NULL;
    if (iter.Current != NULL)
        iter.Tree = iter.Current->GetRoot();
    if (iter.Current == NULL && item1 != NULL)
        iter.Tree = item1->GetRoot();
    return ret;
};

//#ifdef _MSC_VER
//template <class T>
//void CAVLTree<T>::Draw(CDC* dc, CRect& rect)
//{
//    if (IsEmpty()) return;
//    Tree->Draw(dc, rect);
//};
//#endif


/*!
** @name      CAVLTreeIterator
** @brief     AVL-TREE平衡二叉树迭代类
**
******************************************************************************* 
*/
template <class T>
class CAVLTreeIterator
{
public:
    // construction and destruction
    CAVLTreeIterator(CAVLNode<T>* tree);
    CAVLTreeIterator(CAVLTree<T>& tree);
    virtual ~CAVLTreeIterator();

    // validity
    operator bool();

    // navigation
    void operator ++();
    void operator --();
    void operator ++(int);
    void operator --(int);
    void Reset();
    void MoveFirst();
    void MoveLast();
    void MoveNext();
    void MovePrev();

    // access to current node
    T* GetData();
    CAVLNode<T>* Current;
private:
    CAVLNode<T>* Tree;
    CAVLTreeIterator(const CAVLTreeIterator<T>& /*treeiterator*/) {};
    CAVLTreeIterator& operator= (const CAVLTreeIterator<T>& /*treeiterator*/) {return *this;};

    friend class CAVLTree<T>;
};

template <class T>
CAVLTreeIterator<T>::CAVLTreeIterator(CAVLNode<T>* tree)
    : Tree(tree)
{
    if (Tree != NULL)
        Current = Tree->GetFirstNodeInOrder();
    else
        Current = NULL;
};

template <class T>
CAVLTreeIterator<T>::CAVLTreeIterator(CAVLTree<T>& tree)
    : Tree(tree.Tree)
{
    if (Tree != NULL)
        Current = Tree->GetFirstNodeInOrder();
    else
        Current = NULL;
};

template <class T>
CAVLTreeIterator<T>::~CAVLTreeIterator()
{
};

template <class T>
void CAVLTreeIterator<T>::Reset()
{
    if (Tree)
        Current = Tree->GetFirstNodeInOrder();
    else
        Current = NULL;
};

template <class T>
void CAVLTreeIterator<T>::MoveFirst()
{
    if (Tree)
        Current = Tree->GetFirstNodeInOrder();
    else
        Current = NULL;
};

template <class T>
void CAVLTreeIterator<T>::MoveLast()
{
    if (Tree)
        Current = Tree->GetLastNodeInOrder();
    else
        Current = NULL;
};

template <class T>
CAVLTreeIterator<T>::operator bool()
{
    return Current != NULL;
};

template <class T>
void CAVLTreeIterator<T>::operator ++()
{
    if (Current == NULL) return;
    Current = Current->GetNextNodeInOrder();
};

template <class T>
void CAVLTreeIterator<T>::operator --()
{
    if (Current == NULL) return;
    Current = Current->GetPrevNodeInOrder();
};

template <class T>
void CAVLTreeIterator<T>::operator ++(int)
{
    if (Current == NULL) return;
    Current = Current->GetNextNodeInOrder();
};

template <class T>
void CAVLTreeIterator<T>::operator --(int)
{
    if (Current == NULL) return;
    Current = Current->GetPrevNodeInOrder();
};

template <class T>
void CAVLTreeIterator<T>::MoveNext()
{
    if (Current == NULL) return;
    Current = Current->GetNextNodeInOrder();
};

template <class T>
void CAVLTreeIterator<T>::MovePrev()
{
    if (Current == NULL) return;
    Current = Current->GetPrevNodeInOrder();
};

template <class T>
T* CAVLTreeIterator<T>::GetData()
{
    if (Current)
        return Current->Data;
    else
        return NULL;
};

/* 
template <class T>
class CAVLTreeDialog : public CDialog
{
public:
    // standard constructor
    CAVLTreeDialog(CAVLTree<T>& tree, CWnd* pParent = NULL);   

    // Dialog Data
    enum { IDD = IDD_VEKTORTREEDIALOG1 };
    CTreeCtrl   m_Tree;


    // Overrides
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Implementation
protected:
    CAVLTree<T>& Tree;
    CImageList  m_ctlImage;

    virtual BOOL OnInitDialog();
    virtual void Insert(CAVLNode<T>* item, HTREEITEM pos);
};

template <class T>
CAVLTreeDialog<T>::CAVLTreeDialog(CAVLTree<T>& tree, CWnd* pParent)
    : CDialog(CTreeDialog::IDD, pParent), Tree(tree)
{
};

template <class T>
void CAVLTreeDialog<T>::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TREE1, m_Tree);
};

template <class T>
BOOL CAVLTreeDialog<T>::OnInitDialog()
{
    bool ret = CDialog::OnInitDialog();
    m_ctlImage.Create(IDB_BITMAP1,16,0,RGB(255,0,255));
    m_ctlImage.SetBkColor(GetSysColor(COLOR_WINDOW));
    m_Tree.SetImageList(&m_ctlImage, TVSIL_NORMAL);

    Insert(Tree.Tree, TVI_ROOT);
    return ret;
};

template <class T>
void CAVLTreeDialog<T>::Insert(CAVLNode<T>* tree, HTREEITEM pos)
{
    if (tree == NULL)
    {
        m_Tree.InsertItem(_T("NULL"), 3, 3, pos);
        return;
    }
    int image = 1 + tree->Balance;
    HTREEITEM newpos = m_Tree.InsertItem(*(tree->Data), image, image, pos);
    Insert(tree->Left, newpos);
    Insert(tree->Right, newpos);
};
*/

}

#endif//__AVL_TREE_H__

