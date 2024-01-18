#include "reassembler.hh"

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring, Writer& output )
{
  
  // If this is the last substring, mark the reassembler as finished.
  if ( is_last_substring ) {
    is_finished_ = is_last_substring;
  }

  //drop 
  if (data.size() + first_index < stream_index_ && !is_last_substring) return;
  uint64_t start_idx_ = first_index;
  // overlap
 if (first_index < stream_index_ &&  first_index + data.size() > stream_index_) {
    data = data.substr( stream_index_ - first_index );
    start_idx_ = stream_index_; 
  }

  data = data.substr( 0, output.available_capacity() );
  
  if ( data.size() + bytes_pending_ > output.available_capacity() && !is_last_substring && start_idx_ > stream_index_)
  return;

  // write to assembler's buffer
  for(uint64_t i = start_idx_; i < start_idx_ + data.size(); i++){
    // avoid the overlapped buffer
    if(i < stream_index_) continue;
    auto result = buffer.emplace(i, data[i - start_idx_]);
    if(result.second) { // 如果插入成功，那么增加 bytes_pending_
      bytes_pending_ ++;
    }
  }

  while ( !buffer.empty() && !output.is_closed() && output.available_capacity() > 0 ) {
    auto it = buffer.find( stream_index_ );
    if ( it == buffer.end() ) {
      break;
    }
    
    output.push( string(1,it->second) );
    bytes_pending_ --;
    buffer.erase( it );
    stream_index_ ++;
    
    if (is_finished_ && buffer.empty()) {
      buffer.clear();
      output.close();
      return;
    }
  }
  if(stream_index_ != 0){
  std::cout<< " first_index " << first_index <<" buffer size: "<<buffer.size()<<" stream_index_ "<<stream_index_ <<" is_finished_ "<<is_finished_<<std::endl;
  }
  if ( is_finished_ && !buffer.size() ) {
    buffer.clear();
    output.close();
  }
}

uint64_t Reassembler::bytes_pending() const
{
  return bytes_pending_;
}
