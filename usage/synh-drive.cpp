#include <iostream>
#include <stdexcept>
#include <polylib/my-drive.hpp>

int main()
{
  try
  {
    polylib::MyDrive drive("AQAAAAAoMACDAAUZU0a3G9cFJEVOhhJgEnDciLs");
    drive.synchronizeResources();
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}
