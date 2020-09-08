#include "locked.hpp"

int main()
{
  locked<int> i(5);
  i.mut([](int& i){ i++; });
  return i.get() == 6 ? 0 : 1;
}
