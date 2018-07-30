#include <polylib/my-drive.hpp>
#include <polylib/db.hpp>
#include <pqxx/pqxx>
#include <iostream>

using namespace polylib;
using namespace yandex;
using namespace pqxx;
using namespace std;

MyDrive::MyDrive(const string &access_token):
  Drive(access_token)
{}

vector<resource_t> MyDrive::getResources(const string &path, const size_t limit, const size_t offset) const
{
  vector<resource_t> resources;

  result res = DB::get()->exec("SELECT * FROM resources WHERE path='" + path + "' ORDER BY type ASC LIMIT " + to_string(limit) + " OFFSET " + to_string(offset));

  for (auto resource: res)
  {
    resources.push_back({
      resource["id"].as<string>(),
      resource["path"].as<string>(),
      resource["type"].as<string>(),
      resource["name"].as<string>()
    });
  }

  return resources;
}

size_t MyDrive::getResourcesCount(const string &path) const
{
  result res = DB::get()->exec("SELECT COUNT(*) FROM resources WHERE path='" + path + "'");
  return res[0][0].as<size_t>();
}

resource_t MyDrive::getResourceById(const std::string &id) const
{
  result res = DB::get()->exec("SELECT * FROM resources WHERE id='" + id + "'");

  return {
    res[0]["id"].as<string>(),
    res[0]["path"].as<string>(),
    res[0]["type"].as<string>(),
    res[0]["name"].as<string>()
  };
}

void MyDrive::synchronizeResources(const string &path) const
{
  vector<resource_t> resources = Drive::getResources(path);
  string new_path;

  for (resource_t resource: resources)
  {
    new_path = (path == "disk:/") ? "" : path.substr(5);

    if ((DB::get()->exec("SELECT * FROM resources WHERE id='" + resource.id + "'")).empty())
    {
      DB::get()->exec("INSERT INTO resources(id, path, type, name) "
        "VALUES ("
        + DB::get()->quote(resource.id) + ","
        + DB::get()->quote(new_path) + ","
        + DB::get()->quote(resource.type) + ","
        + DB::get()->quote(resource.name) + ")");

      if (resource.type == "file")
      {
        cout << resource.path << endl;
        
        size_t price;
        cin >> price;
        
        DB::get()->exec("INSERT INTO price_list(file_id, price) VALUES ("
          + DB::get()->quote(resource.id) + ","
          + DB::get()->quote(price) + ")");
      }
    }
    else
    {
      DB::get()->exec("UPDATE resources SET path='" + new_path + "', name='" + resource.name + "' WHERE id='" + resource.id + "'");
    }

    if (resource.type == "dir")
    {
      synchronizeResources(resource.path);
    }
  }
}

string MyDrive::getFileLink(const std::string &id) const
{
  resource_t resource = getResourceById(id);
  string path = "disk:" + resource.path + "/" + resource.name;

  return Drive::getFileLink(path);
}
