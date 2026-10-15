#include "thinkgame/progress.hpp"
#include <cstdlib>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <system_error>
#ifdef _WIN32
#include <windows.h>
#endif

namespace thinkgame {
bool Progress::valid() const {
    if (completed < 0 || completed > 30 || !unlocked(current)) return false;
    for (int h : hints) if (h < 0 || h > 3) return false;
    for (int i = completed + 1; i < 30; ++i) if (hints[i] != 0) return false;
    return true;
}
namespace {
std::uint64_t checksum(const std::string& text) {
    std::uint64_t hash = 14695981039346656037ULL;
    for (unsigned char c : text) { hash ^= c; hash *= 1099511628211ULL; }
    return hash;
}
std::string payload(const Progress& p) {
    std::ostringstream out;
    out << "THINKGAME 1\n" << p.completed << ' ' << p.current << '\n';
    for (int h : p.hints) out << h << ' ';
    out << '\n';
    return out.str();
}
bool read(const std::filesystem::path& path, Progress& p) {
    std::error_code ec;
    const auto size = std::filesystem::file_size(path, ec);
    if (ec || size > 4096) return false;
    std::ifstream in(path, std::ios::binary);
    std::string magic; int version; std::uint64_t sum;
    if (!(in >> magic >> version) || magic != "THINKGAME" || version != 1) return false;
    if (!(in >> p.completed >> p.current)) return false;
    for (int& h : p.hints) if (!(in >> h)) return false;
    if (!(in >> sum)) return false;
    in >> std::ws;
    return in.eof() && p.valid() && sum == checksum(payload(p));
}
bool atomicReplace(const std::filesystem::path& source, const std::filesystem::path& target, std::error_code& ec) {
#ifdef _WIN32
    if (!MoveFileExW(source.c_str(), target.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
        ec = std::error_code(GetLastError(), std::system_category()); return false;
    }
    return true;
#else
    std::filesystem::rename(source, target, ec); return !ec;
#endif
}
}
SaveStore::SaveStore(std::filesystem::path path) : path_(std::move(path)) {}
std::filesystem::path SaveStore::defaultPath() {
#ifdef _WIN32
    if (const char* p = std::getenv("LOCALAPPDATA")) return std::filesystem::path(p) / "ThinkGame" / "progress.save";
#elif defined(__APPLE__)
    if (const char* p = std::getenv("HOME")) return std::filesystem::path(p) / "Library/Application Support/ThinkGame/progress.save";
#else
    if (const char* p = std::getenv("XDG_DATA_HOME"); p && std::filesystem::path(p).is_absolute()) return std::filesystem::path(p) / "thinkgame/progress.save";
    if (const char* p = std::getenv("HOME")) return std::filesystem::path(p) / ".local/share/thinkgame/progress.save";
#endif
    return std::filesystem::temp_directory_path() / "thinkgame" / "progress.save";
}
Progress SaveStore::load(std::string& notice) const {
    notice.clear();
    Progress p;
    if (read(path_, p)) return p;
    if (read(path_.string() + ".bak", p)) {
        notice = "Recovered saved progress from backup."; return p;
    }
    std::error_code ec;
    if (std::filesystem::exists(path_, ec)) notice = "Saved progress could not be read. Starting at Level 1.";
    return {};
}
bool SaveStore::save(const Progress& p, std::string& error) const {
    if (!p.valid()) { error = "invalid progress"; return false; }
    std::error_code ec;
    if (!path_.parent_path().empty()) std::filesystem::create_directories(path_.parent_path(), ec);
    if (ec) { error = ec.message(); return false; }
    const auto temporary = std::filesystem::path(path_.string() + ".tmp");
    {
        std::ofstream out(temporary, std::ios::binary | std::ios::trunc);
        out << payload(p) << checksum(payload(p)) << '\n';
        out.flush();
        if (!out) { error = "write failed"; return false; }
    }
    Progress previous;
    if (read(path_, previous)) {
        std::filesystem::copy_file(path_, path_.string() + ".bak", std::filesystem::copy_options::overwrite_existing, ec);
        if (ec) { error = "backup failed: " + ec.message(); return false; }
    }
    if (!atomicReplace(temporary, path_, ec)) { error = ec.message(); return false; }
    return true;
}
}
