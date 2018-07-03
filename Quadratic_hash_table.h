/****************************************
 *  Copyright © 2018 Arnoldo Rodriguez  *
 ****************************************/
 // Signature type methods provided by Douglas W. Harder https://ece.uwaterloo.ca/~dwharder/
 
enum bin_state_t { UNOCCUPIED, OCCUPIED, ERASED };

template <typename Type>
class Quadratic_hash_table {
private:
    // Members
    int bins_occupied;                  // Current number of items with OCCUPIED state.
    int bins_erased;                    // Current umber of items with ERASED state.
    int power;
    int array_size;                     // Array size.
    int mask;
    Type *array;                        // Array that stores the values
    bin_state_t *occupied;              // Array that stores the current state of each bin.
    int hash( Type const & ) const;
    
public:
    // Constructor / Destructor
    Quadratic_hash_table( int = 5 );
    ~Quadratic_hash_table();
    
    // Accessors
    int size() const;
    int capacity() const;
    double load_factor() const;
    bool empty() const;
    bool member( Type const & ) const;
    Type bin( int ) const;
    void print() const;
    
    // Mutators
    void insert( Type const & );
    bool erase( Type const & );
    void clear();
    
    // Friends
    template <typename T>
    friend std::ostream &operator<<( std::ostream &, Quadratic_hash_table<T> const & );
    
};

//////////////////////////////////////////////////////////////////////
//                    Constructor & Cestructor                      //
//////////////////////////////////////////////////////////////////////

// Constructor
template <typename Type>
Quadratic_hash_table<Type>::Quadratic_hash_table( int m ):
bins_occupied( 0 ),
bins_erased( 0 ),
power( m ),
array_size( 1 << power ),
mask( array_size - 1 ),
array( new Type[array_size] ),
occupied( new bin_state_t[array_size] ) {
    
    for ( int i = 0; i < array_size; ++i ) {
        occupied[i] = UNOCCUPIED;
    }
}

// Destructor
template <typename Type>
Quadratic_hash_table<Type>::~Quadratic_hash_table() {
    clear();         // Deleting hash table content
    delete[] array;
    delete[] occupied;
}

//////////////////////////////////////////////////////////////////////
//                             Accessors                            //
//////////////////////////////////////////////////////////////////////

// Hashes objects
template <typename Type>
int Quadratic_hash_table<Type>::hash(Type const &obj) const{
    int i = static_cast<int>(obj);
    int hash_value = (i&mask);
    if(hash_value < 0){
        return hash_value + array_size;
    }
    return hash_value;
}


// Returns the number of elements currently stored in the hash table.
template <typename Type>
int Quadratic_hash_table<Type>::size() const{
    return bins_occupied;
}

// Returns the number of bins in the hash table.
template <typename Type>
int Quadratic_hash_table<Type>::capacity() const{
    return array_size;
}

// Returns the load factor of hash table
template <typename Type>
double Quadratic_hash_table<Type>::load_factor() const{
    return (static_cast<double>(bins_occupied) + static_cast<double>(bins_erased)) / static_cast<double>(capacity());
}

// Returns true if the hash table is empty, false otherwise.
template <typename Type>
bool Quadratic_hash_table<Type>::empty() const{
    return (bins_occupied!=0)?0:1;
}

// Returns true if object obj is in the hash table and false otherwise.
template <typename Type>
bool Quadratic_hash_table<Type>::member(Type const &obj ) const{
    // The initial vlaue we will be looking at.
    int index = hash(obj);
    
    // Iterate a number of times equals to "count".
    for (int i = 1; i <= capacity(); i++) {
        
        // Return false if an empty bin is found
        if (occupied[index] == UNOCCUPIED) {
            return false;
        }
        
        // Return true if an occupaied bin with the same value is found.
        if (array[index] == obj && occupied[index] == OCCUPIED) {
            return true;
        }
        
        // Set the index to the next value.
        index = (index + i)&mask;
    }
    
    // The object was not found after iterating throughout the entire hash table
    return false;
}


// Prints the contents of the class
template <typename Type>
void Quadratic_hash_table<Type>::print() const{
    // For testing purposes. Loop array and print all values..
    for (int i = 0; i < capacity(); i++) {
        std::cout << i << ": " << array[i] << std::endl;
    }
    std::cout << std::endl;
}

// Returns the entry in bin n
template <typename Type>
Type Quadratic_hash_table<Type>::bin( int n ) const {
    return array[n];
}

//////////////////////////////////////////////////////////////////////
//                             Mutators                             //
//////////////////////////////////////////////////////////////////////

// Inserts the argument into the hash table.
template <typename Type>
void Quadratic_hash_table<Type>::insert( Type const &obj ){
    
    // Throw overflow exception if the array is full;
    if( size() == capacity()){
        throw overflow();
    }
    
    int index = hash(obj);
    
    // Looping through the entire has table
    if (!member(obj)) {
        for (int i = 1; i <= capacity(); i ++) {
            if (occupied[index] == UNOCCUPIED) {
                array[index] = obj;
                
                 // Updating member variables
                occupied[index] = OCCUPIED;
                ++bins_occupied;
                return;
            }
            if (occupied[index] == ERASED) {
                array[index] = obj;
                
                // Updating member variables
                occupied[index] = OCCUPIED;
                --bins_erased;
                ++bins_occupied;
                return;
            }
            // Set the index to the next value.
            index = (index + i)&mask;
        }
    }
}

// Remove the argument from the hash table
template <typename Type>
bool Quadratic_hash_table<Type>::erase( Type const &obj ){
    int index = hash(obj);
    for (int i = 1; i <= capacity(); i++) {
        if (array[index] == obj && occupied[index] == OCCUPIED) {
            
            // Setting the flag of the "deleted" bin to ERASED.
            occupied[index] = ERASED;
            
            // Updating member variables.
            ++bins_erased;
            --bins_occupied;
            return true;
        }
        if (occupied[index] == UNOCCUPIED) {
            
            // An unoccupied bin was found so return false.
            return false;
        }
        // Set the index to the next value.
        index = (index + i)&mask;
    }
    return false;
}

// Removes all the elements in the hash table
template <typename Type>
void Quadratic_hash_table<Type>::clear(){
    
    // Setting all values in array to UNOCCUPIED state.
    for (int i = 0; i< capacity(); i++) {
        occupied[i] = UNOCCUPIED;
    }
    
    // Updating member variables.
    bins_occupied = 0;
    bins_erased = 0;
}


template <typename T>
std::ostream &operator<<( std::ostream &out, Quadratic_hash_table<T> const &hash ) {
    for ( int i = 0; i < hash.capacity(); ++i ) {
        if ( hash.occupied[i] == UNOCCUPIED ) {
            out << "- ";
        } else if ( hash.occupied[i] == ERASED ) {
            out << "x ";
        } else {
            out << hash.array[i] << ' ';
        }
    }
    return out;
}

#endif
