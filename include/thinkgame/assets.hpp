#pragma once
#include <filesystem>
#include <string>

namespace thinkgame {
std::filesystem::path findAssets(const std::filesystem::path& executable,
                                 const std::filesystem::path& fallback,
                                 const std::string& searchPath);
}
