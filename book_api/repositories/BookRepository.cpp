#include "repositories/BookRepository.h"
 #include <drogon/drogon.h>
BookRepository::BookRepository(const std::string& dbPath) {
 
    
    int rc = sqlite3_open(dbPath.c_str(), &db_);
    if (rc != SQLITE_OK) {
        std::string err = sqlite3_errmsg(db_);
        sqlite3_close(db_);
        throw std::runtime_error("No se pudo abrir la base de datos: " + err);
    }
   
    initDb();
}
BookRepository::~BookRepository() {
    sqlite3_close(db_);
}
std::mutex BookRepository::mutex_;
void BookRepository::initDb() {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS books (
            id    INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            autor TEXT NOT NULL,
            year  INTEGER NOT NULL,
            pages INTEGER,
            read  INTEGER NOT NULL DEFAULT 0,
            date_read Text NOT NULL DEFAULT CURRENT_TIMESTAMP,
            UNIQUE(title, autor) 
              

        );
      
    )";
const char* seedData = R"(
         INSERT OR IGNORE INTO books (id, title, autor, year,pages) VALUES
            (1, 'El Quijote',           'Cervantes',      1605, 863);
        INSERT OR IGNORE INTO books (id, title, autor, year,pages) VALUES
            (2, 'Cien años de soledad', 'Garcia Marquez', 1967, 417);
        INSERT OR IGNORE INTO books (id, title, autor, year,pages) VALUES
            (3, '1984',                 'George Orwell',  1949, 328);
    )";
    char* errMsg = nullptr;
   if (sqlite3_exec(db_, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string err = errMsg;
        sqlite3_free(errMsg);
        throw std::runtime_error("Error al crear tabla: " + err);
    }

    if (sqlite3_exec(db_, seedData, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string err = errMsg;
        sqlite3_free(errMsg);
        throw std::runtime_error("Error al insertar datos: " + err);
    }

}

Result<std::vector<Book>> BookRepository::getAll() {
    std::lock_guard<std::mutex> lock(mutex_);
        std::vector<Book> libros;
    const char* sql = "SELECT id, title, autor, year, pages, read, date_read FROM books";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return Result<std::vector<Book>>::fail("Error al preparar consulta");
    }
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
       Book b;
        b.id = sqlite3_column_int(stmt, 0);
        b.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        b.autor = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        b.year = sqlite3_column_int(stmt, 3);
        b.pages = sqlite3_column_int(stmt, 4);
        b.read = sqlite3_column_int(stmt, 5);
        b.date_read = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        libros.push_back(b);
    }
    
    return Result<std::vector<Book>>::ok(libros) ;
}

Result<Book> BookRepository::getById(int id) {
   try
   {
    std::lock_guard<std::mutex> lock(mutex_);
   const char* sql = "SELECT id, title, autor, year, pages, read, date_read FROM books WHERE id = ?";
   
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
       return Result<Book>::fail("Error en al preparar consulta");
    }
    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
         sqlite3_finalize(stmt);
        return Result<Book>::fail("Book not found");
    }
    Book b;
    b.id = sqlite3_column_int(stmt, 0);
    b.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    b.autor = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    b.year = sqlite3_column_int(stmt, 3);
    b.pages = sqlite3_column_int(stmt, 4);
    b.read = sqlite3_column_int(stmt, 5);
    b.date_read = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
    sqlite3_finalize(stmt);
    return Result<Book>::ok(b);
   }
   catch(const std::exception& e)
   {
        LOG_ERROR << e.what() << '\n';
        return Result<Book>::fail(e.what() );
   
   }
   
}

Result<Book> BookRepository::create(const Book& book) {
    std::lock_guard<std::mutex> lock(mutex_);
    const char* sql = "INSERT INTO books (title, autor, year, pages, read, date_read) VALUES (?, ?, ?, ?, ?, ?)";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
       return Result<Book>::fail("Error al preparar consulta");
    }
    sqlite3_bind_text(stmt, 1, book.title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, book.autor.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, book.year);
    sqlite3_bind_int(stmt, 4, book.pages);
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return Result<Book>::fail("Error al insertar libro");
        
    }
    sqlite3_finalize(stmt);
    Book created = book;
    created.id = static_cast<int>(sqlite3_last_insert_rowid(db_));
    return Result<Book>::ok(created);
 
}
Result<bool> BookRepository::update(const Reader& item)
{
    std::lock_guard<std::mutex> lock(mutex_);
    const char* sql = "UPDATE books SET  read = ?, date_read = CURRENT_TIMESTAMP WHERE id = ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
       return Result<bool>::fail("Error al preparar script");
    }
    
    sqlite3_bind_int(stmt, 1, item.isReading ? 1 : 0);
   
    sqlite3_bind_int(stmt, 2, item.id);
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return Result<bool>::fail("Error al actualizar libro");
        
    }
    sqlite3_finalize(stmt);
    return Result<bool>::ok(true);
}
Result<bool> BookRepository::remove(int id) {
    try
    {
         std::lock_guard<std::mutex> lock(mutex_);
        const char* sql = "DELETE FROM books WHERE id = ?";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            return Result<bool>::fail("Error al preparar consulta");
        }
        sqlite3_bind_int(stmt, 1, id);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return Result<bool>::fail("Error al eliminar libro");
        }
        sqlite3_finalize(stmt);
        return Result<bool>::ok(true);
    }
    catch(const std::exception& e)
    {
         return Result<bool>::fail(e.what());
   
    }
    
   
}