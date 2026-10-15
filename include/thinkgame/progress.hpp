#pragma once
#include <array>
#include <filesystem>
#include <string>

namespace thinkgame {
struct Progress {
    int completed = 0;
    int current = 1;
    std::array<int, 30> hints{};
    bool unlocked(int level) const { return level >= 1 && level <= 30 && level <= completed + 1; }
    bool solved(int level) const { return level >= 1 && level <= completed; }
    bool valid() const;
};
class SaveStore {
public:
    explicit SaveStore(std::filesystem::path path);
    Progress load(std::string& notice) const;
    bool save(const Progress& progress, std::string& error) const;
    const std::filesystem::path& path() const { return path_; }
    static std::filesystem::path defaultPath();
private:
    std::filesystem::path path_;
};
}
