#include "thinkgame/assets.hpp"
#include <sstream>
#include <stdexcept>

namespace thinkgame {
std::filesystem::path findAssets(const std::filesystem::path& executable,
                                 const std::filesystem::path& fallback,
                                 const std::string& searchPath) {
    auto resolved = executable;
    std::error_code ec;
    if (!executable.has_parent_path()) {
#ifdef _WIN32
        constexpr char separator = ';';
#else
        constexpr char separator = ':';
#endif
        std::istringstream paths(searchPath);
        std::string directory;
        while (std::getline(paths, directory, separator)) {
            const auto candidate = std::filesystem::path(directory.empty() ? "." : directory) / executable;
            if (std::filesystem::is_regular_file(candidate, ec)) { resolved = candidate; break; }
#ifdef _WIN32
            if (candidate.extension().empty() && std::filesystem::is_regular_file(candidate.string() + ".exe", ec)) {
                resolved = candidate.string() + ".exe"; break;
            }
#endif
        }
    }
    const auto canonical = std::filesystem::canonical(resolved, ec);
    const auto beside = (ec ? std::filesystem::absolute(resolved) : canonical).parent_path();
    for (const auto& path : {beside / "assets", beside / "../share/thinkgame/assets", fallback}) {
        if (std::filesystem::is_regular_file(path / "DejaVuSans.ttf", ec) &&
            std::filesystem::is_regular_file(path / "DejaVuSans-Bold.ttf", ec) &&
            std::filesystem::is_regular_file(path / "logo.png", ec)) return path;
    }
    throw std::runtime_error("Assets not found. Keep the assets directory beside the executable.");
}
}
