#ifndef LINEAR_PROBING_H
#define LINEAR_PROBING_H

#include <vector>
#include <algorithm>
#include <functional>


namespace {

// Internal method to test if a positive number is prime.
bool IsPrimeLinear(size_t n) {      //had to change this by addin linear
  if( n == 2 || n == 3 )
    return true;
  
  if( n == 1 || n % 2 == 0 )
    return false;
  
  for( int i = 3; i * i <= n; i += 2 )
    if( n % i == 0 )
      return false;
  
  return true;
}


// Internal method to return a prime number at least as large as n.
int NextPrimeLinear(size_t n) {     //had to change this by adding linear
  if (n % 2 == 0)
    ++n;  
  while (!IsPrimeLinear(n)) n += 2;  //had to add Linear
  return n;
}

}  // namespace


// Quadratic probing implementation.
template <typename HashedObj>
class HashTableLinear {
 public:
  enum EntryType {ACTIVE, EMPTY, DELETED};

  explicit HashTableLinear(size_t size = 101) : array_(NextPrimeLinear(size))   //had to add Linear
    { MakeEmpty(); }
  
  bool Contains(const HashedObj & x) /*const*/ {// had to take out const for now 
    return IsActive(FindPos(x));
  }
  
  void MakeEmpty() {
    current_size_ = 0;
    for (auto &entry : array_)
      entry.info_ = EMPTY;
  }

  bool Insert(const HashedObj & x) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;
    
    array_[current_pos].element_ = x;
    array_[current_pos].info_ = ACTIVE;
    
    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();    
    return true;
  }
    
  bool Insert(HashedObj && x) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;
    
    array_[current_pos] = std::move(x);
    array_[current_pos].info_ = ACTIVE;

    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();

    return true;
  }

  bool Remove(const HashedObj & x) {
    size_t current_pos = FindPos(x);
    if (!IsActive(current_pos))
      return false;

    array_[current_pos].info_ = DELETED;
    return true;
  }

  //********************* Adding the following functions ***********************//////////
  int getCollisions() const{
    return collision_;
  }

  size_t getNumberOfItems() const{
    return current_size_;
  }

  int getSizeArray() const{
    return array_.size();
  }

  float loadFactor() const{
    return (float)(current_size_)/(float)(array_.size());
  } 

  float avgNumberOfCollisions() const{
    return (float)getCollisions()/(float)getNumberOfItems();
  }

  int getProbes() const{
    return probes_;
  }
  //_____--------_________------______ End 1 _____--------______-------______-----///////
 private:        
  struct HashEntry {
    HashedObj element_;
    EntryType info_;
    
    HashEntry(const HashedObj& e = HashedObj{}, EntryType i = EMPTY)
    :element_{e}, info_{i} { }
    
    HashEntry(HashedObj && e, EntryType i = EMPTY)
    :element_{std::move(e)}, info_{ i } {}
  };
    

  std::vector<HashEntry> array_;
  size_t current_size_;

  //added for collision
  int collision_=0;
  //added for number of probe
  int probes_=0;

  bool IsActive(size_t current_pos) const
  { return array_[current_pos].info_ == ACTIVE; }

  size_t FindPos(const HashedObj & x) /*const*/ {
    size_t offset = 1;
    size_t current_pos = InternalHash(x);
    
    //addng the following line to keep track of the number of probe
    probes_=1;
    while (array_[current_pos].info_ != EMPTY &&
	   array_[current_pos].element_ != x) {
      current_pos += offset;  // Compute ith probe.
      probes_++;             //  incrementing the probes
      collision_++;                               //added variable for collision
      //offset += 2;
      if (current_pos >= array_.size())
	current_pos -= array_.size();
    }
    return current_pos;
  }

  //Overrriding this to see something with contains, cuz it won't let me use the previous one since contains has to be const
   /* size_t FindPos(const HashedObj & x) const {
    size_t offset = 1;
    size_t current_pos = InternalHash(x);
      
    while (array_[current_pos].info_ != EMPTY &&
     array_[current_pos].element_ != x) {
      current_pos += offset;  // Compute ith probe.
      //collision_++;                               //added variable for collision
      offset += 2;
      if (current_pos >= array_.size())
  current_pos -= array_.size();
    }
    return current_pos;
  }*/

  void Rehash() {
    std::vector<HashEntry> old_array = array_;

    // Create new double-sized, empty table.
    array_.resize(NextPrimeLinear(2 * old_array.size())); //had to add Linear
    for (auto & entry : array_)
      entry.info_ = EMPTY;
    
    // Copy table over.
    current_size_ = 0;
    for (auto & entry :old_array)
      if (entry.info_ == ACTIVE)
	Insert(std::move(entry.element_));
  }
  
  size_t InternalHash(const HashedObj & x) const {
    static std::hash<HashedObj> hf;
    return hf(x) % array_.size( );
  }
};

#endif  // QUADRATIC_PROBING_H
