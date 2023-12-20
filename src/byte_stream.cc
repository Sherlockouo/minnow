#include <stdexcept>

#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

void Writer::push( string data )
{
  // no capacity or data is empty
  if(available_capacity() == 0 || !data.size()) return;
  
  // capacity is smaller than data's length
  auto n = min(available_capacity(),data.size());
  if(n != data.size())
    data = data.substr(0,n);
  buffer.push(data);
  pushed_bytes += n;
  buffered_bytes += n;
}

void Writer::close()
{
  is_closed_ = true;
}

void Writer::set_error()
{
  has_error_ = true;
}

bool Writer::is_closed() const
{
  return is_closed_;
}

uint64_t Writer::available_capacity() const
{
  return capacity_ - buffered_bytes;
}

uint64_t Writer::bytes_pushed() const
{
  return pushed_bytes;
}

string_view Reader::peek() const
{
  if(buffer.empty()){
    return {};
  }
  string_view res = buffer.front();
  return res;
}

bool Reader::is_finished() const
{
  return is_closed_ && buffered_bytes == 0;
}

bool Reader::has_error() const
{
  return has_error_;
}

void Reader::pop( uint64_t len )
{
  auto poplen = min(len,buffered_bytes);
  while(poplen > 0){
    auto &front = buffer.front();
    if(front.size() > poplen){
      front = front.substr(poplen);
      poped_bytes += poplen;
      buffered_bytes -= poplen;
      break;
    }
    else{
      poplen -= front.size();
      poped_bytes += front.size();
      buffered_bytes -= front.size();
      buffer.pop();
    }
  }
}

uint64_t Reader::bytes_buffered() const
{
  return buffered_bytes;
}

uint64_t Reader::bytes_popped() const
{
 return poped_bytes;
}
