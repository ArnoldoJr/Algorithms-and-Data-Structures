/****************************************
 *  Copyright © 2018 Arnoldo Rodriguez  *
 ****************************************/
 
 // Signature type methods provided by Douglas W. Harder https://ece.uwaterloo.ca/~dwharder/
 
#include <iostream>

template <typename Type>
class Double_linked_list {
	public:
		class Double_node {
			public:
				Double_node( Type const & = Type(), Double_node * = nullptr, Double_node * = nullptr );

				Type value() const;
				Double_node *previous() const;
				Double_node *next() const;

				Type         node_value;
				Double_node *previous_node;
				Double_node *next_node;
		};

		Double_linked_list();
		Double_linked_list( Double_linked_list const & );
		Double_linked_list( Double_linked_list && );
		~Double_linked_list();

		// Accessors

		int size() const;
		bool empty() const;

		Type front() const;
		Type back() const;

		Double_node *begin() const;
		Double_node *end() const;
		Double_node *rbegin() const;
		Double_node *rend() const;

		Double_node *find( Type const & ) const;
		int count( Type const & ) const;

		// Mutators

		void swap( Double_linked_list & );
		Double_linked_list &operator=( Double_linked_list );
		Double_linked_list &operator=( Double_linked_list && );

		void push_front( Type const & );
		void push_back( Type const & );

		void pop_front();
		void pop_back();

		int erase( Type const & );

	private:
		Double_node *list_head;
		Double_node *list_tail;
		int list_size;

	// Friends

	template <typename T>
	friend std::ostream &operator<<( std::ostream &, Double_linked_list<T> const & );
    
    void initialize_list();
};

/////////////////////////////////////////////////////////////////////////
//                      Public member functions                        //
/////////////////////////////////////////////////////////////////////////

////////////////////// Default Constructor ///////////////////////////////////////
template <typename Type>
Double_linked_list<Type>::Double_linked_list():
list_head( new Double_node() ),
list_tail( new Double_node() ),
list_size( 0 )
{
    initialize_list();
}

////////////////////// Copy Constructor ///////////////////////////////////////
template <typename Type>
Double_linked_list<Type>::Double_linked_list( Double_linked_list<Type> const &list ):
list_head( new Double_node() ),
list_tail( new Double_node() ),
list_size( 0 )
{
    // Initializing sentinels for new list
    initialize_list();
    
    // Copy of the first node of the argument Linked List.
    Double_node* arg_current_node = list.list_head->next()->next();
    
    // Copying node by node into the new Linked List.
    while(arg_current_node->next() != nullptr)
    {
        push_back(arg_current_node->value());
        arg_current_node = arg_current_node->next();
    }
}

////////////////////// Move Constructor ///////////////////////////////////////
template <typename Type>
Double_linked_list<Type>::Double_linked_list( Double_linked_list<Type> &&list ):
list_head( new Double_node() ),
list_tail( new Double_node() ),
list_size( 0 )
{
    initialize_list();
    swap(list);
}
///////////////////////// Destructor //////////////////////////////////////////
template <typename Type>
Double_linked_list<Type>::~Double_linked_list()
{
    // Deleting all nodes first.
    while (!empty())
    {
        pop_front();
    }
    
    // Deleting sentinels.
    list_head->next_node->next_node = nullptr;
    list_tail->next_node->previous_node = nullptr;
    delete list_head->next();
    delete list_tail->next();
    
    // Deleting head an tail nodes.
    list_head->next_node = nullptr;
    list_tail->next_node = nullptr;
    delete list_head;
    delete list_tail;
}

// Returns the size of the Linked List.
template <typename Type>
int Double_linked_list<Type>::size() const
{
	return list_size;
}

// Returns true if list is empty.
template <typename Type>
bool Double_linked_list<Type>::empty() const
{
    if(list_size == 0){
        return true;
    }
    return false;
}

// This method returns the value of the first node in the Linked List.
template <typename Type>
Type Double_linked_list<Type>::front() const
{
    // Throw exception if Linked List is empty.
    if(empty()){
        throw underflow();
    }
    // If non-empty, return the value of the first node.
	return list_head->next()->next()->value();
}

