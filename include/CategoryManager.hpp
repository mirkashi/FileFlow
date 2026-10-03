#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace fileflow {

class CategoryManager {
public:
    enum class Category {
        Images,
        Videos,
        Audio,
        Documents,
        Spreadsheets,
        Presentations,
        Archives,
        Code,
        Others
    };

    CategoryManager();

    [[nodiscard]] Category getCategory(std::string_view extension) const noexcept;
    [[nodiscard]] std::string_view getCategoryName(Category category) const noexcept;
    [[nodiscard]] std::vector<std::string_view> getAllCategories() const noexcept;
    [[nodiscard]] std::vector<std::string_view> getExtensionsForCategory(Category category) const noexcept;

    static std::string normalizeExtension(std::string_view extension) noexcept;

private:
    void initializeMappings();

    std::unordered_map<std::string, Category> extension_to_category_;
    std::unordered_map<Category, std::vector<std::string>> category_to_extensions_;
    std::vector<Category> category_order_;
};

} // namespace fileflow