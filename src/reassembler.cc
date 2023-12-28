#include "reassembler.hh"

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring, Writer& output )
{
  std::cout << "params " << data << " " << first_index << " " << is_last_substring << " " << stream_index_ << " "
            << buffer.size() << std::endl;

  // If this is the last substring, mark the reassembler as finished.
  if ( is_last_substring ) {
    is_finished_ = is_last_substring;
  }

  if ( first_index + data.size() > stream_index_) {
    data = data.substr( stream_index_ - first_index );
  }
  data = data.substr( 0, output.available_capacity() );

  if ( data.size() + bytes_pending_ > output.available_capacity() )
    return;

  if ( first_index + data.size() > stream_index_) {
    buffer[stream_index_] = data;
    bytes_pending_ += data.size();
  } 
  // else {
  //   if ( first_index + data.size() > stream_index_ ) {

  //     buffer[first_index] = data;
  //     bytes_pending_ += data.size();
  //   }
  // }

  std::cout << "capacity " << buffer.size() << " " << output.available_capacity() << std::endl;

  while ( !buffer.empty() && !output.is_closed() && output.available_capacity() > 0 ) {
    if ( buffer.find( stream_index_ ) == buffer.end() ) {
      break;
    }

    auto& str = buffer[stream_index_];
    auto pushlen = str.size();
    std::cout << "str " << str << std::endl;

    output.push( str );
    // output.push( str );
    // bytes_pending_ -= pushlen;
    bytes_pending_ -= pushlen;
    buffer.erase( stream_index_ );
    stream_index_ += pushlen;
  }
  if ( is_finished_ && !buffer.size() ) {
    output.close();
  }
}

uint64_t Reassembler::bytes_pending() const
{
  return bytes_pending_;
}