// This method returns the value of the last node in the Linked List.
template <typename Type>
Type Double_linked_list<Type>::back() const
{
    // Throw exception if Linked List is empty.
    if(empty()){
        throw underflow();
    }
    // If non-empty, return the value of the last node.
	return list_tail->next()->previous()->value();
}

template <typename Type>
typename Double_linked_list<Type>::Double_node *Double_linked_list<Type>::begin() const
{
	return list_head->next()->next();
}

template <typename Type>
typename Double_linked_list<Type>::Double_node *Double_linked_list<Type>::end() const
{
	return list_tail->next();
}

template <typename Type>
typename Double_linked_list<Type>::Double_node *Double_linked_list<Type>::rbegin() const
{
	return list_tail->next()->previous();
}

template <typename Type>
typename Double_linked_list<Type>::Double_node *Double_linked_list<Type>::rend() const
{
	return list_head->next();
}

// This method returns the address of the first node whose value equals "obj".
template <typename Type>
typename Double_linked_list<Type>::Double_node *Double_linked_list<Type>::find( Type const &obj ) const
{
    Double_node* current_node = list_head->next()->next();
    while (current_node->next() != nullptr)
    {
        if(current_node->value() == obj)
        {
            return current_node;
        }
        current_node = current_node->next();
    }
    // None was found so return the address of tail sentinel.
	return end();
}

// This method returns the number of nodes in the Linked List whose value equals to "obj".
template <typename Type>
int Double_linked_list<Type>::count( Type const &obj ) const
{
    int total_matches = 0;
    Double_node* current_node = list_head->next()->next();
    
    while(current_node->next() != nullptr)
    {
        if(current_node->value() == obj)
        {
            total_matches++;
        }
        current_node = current_node->next();
    }
	return total_matches;
}

template <typename Type>
void Double_linked_list<Type>::swap( Double_linked_list<Type> &list ) {
	// This is done for you
	std::swap( list_head, list.list_head );
	std::swap( list_tail, list.list_tail );
    std::swap( list_size, list.list_size );
}

template <typename Type>
Double_linked_list<Type> &Double_linked_list<Type>::operator=( Double_linked_list<Type> rhs ) {
	// This is done for you
	swap( rhs );
    
	return *this;
}

template <typename Type>
Double_linked_list<Type> &Double_linked_list<Type>::operator=( Double_linked_list<Type> &&rhs ) {
	// This is done for you
	swap( rhs );

	return *this;
}

// This method adds a number to the front of the Linked List.
template <typename Type>
void Double_linked_list<Type>::push_front( Type const &obj ) {
    
    // Creating the new node and conecting it to the Linked List.
    Double_node* new_node = new Double_node(obj, list_head->next(), list_head->next()->next());
    list_head->next_node->next_node->previous_node = new_node;
    list_head->next_node->next_node = new_node;
    
    // Updating Linked List's fields.
    list_size++;
}

// This method adds a number to the back of the Linked List.
template <typename Type>
void Double_linked_list<Type>::push_back( Type const &obj ) {
    
    // Creating the new node and conecting it to the Linked List.
    Double_node* new_node = new Double_node(obj, list_tail->next()->previous(), list_tail->next());
    list_tail->next_node->previous_node->next_node = new_node;
    list_tail->next_node->previous_node = new_node;
    
    // Updating Linked List's fields.
    list_size++;
}

// This method removes the Linked List's first node.
template <typename Type>
void Double_linked_list<Type>::pop_front()
{
    // Throw exception if Linked List is empty.
    if(empty()){
        throw underflow();
        return;
    }
    
    Double_node* deleted_node = list_head->next()->next();
    
    // Disconnecting the node from the Linked List and deleting it.
    list_head->next_node->next_node = deleted_node->next();
    deleted_node->next_node->previous_node = list_head->next();
    delete deleted_node;
    
    // Updating Linked List's fields.
    list_size--;
}

