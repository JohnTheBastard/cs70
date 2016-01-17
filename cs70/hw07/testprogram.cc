#include "intlist.hh"
#include "intlist.cc"
#include <cassert>
#include <iostream>

int main(int, char *[])
{
  IntList foo;
  foo.pushHead(0);
  foo.pushTail(1);
  foo.pushTail(2);
  foo.pushTail(3);
  foo.pushHead(5);
  assert(foo.length() == 5);
  
  for (IntListIterator bar(foo); bar; ++bar)
    cout << *bar << endl;

  IntList bar(foo);

  assert(foo.peekHead() == 5);  
  assert(foo.popHead() == 5);
  assert(foo.peekHead() == 0);
  assert(foo.popHead() == 0);
  assert(foo.popHead() == 1);
  assert(foo.popHead() == 2);
  assert(foo.popHead() == 3);
  
  assert(foo.length() == 0);
  
  return 0;
}
