#ifndef TAG_FOLDER_HPP_
#define TAG_FOLDER_HPP_

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
    const std::string kTagPathErrorMsg              { "Path is not directory" };
    const std::string kTagRestrictedCharErrorMsg    { "Tag has Restricted Character" };

    const std::string kDefaultTagSeparator{ "___" };
    const std::wstring kDefaultTagSeparatorW{ L"___" };

    class FolderTagErrorRuntime : public std::runtime_error {
    public:
        FolderTagErrorRuntime(const std::string& error_msg) : std::runtime_error{ error_msg } {};
    };

    template<CharsType CharT = wchar_t>
    inline std::basic_string<CharT>
        SetPathTag(const std::basic_string<CharT>& tag_separator, const std::basic_string<CharT>& tag_name) {

        return tag_separator + tag_name;
    }

    template<CharsType CharT>
    std::basic_string<CharT> TagRegexInit(const std::basic_string<CharT>& full_tag);
    template<>
    std::string TagRegexInit<char>(const std::string& full_tag) { return full_tag + "\\z"; }
    template<>
    std::wstring TagRegexInit<wchar_t>(const std::wstring& full_tag) { return full_tag + L"\\z"; }

    /// Checks if Folder has some tag
    /// full_tag = tag_separator + tag_name
    template<CharsType CharT = wchar_t>
    bool HasFolderTag(const boost::filesystem::path& folder_path, const std::basic_string<CharT>& full_tag) {
        if (!full_tag.empty() && !folder_path.empty()) {
            if (boost::filesystem::is_directory(folder_path)) {
                if (HasRestrictedCharsForOS(full_tag)) {
                    throw FolderTagErrorRuntime(kTagRestrictedCharErrorMsg);
                }

                const std::basic_string<CharT> folder_name{ folder_path.filename().string<std::basic_string<CharT>>() };
                const boost::basic_regex<CharT> tag_regex{ TagRegexInit<CharT>(full_tag) };
                if (boost::regex_search(folder_name.begin(), folder_name.end(), tag_regex)) { // path_name has tag
                    return true;
                } else { return false; }

            } else {
                throw FolderTagErrorRuntime(kTagPathErrorMsg);
            }
        } else {
            return true;
        }
    }

    template<CharsType CharT>
    std::basic_string<CharT> TagRegexName(const std::basic_string<CharT>& tag_separator);
    template<>
    std::string TagRegexName<char>(const std::string& tag_separator) { return "(?<=" + tag_separator + ").+(?=\\z)"; }
    template<>
    std::wstring TagRegexName<wchar_t>(const std::wstring& tag_separator) { return L"(?<=" + tag_separator + L").+(?=\\z)"; }

    template<CharsType CharT = wchar_t>
    std::basic_string<CharT> GetFolderTag(const boost::filesystem::path& folder_path,
                                          const std::basic_string<CharT>& tag_separator) {
        if (!folder_path.empty() && !tag_separator.empty() && boost::filesystem::is_directory(folder_path)) {
            const std::basic_string<CharT> folder_name{ folder_path.filename().string<std::basic_string<CharT>>() };
            const boost::basic_regex<CharT> tag_regex{ TagRegexName<CharT>(tag_separator) };
            boost::match_results<const CharT*> tag_regex_match{};
            if (boost::regex_search(&folder_name[0], (&folder_name.back()) + 1, tag_regex_match, tag_regex)) { // path_name has tag
                return tag_regex_match.str();
            } else { return std::basic_string<CharT>{}; }

        } else { return std::basic_string<CharT>{}; }
    }

    /// Add specific tag to folder name
    /// full_tag = tag_separator + tag_name
    template<CharsType CharT = wchar_t>
    void AddTagToFolder(const boost::filesystem::path& folder_path, const std::basic_string<CharT>& full_tag) {
        if (!full_tag.empty() && !folder_path.empty() && !HasFolderTag(folder_path, full_tag)) {
            boost::filesystem::rename(folder_path, boost::filesystem::path(folder_path.string<std::basic_string<CharT>>() + full_tag));
        }
    }

    /// Deletes Tag from boost::filesystem::path
    template<CharsType CharT = wchar_t>
    boost::filesystem::path DeleteTagFromPath(const boost::filesystem::path& folder_path, const std::basic_string<CharT>& full_tag) {
        const std::basic_string<CharT> path_str{ folder_path.string<std::basic_string<CharT>>() };
        const size_t path_length{ path_str.length() };
        const size_t tag_length{ full_tag.length() };
        return boost::filesystem::path(path_str.substr(0, path_length - tag_length));
    }

    /// Delete specific tag from folder name
    /// full_tag = tag_separator + tag_name
    template<CharsType CharT = wchar_t>
    void DeleteTagFromFolder(const boost::filesystem::path& folder_path, const std::basic_string<CharT>& full_tag) {
        if (!full_tag.empty() && !folder_path.empty() && HasFolderTag(folder_path, full_tag)) {
            boost::filesystem::rename(folder_path, DeleteTagFromPath(folder_path, full_tag));
        }
    }

    /// Replace specific tag from folder name
    /// full_tag = tag_separator + tag_name
    template<CharsType CharT = wchar_t>
    void ReplaceTagFromFolder(const boost::filesystem::path& folder_path, const std::basic_string<CharT>& full_tag,
                              const std::basic_string<CharT>& new_tag) {
        if (!full_tag.empty() && !new_tag.empty() && !folder_path.empty() && HasFolderTag(folder_path, full_tag)) {
            DeleteTagFromFolder(folder_path, full_tag);
            const boost::filesystem::path path_without_tag{ DeleteTagFromPath(folder_path, full_tag) };
            AddTagToFolder(path_without_tag, new_tag);
        }
    }

} // !namespace file

#endif // !TAG_FOLDER_HPP_

// TODO: Convert functionality from Folder to Path. (Regular files)
