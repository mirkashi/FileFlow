#include "CategoryManager.hpp"
#include <algorithm>
#include <cctype>

namespace fileflow {

CategoryManager::CategoryManager() {
    initializeMappings();
}

void CategoryManager::initializeMappings() {
    using Cat = Category;

    category_order_ = {
        Cat::Images,
        Cat::Videos,
        Cat::Audio,
        Cat::Documents,
        Cat::Spreadsheets,
        Cat::Presentations,
        Cat::Archives,
        Cat::Code,
        Cat::Others
    };

    const std::initializer_list<std::pair<std::string_view, Cat>> mappings = {
        // Images
        {".jpg", Cat::Images}, {".jpeg", Cat::Images}, {".png", Cat::Images},
        {".gif", Cat::Images}, {".bmp", Cat::Images}, {".webp", Cat::Images},
        {".svg", Cat::Images}, {".ico", Cat::Images},

        // Videos
        {".mp4", Cat::Videos}, {".mkv", Cat::Videos}, {".avi", Cat::Videos},
        {".mov", Cat::Videos}, {".wmv", Cat::Videos}, {".flv", Cat::Videos},
        {".webm", Cat::Videos},

        // Audio
        {".mp3", Cat::Audio}, {".wav", Cat::Audio}, {".flac", Cat::Audio},
        {".aac", Cat::Audio}, {".ogg", Cat::Audio}, {".m4a", Cat::Audio},

        // Documents
        {".pdf", Cat::Documents}, {".doc", Cat::Documents}, {".docx", Cat::Documents},
        {".txt", Cat::Documents}, {".rtf", Cat::Documents}, {".odt", Cat::Documents},

        // Spreadsheets
        {".xls", Cat::Spreadsheets}, {".xlsx", Cat::Spreadsheets}, {".csv", Cat::Spreadsheets},

        // Presentations
        {".ppt", Cat::Presentations}, {".pptx", Cat::Presentations},

        // Archives
        {".zip", Cat::Archives}, {".rar", Cat::Archives}, {".7z", Cat::Archives},
        {".tar", Cat::Archives}, {".gz", Cat::Archives}, {".bz2", Cat::Archives},

        // Code
        {".cpp", Cat::Code}, {".hpp", Cat::Code}, {".c", Cat::Code}, {".h", Cat::Code},
        {".py", Cat::Code}, {".js", Cat::Code}, {".ts", Cat::Code},
        {".jsx", Cat::Code}, {".tsx", Cat::Code}, {".java", Cat::Code},
        {".rs", Cat::Code}, {".go", Cat::Code}, {".php", Cat::Code},
        {".html", Cat::Code}, {".css", Cat::Code}
    };

    for (const auto& [ext, cat] : mappings) {
        std::string normalized = normalizeExtension(ext);
        extension_to_category_[normalized] = cat;
        category_to_extensions_[cat].push_back(normalized);
    }
}

CategoryManager::Category CategoryManager::getCategory(std::string_view extension) const noexcept {
    std::string normalized = normalizeExtension(extension);
    auto it = extension_to_category_.find(normalized);
    if (it != extension_to_category_.end()) {
        return it->second;
    }
    return Category::Others;
}

std::string_view CategoryManager::getCategoryName(Category category) const noexcept {
    using Cat = Category;
    switch (category) {
        case Cat::Images: return "Images";
        case Cat::Videos: return "Videos";
        case Cat::Audio: return "Audio";
        case Cat::Documents: return "Documents";
        case Cat::Spreadsheets: return "Spreadsheets";
        case Cat::Presentations: return "Presentations";
        case Cat::Archives: return "Archives";
        case Cat::Code: return "Code";
        case Cat::Others: return "Others";
    }
    return "Unknown";
}

std::vector<std::string_view> CategoryManager::getAllCategories() const noexcept {
    std::vector<std::string_view> result;
    result.reserve(category_order_.size());
    for (const auto& cat : category_order_) {
        result.push_back(getCategoryName(cat));
    }
    return result;
}

std::vector<std::string_view> CategoryManager::getExtensionsForCategory(Category category) const noexcept {
    auto it = category_to_extensions_.find(category);
    if (it != category_to_extensions_.end()) {
        std::vector<std::string_view> result;
        result.reserve(it->second.size());
        for (const auto& ext : it->second) {
            result.push_back(ext);
        }
        return result;
    }
    return {};
}

std::string CategoryManager::normalizeExtension(std::string_view extension) noexcept {
    std::string result;
    result.reserve(extension.size());

    bool has_dot = !extension.empty() && extension[0] == '.';
    std::size_t start = has_dot ? 1 : 0;

    for (std::size_t i = start; i < extension.size(); ++i) {
        result.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(extension[i]))));
    }

    return "." + result;
}

} // namespace fileflow