/****************************************
 *  Copyright © 2018 Arnoldo Rodriguez  *
 ****************************************/
 
 // Signature type methods provided by Douglas W. Harder https://ece.uwaterloo.ca/~dwharder/

#include <cassert>

template <typename Type>
class AVL_tree {
public:
    class Iterator;
    
private:
    class Node {
    public:
        Type node_value;
        int tree_height;
        
        // The left and right sub-trees
        Node *left_tree;
        Node *right_tree;
        
        // Hint as to how you can create your iterator
        // Point to the previous and next nodes in linear order
        Node *previous_node;
        Node *next_node;
        
        // Member functions
        Node( Type const & = Type() );
        
        void update_height();
        
        int height() const;
        bool is_leaf() const;
        Node *front();
        Node *back();
        Node *find( Type const &obj );
        
        void clear();
        bool insert( Type const &obj, Node *&to_this );
        bool erase( Type const &obj, Node *&to_this );
        
        // Added this member functions for tree balancing purposes.
        bool check_balance( Node *&root );      // This method checks if a node is balanced.
        int height_difference();                // This method returns the height difference between right and left node.
        // Four cases of balancing.
        void case_1_left(Node *&root);
        void case_1_right(Node *&root);
        void case_2_left(Node *&root);
        void case_2_right(Node *&root);
        
        //friend function
        void print( std::ostream &out, int tabs = 0);
    };
    
    Node *root_node;
    int tree_size;
    
    // Hint as to how to start your linked list of the nodes in order
    Node *front_sentinel;
    Node *back_sentinel;
    
public:
    class Iterator {
    private:
        AVL_tree *containing_tree;
        Node *current_node;
        bool is_end;
        
        // The constructor is private so that only the search tree can create an iterator
        Iterator( AVL_tree *tree, Node *starting_node );
        
    public:
        // DO NOT CHANGE THE SIGNATURES FOR ANY OF THESE
        Type operator*() const;
        Iterator &operator++();
        Iterator &operator--();
        bool operator==( Iterator const &rhs ) const;
        bool operator!=( Iterator const &rhs ) const;
        
        // Make the search tree a friend so that it can call the constructor
        friend class AVL_tree;
    };
    
    // DO NOT CHANGE THE SIGNATURES FOR ANY OF THESE
    AVL_tree();
    ~AVL_tree();
    
    bool empty() const;
    int size() const;
    int height() const;
    
    Type front() const;
    Type back() const;
    
    Iterator begin();
    Iterator end();
    Iterator rbegin();
    Iterator rend();
    Iterator find( Type const & );
    
    void clear();
    bool insert( Type const & );
    bool erase( Type const & );
    
    // Friends
    
    template <typename T>
    friend std::ostream &operator<<( std::ostream &, AVL_tree<T> const & );
};

//////////////////////////////////////////////////////////////////////
//                Search Tree Public Member Functions               //
//////////////////////////////////////////////////////////////////////

template <typename Type>
AVL_tree<Type>::AVL_tree():
root_node( nullptr ),
tree_size( 0 ),
front_sentinel( new AVL_tree::Node( Type() ) ),
back_sentinel( new AVL_tree::Node( Type() ) ) {
    front_sentinel->next_node = back_sentinel;
    back_sentinel->previous_node = front_sentinel;
}

template <typename Type>
AVL_tree<Type>::~AVL_tree() {
    clear();  // might as well use it...
    delete front_sentinel;
    delete back_sentinel;
}

template <typename Type>
bool AVL_tree<Type>::empty() const {
    return ( root_node == nullptr );
}

template <typename Type>
int AVL_tree<Type>::size() const {
    return tree_size;
}

template <typename Type>
int AVL_tree<Type>::height() const {
    return root_node->height();
}

/* Returns the value of the front node in the linked list.
 * (i.e. The left most node in the tree.) */
