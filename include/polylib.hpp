#ifndef POLYLIB_HPP_INCLUDED
#define POLYLIB_HPP_INCLUDED

#include <polylib/my-drive.hpp>
#include <vk/base-types.hpp>
#include <vk/bot.hpp>
#include <nlohmann/json.hpp>

namespace polylib
{
  class PolyLib
  {
  public:
    PolyLib(const polylib::MyDrive *drive, const vk::Bot *bot);
    ~PolyLib();
    void runBot() const;
    void handleUpdates(const nlohmann::json &updates) const;
    void handleMessage(const vk::message_t &message) const;
    vk::keyboard_t getKeyboardByPath(const std::string &path = "", const size_t offset = 0) const;

  private:
    const polylib::MyDrive *drive_;
    const vk::Bot *bot_;
  };
}

#endif
