#pragma once
#include "IBookRepository.h"
#include <sqlite3.h>
#include <vector>
#include <mutex>
 
class BookRepository : public IBookRepository {
public:

    explicit BookRepository(const std::string& dbPath = "biblio.db");
    ~BookRepository();
    Result<std::vector<Book>> getAll() override;
     Result<Book> getById(int id) override;
     Result<Book> create(const Book& libro) override;
     Result<bool> remove(int id) override;
     Result<bool> update(const Reader& item) override;

private:
     
    static std::mutex mutex_;
     sqlite3* db_;

    void initDb(); 
};