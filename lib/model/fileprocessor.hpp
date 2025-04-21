#pragma once

#include "model.hpp"

class FileProcessor {
   public:
    explicit FileProcessor(DataModel* model);

    void open();
    void save();
    void save_as();

   private:
    QString m_filename;
    DataModel* m_data;

    void read_file(const std::string& filename);
    void write_file(const std::string& filename);
};
