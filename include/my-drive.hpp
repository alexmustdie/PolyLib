#ifndef MY_DRIVE_HPP_INCLUDED
#define MY_DRIVE_HPP_INCLUDED

#include <yandex/base-types.hpp>
#include <yandex/drive.hpp>

namespace polylib
{
  class MyDrive:
    public yandex::Drive
  {
  public:
    MyDrive(const std::string &access_token);
    std::vector<yandex::resource_t> getResources(const std::string &path, const size_t limit, const size_t offset) const;
    size_t getResourcesCount(const std::string &path) const;
    yandex::resource_t getResourceById(const std::string &id) const;
    void synchronizeResources(const std::string &path = "disk:/") const;
    std::string getFileLink(const std::string &id) const;
  };
}

#endif
