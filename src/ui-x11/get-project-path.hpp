#ifndef GET_PROJECT_PATH_HPP_
#define GET_PROJECT_PATH_HPP_

#include <string>
#include <filesystem>

/**
 * @brief returns relative path for file in project
 * 
 * @param rel relative portion of filepath from project head
*/
std::string getProjectPath(const std::string& rel) {
    std::filesystem::path fp(__FILE__);
    std::filesystem::path d = fp.parent_path() / ".." / ".." / std::filesystem::path(rel);
    return d.string();
}

#endif // GET_ASSETS_PATH_HPP_