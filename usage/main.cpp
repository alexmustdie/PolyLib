#include <iostream>
#include <stdexcept>
#include <string>
#include <vk/bot.hpp>
#include <polylib/my-drive.hpp>
#include <polylib/polylib.hpp>

const std::string YANDEX_ACCESS_TOKEN = "AQAAAAAoMACDAAUZU0a3G9cFJEVOhhJgEnDciLs";
const std::string VK_ACCESS_TOKEN = "9b4cf3dc41341aebb446f108bed71a619a0cf2a64c5d2ba4226b937bc0d74de1df901b04f2317b00c3076";

int main()
{
  try
  {
    polylib::MyDrive drive(YANDEX_ACCESS_TOKEN);
    vk::Bot bot(VK_ACCESS_TOKEN);

    polylib::PolyLib polylib(&drive, &bot);
    polylib.runBot();
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}
