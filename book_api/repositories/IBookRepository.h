#pragma once
#include <vector>
#include <optional>
#include "models/Book.h"
#include "common/Result.h"

class IBookRepository {
public:
    virtual ~IBookRepository() = default;

    virtual Result<std::vector<Book>> getAll() = 0;
    virtual Result<Book> getById(int id) = 0;
    virtual Result<Book> create(const Book& libro) = 0;
 
    virtual Result<bool> update(const Reader& item) = 0;
 
    virtual Result<bool> remove(int id) = 0;
};
