#include <iostream>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iomanip>

namespace fs = std::filesystem;

// INFO ABOUT FOLDER
struct FolderInfo {
    std::uintmax_t size;
    fs::path path;
};

// RECURRENT FUNCTION TO CALCULATE FOLDER SIZE
std::uintmax_t calculateFolderSize(const fs::path& folderPath, std::unordered_map<fs::path, std::uintmax_t>& folderSizes) {
    std::uintmax_t totalSize = 0;

    try {
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (fs::is_regular_file(entry.path())) {
                totalSize += fs::file_size(entry.path());
            } else if (fs::is_directory(entry.path())) {
                totalSize += calculateFolderSize(entry.path(), folderSizes);
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "ERROR: " << e.what() << '\n';
    }

    folderSizes[folderPath] = totalSize; // Zapisujemy rozmiar folderu
    return totalSize;
}


std::vector<FolderInfo> listFoldersBySize(const fs::path& folderPath) {
    std::unordered_map<fs::path, std::uintmax_t> folderSizes;

    calculateFolderSize(folderPath, folderSizes);

    std::vector<FolderInfo> folders;
    for (const auto& [path, size] : folderSizes) {
        folders.push_back({size, path});
    }

    std::sort(folders.begin(), folders.end(), [](const FolderInfo& a, const FolderInfo& b) {
        return a.size > b.size;
    });

    return folders;
}

int main() {
    std::string folderPath="PATH TO FOLDEDR";

    if (!fs::exists(folderPath) || !fs::is_directory(folderPath)) {
        std::cerr << "PATH DOESN'T EXIST OR IT IS NOT DIRECTORY.\n";
        return 1;
    }

    auto folders = listFoldersBySize(folderPath);

    if (folders.empty()) {
        std::cout << "THERE ISNT ANY FOLDER UNDEDR THIS PATH.\n";
        return 0;
    }

    std::cout << "FOLDERS SORTED DESCENDING:\n";
    for (const auto& folder : folders) {
        std::cout << std::fixed << std::setprecision(2)
                  << static_cast<double>(folder.size) / (1024 * 1024) << " MB - "
                  << folder.path.string() << '\n';
    }

    return 0;
}
