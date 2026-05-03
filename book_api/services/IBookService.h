#pragma once
#include <vector>
#include <optional>
#include "models/Book.h"
#include "common/Result.h"
class IBookService {
public:
    virtual ~IBookService() = default;

    virtual Result<std::vector<Book>> getAll() = 0;
    virtual Result<Book> getById(int id) = 0;
    virtual Result<Book> create(const Book& libro) = 0;
    virtual Result<bool> remove(int id) = 0;
<<<<<<< HEAD
    virtual Result<bool> update(const Reader& item) = 0;
=======
>>>>>>> de6f692cc521b603432362c4212f7a0d55aa9687
};