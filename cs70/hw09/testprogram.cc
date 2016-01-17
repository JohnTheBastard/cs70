#include "chunkystring.hh"
#include "chunkystring.cc"
#include <cassert>
#include <iostream>

int main(int, char *[])
{
  ChunkyString foo;
  foo+='a';
  foo+='b';
  foo+='c';
  foo+='d';
  foo+='e';
  foo+='f';
  assert(foo.length() == 6);
  
  for (ChunkyStringIterator bar(foo); bar; ++bar)
    cout << *bar << endl;

  foo.print(cout);

  cout << endl;
  
  return 0;
}
