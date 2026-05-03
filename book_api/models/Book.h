#pragma once
#include <string>
 
struct Reader
{
   bool isReading;
   int id;
};
 

struct Book {
    int id;
    std::string title;
    std::string autor;
    int year;
     int pages;
     int read;
     std::string date_read;
};