template <typename Type>
Type AVL_tree<Type>::front() const {
    if ( empty() ) {
        throw underflow();
    }
    
    return root_node->front()->node_value;
}
/* Returns the value of the back node in the linked list.
 * (i.e. The right most node in the tree.) */
template <typename Type>
Type AVL_tree<Type>::back() const {
    if ( empty() ) {
        throw underflow();
    }
    
    return root_node->back()->node_value;
}

/* This method returns an iterator whose tree is the current serach_tree and
 * the current node is the smallest node.*/
template <typename Type>
typename AVL_tree<Type>::Iterator AVL_tree<Type>::begin() {
    return empty() ? Iterator( this, back_sentinel ) : Iterator( this, root_node->front() );
}

/* This method returns an iterator whose tree is the current serach_tree and
 * the current node is the back_sentinel node.*/
template <typename Type>
typename AVL_tree<Type>::Iterator AVL_tree<Type>::end() {
    return Iterator( this, back_sentinel );
}

/* This method returns an iterator whose tree is the current serach_tree and
 * the current node is the highest node.*/
template <typename Type>
typename AVL_tree<Type>::Iterator AVL_tree<Type>::rbegin() {
    return empty() ? Iterator( this, front_sentinel ) : Iterator( this, root_node->back() );
}

/* This method returns an iterator whose tree is the current serach_tree and
* the current node is the front_sentinel node.*/
template <typename Type>
typename AVL_tree<Type>::Iterator AVL_tree<Type>::rend() {
    return Iterator( this, front_sentinel );
}

template <typename Type>
typename AVL_tree<Type>::Iterator AVL_tree<Type>::find( Type const &obj ) {
    if ( empty() ) {
        return Iterator( this, back_sentinel );
    }
    
    typename AVL_tree<Type>::Node *search_result = root_node->find( obj );
    
    if ( search_result == nullptr ) {
        return Iterator( this, back_sentinel );
    } else {
        return Iterator( this, search_result );
    }
}

template <typename Type>
void AVL_tree<Type>::clear() {
    if ( !empty() ) {
        root_node->clear();
        root_node = nullptr;
        tree_size = 0;
    }
    
    // Reinitialize the sentinels
    front_sentinel->next_node = back_sentinel;
    back_sentinel->previous_node = front_sentinel;
}

// This method inserts a node in a tree. If the node already exists it returns false.
template <typename Type>
bool AVL_tree<Type>::insert( Type const &obj ) {
    if ( empty() ) {
        root_node = new AVL_tree::Node( obj );
        tree_size = 1;
        // Added this code to update next and previous nodes.
        root_node->previous_node = front_sentinel;
        root_node->next_node = back_sentinel;
        front_sentinel->next_node = root_node;
        back_sentinel->previous_node = root_node;
        
        return true;
    } else if ( root_node->insert( obj, root_node ) ) {
        ++tree_size;
        return true;
    } else {
        return false;
    }
}

