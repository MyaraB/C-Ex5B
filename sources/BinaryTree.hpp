#include <deque>
#include <stack>
#include <iostream>
#include <iterator>

namespace ariel
{
    template <typename T>
    class BinaryTree
    {
        //build a template Node class for the Binary Tree
        class Node
        {
            T val;// template value
            Node *left, *right, *father; // left and right sons of node
             // initialize node
            Node(T value)
                : val(value), left(nullptr), right(nullptr), father(nullptr){};
        
            friend class BinaryTree; // in order to have access to the nodes
            
            T get_val(){
            return val;
        }

        bool operator==(Node nodeB)
            {
                return (this->right == nodeB.right &&  this->left == nodeB->left && this->father == nodeB->father && this->data == nodeB->data);
            }
        };
        
       
        Node *root;

        enum OrderBy // different order types for our binary tree
        {
            preorder,
            inorder,
            postorder
        };
        

    public:
    // creating an iterator class to be able to iterate over the binary tree
        class iterator
        {
            template <Node *, class Container = std::deque<Node *>> //help get the correct order of the tree  by using stacks



            class stack;

            Node *nd;
            OrderBy orderby;
            std::stack<Node *> tack;

        public:

        
            iterator(Node *start, const OrderBy order) : orderby(order), nd(start) // to be able to choose an iteration depending on binary tree order (pre,in,post)
            {
                initStack(start);
                if (!tack.empty())
                {
                    nd = tack.top();
                    tack.pop();
                }
                
            }

            iterator() : nd(nullptr){};

            Node *get_nd(){
                //pointer to node
                return nd;
            }
            
            

            void initStack(Node *a)// initialize stack by order. helps to iterate over binary tree in the correct order.
            {
                if (a == nullptr)
                {
                    return;
                }
                if (orderby == preorder)
                {
                    //preorder
                    initStack(a->right);
                    initStack(a->left);
                    tack.push(a);
                }
                else if (orderby == inorder)
                {
                    //inorder
                    initStack(a->right);
                    tack.push(a);
                    initStack(a->left);
                }
                else
                {
                    //postorder
                    tack.push(a);
                    initStack(a->right);
                    initStack(a->left);
                }
            }

            iterator operator++(int) // postfix ++
            {
                iterator plus = *this;
                if (tack.empty())
                {   
                    nd = nullptr;
                }
                else{
                    nd = tack.top();
                    tack.pop();
                }
                return plus;
            }

            iterator &operator++() // prefix ++
            {
                if (tack.empty()) // if our stack is empty make sure iterator points at nullptr
                {   
                    nd = nullptr;
                }
                else{
                    nd = tack.top();
                    tack.pop();
                }
                
                return *this;
            }

            T &operator*() const  // returns refrence to val
            {
                return nd->val;
            }

            T *operator->() const // returns pointer to val
            {
                return &(nd->val);
            }
            bool operator==(const iterator &iter) const
            {
                return nd == iter.nd;
            }

            bool operator!=(const iterator &iter) const
            {
                return !(*this == iter);
            }
        };





        BinaryTree<T>():root(nullptr){};

        // add new root or override old root

        BinaryTree<T> &add_root(T a)  
        {
            if(root!=nullptr){
                root->val = a;
            }
            else{
                this->root = new Node(a);
            }
            return *this;
        }
        //copy stuff
         void tree_copy(Node *nodeA, const Node *nodeB)
         {
             if (nodeB->right != nullptr)
            {
                nodeA->right = new Node(nodeB->right->val);
                tree_copy(nodeA->right, nodeB->right);
            }

             if (nodeB->left != nullptr)
            {
                nodeA->left = new Node(nodeB->left->val);
                tree_copy(nodeA->left, nodeB->left);
            }
         }

        //required operators
         BinaryTree<T> &operator=(const BinaryTree<T> &tr)
        {
             if (root != nullptr)
            {
                //make the root go bye bye
                delete root;
            }

            if (this == &tr)
            {
                //returns this
                return *this;
            }
            if (tr.root != nullptr)
            {
                root = new Node{tr.root->val};
                tree_copy(root, tr.root);
            }
            return *this;
        }

        BinaryTree<T> &operator=(BinaryTree<T> &&tr) noexcept
        {
            // coppy stuff
            if (root)
            {
                delete root;
            }
            root = tr.root;
            tr.root = nullptr;
            return *this;
        }

        // noexcept copy
        BinaryTree(BinaryTree<T> &&tr) noexcept
        {
            this->root = tr.root;
            tr.root = nullptr;
        }

        BinaryTree<T>(const BinaryTree<T> &tr)
        {
            if (tr.root != nullptr)
            {
                this->root = new Node(tr.root->val);
                tree_copy(root, tr.root);
            }
        }

        ~BinaryTree<T>()
        {
            if (root)
            {
                for (auto iter = (*this).begin_postorder(); iter != (*this).end_postorder(); ++iter)
                {
                    delete iter.get_nd();
                }
            }
        }


        BinaryTree<T> &add_right(const T last, const T right_node) // add a right son to the last node
        {
            Node *node = searchNode(this->root, last);
            if (node == nullptr){
                throw std::invalid_argument("This node does not exist");
            }

            if (node->right == nullptr) //if right son doesnt exist we create new
            {
                (node->right) = new Node(right_node);
                node->right->father = node;
            }
            else
            {
                    node->right->val = right_node;
            }
                return *this;
        }
            

        BinaryTree<T> &add_left(const T last, const T left_node) // add a left son to the last node
        {
            Node *node = searchNode(this->root, last);
            if (node == nullptr){
                throw std::invalid_argument("This node does not exist");
            }

            if (node->left == nullptr) //if right son doesnt exist we create new
            {
                (node->left) = new Node(left_node);
                node->left->father = node;
            }
            else
            {
                    node->left->val = left_node;
            }
                return *this;
        }


        Node *searchNode(Node *point, T find) // find node find with start node. 
        {
            if (point == nullptr)
            {
                return nullptr;
            }
            for (auto iter = begin_inorder(); iter!= end_inorder(); ++iter)
            {
               if(*iter==find)
               {
                   return iter.get_nd();
               }
            }

            return nullptr;
        }

        // functions of binary tree

        iterator begin() { return iterator{root, inorder}; }
        
        iterator end() { return iterator{}; }

        iterator begin_preorder() { return iterator{root, preorder}; }

        iterator end_preorder() { return iterator{}; }

        iterator begin_inorder() { return iterator{root, inorder}; }

        iterator end_inorder() { return iterator{}; }

        iterator begin_postorder() { return iterator{root, postorder}; }

        iterator end_postorder() { return iterator{}; }

        friend std::ostream &operator<<(std::ostream &stream, const BinaryTree<T> &t)
         {
            stream << t.root->data;
            return stream; }
    };
}