#pragma once
#include <atomic>
//
// INVARIANT: w_deleted_count <= w_deleting_count <= w_cached_read_count <= shared_read_count <= r_reading_count <= r_cached_write_count <= shared_write_count <= w_writing_count <= w_deleted_count + MAX_SIZE
//
// INVARIANT:      w_writing_count      - w_deleted_count     <= MAX_SIZE
// =========>      w_writing_count      - w_cached_read_count <= MAX_SIZE
// =========>      shared_write_count   - w_cached_read_count <= MAX_SIZE
// =========>      shared_write_count   - shared_read_count   <= MAX_SIZE
//
//
// INVARIANT: 0 <= r_cached_write_count - r_reading_count
// =========> 0 <= r_cached_write_count - shared_read_count
// =========> 0 <= shared_write_count   - shared_read_count
//
//
// THEOREM: =========> 0 <= shared_write_count   - shared_read_count   <= MAX_SIZE
//





//
//
// Channel/Queue:
//
// Shared between Producer and Consumer
//
struct channel_t{
  public:

    //insert your code here
    std::atomic<int> shared_read_count;
    std::atomic<int> shared_write_count;
  public:

    //
    // Intialize
    //
    channel_t(){

      // insert your code here
      shared_read_count = 0;
      shared_write_count = 0;
    }
};


//
// Producer's (not shared)
//
struct writeport_t{
public:
    //insert your code here
    int MAXSIZE;
    int w_deleted_count;
    int w_cached_read_count;
    int w_writing_count;
    int w_deleting_count;
    
public:

  //
  // Intialize
  //
  writeport_t(size_t tsize)
  {
    //insert code here
    MAXSIZE = tsize;
    w_deleting_count = 0;
    w_deleted_count = 0;
    w_writing_count = 0;
    w_cached_read_count = 0;
  }

public:

  //
  // no of entries available to write
  //
  // helper function for write_canreserve
  //
  size_t write_reservesize(){

    // insert your code here
    return MAXSIZE - (w_writing_count-w_deleting_count);
  }

  //
  // Can write 'n' entries?
  //
  bool write_canreserve(size_t n){

    // insert your code here
    return n <= write_reservesize();
  }

  //
  // Reserve 'n' entries for write
  //
  size_t write_reserve(size_t n){
    // insert your code here
    int tmp = w_writing_count;
    w_writing_count += n;
    return (size_t)tmp;
  }

  //
  // Commit
  //
  // Read/Write shared memory data structure
  //
  void write_release(channel_t& ch){

    // insert your code here
    ch.shared_write_count = w_writing_count;
  }




public:

  //
  //
  // Read/Write shared memory data structure
  //
  void read_acquire(channel_t& ch){

    //insert your code here
    w_cached_read_count = ch.shared_read_count;
  }




  //
  // No of entires available to delete
  //
  size_t delete_reservesize(){
    //insert your code here
    return w_cached_read_count - w_deleted_count;
  }

  //
  // Can delete 'n' entires?
  //
  bool delete_canreserve(size_t n){
    //insert your code here

    return n <= delete_reservesize();
  }

  //
  // Reserve 'n' entires for deletion
  //
  size_t delete_reserve(size_t n){
    //insert your code here
    int tmp = w_deleting_count;
    w_deleting_count += n;
    return (size_t)tmp;
  }


  //
  // Update the state, if any.
  //
  void delete_release(){
    //insert your code here
    w_deleted_count = w_deleting_count;
  }


};


//
// Consumer's (not shared)
//
//
struct readport_t{
public:

  //insert your code here
  int MAXSIZE;
  int r_reading_count;
  int r_cached_write_count;

public:
  //
  // Initialize
  //
  readport_t(size_t tsize)
  {

    //insert your code here
    MAXSIZE = tsize;
    r_reading_count = 0;
    r_cached_write_count = 0;
  }
  public:

  //
  // Read/Write shared memory data structure
  //
  void write_acquire(channel_t& ch){

    //insert your code here
    r_cached_write_count = ch.shared_write_count;
  }

  //
  // no of entries available to read
  //
  size_t read_reservesize(){

    //insert your code here
    return r_cached_write_count - r_reading_count;
  }

  //
  // Can Read 'n' entires?
  //
  bool read_canreserve(size_t n){

    //insert your code here
    return n <= read_reservesize();
  }

  //
  // Reserve 'n' entires to be read
  //
  size_t read_reserve(size_t n){

    //insert your code here
    int tmp = r_reading_count;
    r_reading_count += n;
    return (size_t)tmp;
  }

  //
  // Read/write shared memory data structure
  //
  void read_release(channel_t& ch){

    //insert your code here
    ch.shared_read_count = r_reading_count;
  }

};


