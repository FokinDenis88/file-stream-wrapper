#ifndef PREFIX_SUFFIX_PATH_HPP_
#define PREFIX_SUFFIX_PATH_HPP_

#include <string>
#include <exception>
#include <stdexcept>
#include <concepts>
#include <type_traits>

#include <iterator>

#include "boost/regex/v5/regex.hpp"
#include "boost/filesystem.hpp"

#include "file-types.hpp"
#define CORRECT_NAME_FOR_WINDOWS
#include "file-service.hpp"

namespace file {
    const std::string kPathTypeErrorMsg         { "Path is not File or Directory" };
    const std::string kRestrictedCharErrorMsg   { "File or Directory has Restricted Character" };

    class PathPrefixSuffixErrorRuntime : public std::runtime_error {
    public:
        PathPrefixSuffixErrorRuntime(const std::string& error_msg) : std::runtime_error{ error_msg } {};
    };


    /// Checks if Path(file of directory) has some prefix or suffix
    bool HasPathPrefixOrSuffix(const boost::filesystem::path& target_path, const std::wstring& prefix_suffix,
                               const bool is_prefix = true) {
        if (!target_path.empty() && !prefix_suffix.empty()) {
            if (boost::filesystem::is_directory(target_path) ||
                boost::filesystem::is_regular_file(target_path)) {

                if (HasRestrictedCharsForOS(prefix_suffix)) {
                    throw PathPrefixSuffixErrorRuntime(kRestrictedCharErrorMsg);
                }

                const std::wstring target_name{ target_path.stem().wstring()};
                const boost::wregex prefix_suffix_regex{
                    [is_prefix, &prefix_suffix]() -> std::wstring {
                        if (is_prefix) { return L"^" + prefix_suffix; }
                        else { return prefix_suffix + L"\\z"; }
                }() };
                if (boost::regex_search(target_name.begin(), target_name.end(), prefix_suffix_regex)) { // path_name has tag
                    return true;
                } else {
                    return false;
                }

            } else {
                throw PathPrefixSuffixErrorRuntime(kPathTypeErrorMsg);
            }
        } else {
            return true;
        }
    }

    /// Get path of file or directory with added prefix or suffix. If they already exists in name, they are not added.
    /// @return path with prefix & suffix
    boost::filesystem::path GetPathWithPrefixNSuffix(const boost::filesystem::path& target_path,
                                                     const std::wstring& prefix, const std::wstring& suffix) {
        std::wstring new_name{};
        if (!HasPathPrefixOrSuffix(target_path, prefix)) { new_name = prefix; }
        new_name.append(target_path.stem().wstring());
        if (!HasPathPrefixOrSuffix(target_path, suffix, false)) { new_name.append(suffix); }
        new_name.append(target_path.extension().wstring());

        return target_path.parent_path().wstring() + L"\\" + new_name;
    }

    /// Deletes Prefix or|and Suffix from boost::filesystem::path
    boost::filesystem::path GetPathWithoutPrefixNSuffix(const boost::filesystem::path& target_path,
                                                        const std::wstring& prefix, const std::wstring& suffix) {
        std::wstring new_name{ target_path.stem().wstring() };
        if (HasPathPrefixOrSuffix(target_path, prefix)) { new_name.erase(0, prefix.length()); }
        if (HasPathPrefixOrSuffix(target_path, suffix, false)) { new_name.erase(new_name.length() - suffix.length(), suffix.length()); }
        new_name.append(target_path.extension().wstring());

        return target_path.parent_path().wstring() + L"\\" + new_name;
    }


    /// Add specific prefix or|and suffix to file or directory
    boost::filesystem::path AddPrefixSuffixToPath(const boost::filesystem::path& target_path, const std::wstring& prefix, const std::wstring& suffix) {
        boost::filesystem::path new_path{};
        if ( !target_path.empty() && (!prefix.empty() || !suffix.empty()) ) {
            new_path = GetPathWithPrefixNSuffix(target_path, prefix, suffix);
            if (target_path != new_path) {
                boost::filesystem::rename(target_path, new_path);
            }
        }
        return new_path;
    }

    /// Delete specific tag from folder name
    /// full_tag = tag_separator + tag_name
    boost::filesystem::path DeletePrefixSuffixFromPath(const boost::filesystem::path& target_path, const std::wstring& prefix, const std::wstring& suffix) {
        boost::filesystem::path new_path{};
        if (!target_path.empty() && (!prefix.empty() || !suffix.empty())) {
            new_path = GetPathWithoutPrefixNSuffix(target_path, prefix, suffix);
            if (target_path != new_path) {
                boost::filesystem::rename(target_path, new_path);
            }
        }
        return new_path;
    }

    /// Replace specific tag from folder name
    /// full_tag = tag_separator + tag_name
    void ReplacePrefixSuffixFromPath(const boost::filesystem::path& target_path, const std::wstring& prefix, const std::wstring& suffix,
                                     const std::wstring& new_prefix, const std::wstring& new_suffix) {
        if (!target_path.empty() && (!prefix.empty() || !suffix.empty())) {
            boost::filesystem::path path_naked{ DeletePrefixSuffixFromPath(target_path, prefix, suffix) };
            AddPrefixSuffixToPath(path_naked, new_prefix, new_suffix);
        }
    }

} // !namespace file

#endif // !PREFIX_SUFFIX_PATH_HPP_

// TODO: Convert functionality from Folder to Path. (Regular files)