// This method erases a node in a tree. If the node doesn't exist it returns false.
template <typename Type>
bool AVL_tree<Type>::erase( Type const &obj ) {
    if ( !empty() && root_node->erase( obj, root_node ) ) {
        --tree_size;
        return true;
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////
//                   Node Public Member Functions                   //
//////////////////////////////////////////////////////////////////////

template <typename Type>
AVL_tree<Type>::Node::Node( Type const &obj ):
node_value( obj ),
left_tree( nullptr ),
right_tree( nullptr ),
next_node( nullptr ),
previous_node( nullptr ),
tree_height( 0 ) {
    // does nothing
}

/* This method updates the height of a node by adding 1 to the highest height between the
 * left and right trees. */
template <typename Type>
void AVL_tree<Type>::Node::update_height() {
    tree_height = std::max( left_tree->height(), right_tree->height() ) + 1;
}

// This method returns the height of a node.
template <typename Type>
int AVL_tree<Type>::Node::height() const {
    return ( this == nullptr ) ? -1 : tree_height;
}

// Return true if the current node is a leaf node, false otherwise
template <typename Type>
bool AVL_tree<Type>::Node::is_leaf() const {
    return ( (left_tree == nullptr) && (right_tree == nullptr) );
}

/* Return a pointer to the front node.
 * Recursive method that finds the left-most node in the tree. */
template <typename Type>
typename AVL_tree<Type>::Node *AVL_tree<Type>::Node::front() {
    return ( left_tree == nullptr ) ? this : left_tree->front();
}

/* Return a pointer to the back node
 * Recursive method that finds the right-most node in the tree. */
template <typename Type>
typename AVL_tree<Type>::Node *AVL_tree<Type>::Node::back() {
    return ( right_tree == nullptr ) ? this : right_tree->back();
}

template <typename Type>
typename AVL_tree<Type>::Node *AVL_tree<Type>::Node::find( Type const &obj ) {
    if ( obj == node_value ) {
        return this;
    } else if ( obj < node_value ) {
        return (left_tree == nullptr) ? nullptr : left_tree->find( obj );
    } else {
        return ( right_tree == nullptr ) ? nullptr : right_tree->find( obj );
    }
}

// Recursively clear the tree
template <typename Type>
void AVL_tree<Type>::Node::clear() {
    if ( left_tree != nullptr ) {
        left_tree->clear();
    }
    
    if ( right_tree != nullptr ) {
        right_tree->clear();
    }
    
    delete this;
}

template <typename Type>
bool AVL_tree<Type>::Node::insert( Type const &obj, AVL_tree<Type>::Node *&to_this) {
    // Go to left side if the node_value is lower than the current node
    if ( obj < node_value ) {
        if ( left_tree == nullptr ) {
            left_tree = new AVL_tree<Type>::Node( obj );
            
            // Updating previous and next nodes.
            previous_node->next_node = left_tree;
            left_tree->previous_node = previous_node;
            left_tree->next_node = to_this;
            previous_node = left_tree;
            
            int prev_height = height();
            update_height();
            
            // Checking whether we need to check if the tree is unbalanced.
            if(prev_height != height()){
                check_balance(to_this);
            }
            return true;
        }
        else {
            if ( left_tree->insert( obj, left_tree ) ) {
                
                int prev_height = height();
                update_height();
                
                // Checking whether we need to check if the tree is unbalanced.
                if(prev_height != height()){
                    check_balance(to_this);
                }
                return true;
            }
            else {
                return false;
            }
        }
    }
    // Go to right side if the node_value is higher than the current node
    else if ( obj > node_value ) {
        if ( right_tree == nullptr ) {
            right_tree = new AVL_tree<Type>::Node( obj );
            
            // Updating previous and next nodes.
            next_node->previous_node = right_tree;
            right_tree->next_node = next_node;
            right_tree->previous_node = to_this;
            next_node = right_tree;
            
            int prev_height = height();
            update_height();
            
            // Checking whether we need to check if the tree is unbalanced.
            if(prev_height != height()){
                check_balance(to_this);
            }
            return true;
            
        } else {
            if ( right_tree->insert( obj, right_tree ) ) {
                int prev_height = height();
                update_height();
                
                // Checking whether we need to check if the tree is unbalanced.
                if(prev_height != height()){
                    check_balance(to_this);
                }
                return true;
            }
            else {
                return false;
            }
        }
    }
    // (obj == node_value)
    else {
        // The node that was going to be inserted already exists.
        return false;
    }
}

/* This method erases a node in the tree. */
template <typename Type>
bool AVL_tree<Type>::Node::erase( Type const &obj, AVL_tree<Type>::Node *&to_this ) {
    if ( obj < node_value ) {
        if ( left_tree == nullptr ) {
            return false;
        } else {
            if ( left_tree->erase( obj, left_tree ) ) {
                update_height();
                
                // Checking whether the tree is balanced at this node.
                check_balance(to_this);
                
                return true;
            }
            return false;
        }
    } else if ( obj > node_value ) {
        if ( right_tree == nullptr ) {
            return false;
        } else {
            if ( right_tree->erase( obj, right_tree ) ) {
                update_height();
                
                // Checking whether the tree is balanced at this node.
                check_balance(to_this);
                
                return true;
            }
            return false;
        }
    } else {
        assert( obj == node_value );
        
        if ( is_leaf() ) {
            // Updating the previous and next nodes
            previous_node->next_node = next_node;
            next_node->previous_node = previous_node;
            
            // Deleting the node
            to_this = nullptr;
            delete this;
        }
        else if ( left_tree == nullptr ) {
            // Updating the previous and next nodes
            previous_node->next_node = next_node;
            next_node->previous_node = previous_node;
            
            // Deleting the node
            to_this = right_tree;
            delete this;
        }
        else if ( right_tree == nullptr ) {
            // Updating the previous and next nodes
            previous_node->next_node = next_node;
            next_node->previous_node = previous_node;
            
            // Deleting the node
            to_this = left_tree;
            delete this;
        }
        /* Set the current node to be the next highest node and delete that
         * node from the right hand side of the tree (since that's where the
         * next highest node is) */
        else {
            // Before we take the lower or higher node we check whether the left_tree is
            // higher than the right_tree
            int diff = height_difference();
            if (diff > 0) {
                node_value = left_tree->back()->node_value;
                left_tree->erase(node_value, left_tree);
                update_height();
            }
            else{
                node_value = right_tree->front()->node_value;
                right_tree->erase( node_value, right_tree );
                update_height();
            }
            
            // Checking whether the tree is balanced at this node.
            check_balance(to_this);
        }
        return true;
    }
}

// Helper functions

/* This method checks whether the tree is balanced or not. If the tree is indeed unbalanced,
 * this method balances it according to the type of unbalancement. */
template <typename Type>
bool AVL_tree<Type>::Node::check_balance( AVL_tree<Type>::Node *&root ) {
    int diff = height_difference();
    
    // The left_tree is higher than the right_tree
    if (diff > 1) {
        // Case 1
        if(left_tree->height_difference() > 0){
            case_1_left(root);
        }
        // Case 2
        else if(left_tree->height_difference() < 0){
            case_2_left(root);
        }
        return true;
    }
    // The right_tree is higher than the left_tree
    else if (diff < -1){
        
        // Case 1
        if(right_tree->height_difference() < 0){
            case_1_right(root);
        }
        // Case 2
        else if(right_tree->height_difference() > 0){
            case_2_right(root);
        }
        return true;
    }
    return false;
}

// This method returns the height difference between the right and left tree of a given node,
template <typename Type>
int AVL_tree<Type>::Node::height_difference(){
    int a = -1;
    int b = -1;
    if(left_tree != nullptr){
        a = left_tree->tree_height;
    }
    if(right_tree != nullptr){
        b = right_tree->tree_height;
    }
    return (a-b);
}


template <typename Type>
void AVL_tree<Type>::Node::case_1_left( AVL_tree<Type>::Node *&root ){
    
    // Doing the node rotation
    Node *temp = left_tree;
    left_tree = left_tree->right_tree;
    temp->right_tree = this;
    
    // Updating the heights of the nodes changed starting in teh lower ones.
    temp->right_tree->update_height();
    temp->left_tree->update_height();
    temp->update_height();
    
    root = temp;
    
    // Deleting the temporary node.
    temp = nullptr;
    delete temp;
}

template <typename Type>
void AVL_tree<Type>::Node::case_1_right( AVL_tree<Type>::Node *&root ){
    
    // Doing the node rotation
    Node *temp = right_tree;
    right_tree = right_tree->left_tree;
    temp->left_tree = this;
    
    // Updating the heights of the nodes changed starting in teh lower ones.
    temp->right_tree->update_height();
    temp->left_tree->update_height();
    temp->update_height();
    
    root = temp;
    
    // Deleting the temporary node.
    temp = nullptr;
    delete temp;
}

template <typename Type>
void AVL_tree<Type>::Node::case_2_left( AVL_tree<Type>::Node *&root ){
    
    // Doing the node rotation
    Node *temp = left_tree->right_tree;
    left_tree->right_tree = temp->left_tree;
    temp->left_tree = left_tree;
    left_tree = temp->right_tree;
    temp->right_tree = this;
    
    // Updating the heights of the nodes changed starting in teh lower ones.
    temp->right_tree->update_height();
    temp->left_tree->update_height();
    temp->update_height();
    
    root = temp;
    
    // Deleting the temporary node.
    temp = nullptr;
    delete temp;
}

template <typename Type>
void AVL_tree<Type>::Node::case_2_right( AVL_tree<Type>::Node *&root ){
    
    // Doing the node rotation
    Node *temp = right_tree->left_tree;
    right_tree->left_tree = temp->right_tree;
    temp->right_tree = right_tree;
    right_tree = temp->left_tree;
    temp->left_tree = this;
    
    // Updating the heights of the nodes changed starting in teh lower ones.
    temp->right_tree->update_height();
    temp->left_tree->update_height();
    temp->update_height();
    
    root = temp;
    
    // Deleting the temporary node.
    temp = nullptr;
    delete temp;
}

//////////////////////////////////////////////////////////////////////
//                   Iterator Private Constructor                   //
//////////////////////////////////////////////////////////////////////

template <typename Type>
AVL_tree<Type>::Iterator::Iterator( AVL_tree<Type> *tree, typename AVL_tree<Type>::Node *starting_node ):
containing_tree( tree ),
current_node( starting_node ) {

}

//////////////////////////////////////////////////////////////////////
//                 Iterator Public Member Functions                 //
//////////////////////////////////////////////////////////////////////

template <typename Type>
Type AVL_tree<Type>::Iterator::operator*() const {
    // This is done for you...
    return current_node->node_value;
}

template <typename Type>
typename AVL_tree<Type>::Iterator &AVL_tree<Type>::Iterator::operator++() {
    // Update the current node to the node containing the next higher value
    // If we are already at end do nothing
    
    // Your implementation here, do not change the return value
    if(current_node->next_node != nullptr){
       current_node = current_node->next_node;
    }
    return *this;
}

template <typename Type>
typename AVL_tree<Type>::Iterator &AVL_tree<Type>::Iterator::operator--() {
    // Update the current node to the node containing the next smaller value
    // If we are already at either rend, do nothing
    
    // Your implementation here, do not change the return value
    if(current_node->previous_node != nullptr){
        current_node = current_node->previous_node;
    }
    
    return *this;
}

template <typename Type>
bool AVL_tree<Type>::Iterator::operator==( typename AVL_tree<Type>::Iterator const &rhs ) const {
    return ( current_node == rhs.current_node );
}

template <typename Type>
bool AVL_tree<Type>::Iterator::operator!=( typename AVL_tree<Type>::Iterator const &rhs ) const {
    return ( current_node != rhs.current_node );
}

//////////////////////////////////////////////////////////////////////
//                            Friends                               //
//////////////////////////////////////////////////////////////////////
template <typename Type>
void AVL_tree<Type>::Node::print( std::ostream &out, int tabs ) {
    for(int i = 0; i < tabs; i++){
        out << " ";
    }
    out << (tabs ? "`==" : "");
    if(this == nullptr){
        out << "xx";
        out << " h=-1" << std::endl;
    } else {
        out << node_value;
        out << " h=" << height() << std::endl;
        left_tree->print(out, tabs+1);
        right_tree->print(out, tabs+1);
    }
}

template <typename T>
std::ostream &operator<<( std::ostream &out, AVL_tree<T> const &list ) {
    list.root_node->print(out);
    return out;
}

