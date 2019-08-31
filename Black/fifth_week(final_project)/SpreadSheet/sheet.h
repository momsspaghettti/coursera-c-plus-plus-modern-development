#pragma once
#include "common.h"
#include "cell.h"
#include <unordered_set>


struct PositionHash {
public:
    size_t operator()(const Position& pos) const {
        return 149 * hash_(pos.row) + pos.col;
    }

private:
    std::hash<int> hash_;
};


class PositionCache {
public:
    [[nodiscard]] const std::optional<std::string>& getCache() const {
        return cache_;
    }

    [[nodiscard]] std::optional<std::string>& getCache() {
        return cache_;
    }

    void setCache(std::string new_val) {
        cache_ = std::move(new_val);
    }

private:
    std::unordered_set<Position, PositionHash> incoming_edges_;
    std::unordered_set<Position, PositionHash> outgoing_edges_;
    std::optional<std::string> cache_;
};


class Sheet : public ISheet {
public:
    Sheet();

    [[nodiscard]] Size GetPrintableSize() const override;

    void SetCell(Position pos, std::string text) override;

    [[nodiscard]] const ICell *GetCell(Position pos) const override;

    [[nodiscard]] ICell *GetCell(Position pos) override;

    void ClearCell(Position pos) override;

    void InsertRows(int before, int count = 1) override;

    void InsertCols(int before, int count = 1) override;

    void DeleteRows(int first, int count = 1) override;

    void DeleteCols(int first, int count = 1) override;

    void PrintValues(std::ostream &output) const override;

    void PrintTexts(std::ostream &output) const override;

private:
    std::unique_ptr<std::vector<std::vector<std::shared_ptr<Cell>>>> sheet_;
    Size size_;

    std::unique_ptr<std::unordered_map<Position, PositionCache, PositionHash>> cache_;

    [[nodiscard]] std::shared_ptr<Cell> create_new_cell(const Position& pos, const std::string& text);
    [[nodiscard]] std::shared_ptr<Cell> update_cell(const Position& pos, const std::string& text);
};