// This method removes the Linked List's last node.
template <typename Type>
void Double_linked_list<Type>::pop_back()
{
    // Throw exception if Linked List is empty.
    if(empty()){
        throw underflow();
        return;
    }
    
    Double_node* garbage_node = list_tail->next()->previous();
    
    // Disconnecting the node from the Linked List and deleting it.
    list_tail->next_node->previous_node = garbage_node->previous();
    garbage_node->previous_node->next_node = list_tail->next();
    delete garbage_node;
    
    // Updating Linked List's fields.
    list_size--;
}

// This method deletes all Nodes whose value equals to "obj"
template <typename Type>
int Double_linked_list<Type>::erase( Type const &obj )
{
    // Getting the first node
    Double_node* current_node = list_head->next()->next();
    
    int deleted_nodes = 0;
    while (current_node->next() != nullptr)
    {
        if(current_node->value() == obj)
        {
            // Disconnecting the node from the Linked List and deleting it.
            current_node->previous_node->next_node = current_node->next();
            current_node->next_node->previous_node = current_node->previous();
            
            Double_node* garbage_node = current_node;
            current_node = current_node->next();
            delete garbage_node;
            
            // Updating Linked list fields
            deleted_nodes++;
            list_size--;
        }
        else
        {
            // Move on to next node in the Linked List
            current_node = current_node->next();
        }
    }
	return deleted_nodes;
}

/////////////////////////////////////////////////////////////////////////
//                      Nested Class Double_Node                       //
/////////////////////////////////////////////////////////////////////////

template <typename Type>
Double_linked_list<Type>::Double_node::Double_node(
	Type const &nv,
	typename Double_linked_list<Type>::Double_node *pn,
	typename Double_linked_list<Type>::Double_node *nn ):
// Updated the initialization list here
node_value( Type() ), // This assigns 'node_value' the default value of Type
previous_node( nullptr ),
next_node( nullptr )
{
    node_value = nv;
    previous_node = pn;
    next_node = nn;
}

template <typename Type>
Type Double_linked_list<Type>::Double_node::value() const
{
	return node_value;
}

template <typename Type>
typename Double_linked_list<Type>::Double_node *Double_linked_list<Type>::Double_node::previous() const
{
	return previous_node;
}

template <typename Type>
typename Double_linked_list<Type>::Double_node *Double_linked_list<Type>::Double_node::next() const
{
	return next_node;
}

/////////////////////////////////////////////////////////////////////////
//                      Private member functions                       //
/////////////////////////////////////////////////////////////////////////

// Some repeated code from the Double_linked_list Constructor.
// It initializes the head, tail and sentinel nodes.
template <typename Type>
void Double_linked_list<Type>::initialize_list()
{
    
    // Initializing sentinels
    Double_node* head_sentinel = new Double_node();
    Double_node* tail_sentinel = new Double_node();
    
    // Pointing head and tail to sentinels
    list_head->next_node = head_sentinel;
    list_tail->next_node = tail_sentinel;
    
    // Initializing head sentinel
    head_sentinel->next_node = tail_sentinel;
    head_sentinel->previous_node = nullptr;
    
    // Initializing tail sentinel
    tail_sentinel->previous_node = head_sentinel;
    tail_sentinel->next_node = nullptr;
}

/////////////////////////////////////////////////////////////////////////
//                               Friends                               //
/////////////////////////////////////////////////////////////////////////

template <typename T>
std::ostream &operator<<( std::ostream &out, Double_linked_list<T> const &list ) {
	out << "head";

	for ( typename Double_linked_list<T>::Double_node *ptr = list.rend(); ptr != nullptr; ptr = ptr->next() ) {
		if ( ptr == list.rend() || ptr == list.end() ) {
			out << "->S";
		} else {
			out << "->" << ptr->value();
		}
	}

	out << "->0" << std::endl << "tail";

	for ( typename Double_linked_list<T>::Double_node *ptr = list.end(); ptr != nullptr; ptr = ptr->previous() ) {
		if ( ptr == list.rend() || ptr == list.end() ) {
			out << "->S";
		} else {
			out << "->" << ptr->value();
		}
	}

	out << "->0";

	return out;
}
