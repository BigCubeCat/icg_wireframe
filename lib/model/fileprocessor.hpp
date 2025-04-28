#pragma once

#include "model.hpp"

class FileProcessor {
   public:
    explicit FileProcessor(DataModel* model);

    void open();
    void save();
    void save_as();

    void read_file(const std::string& filename);

   private:
    QString m_filename;
    DataModel* m_data;

    void write_file(const std::string& filename);
};
