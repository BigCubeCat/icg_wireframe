#include "fileprocessor.hpp"
#include <qlogging.h>
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include <vector>

#include <QObject>

FileProcessor::FileProcessor(DataModel* model) : m_data(model) {}

void FileProcessor::read_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        QMessageBox::critical(nullptr, "Error", "cant open file");
        return;
    }
    int16_t n;
    int16_t m;
    int16_t m1;
    int16_t k;
    file.read(reinterpret_cast<char*>(&n), sizeof(int16_t));
    file.read(reinterpret_cast<char*>(&m), sizeof(int16_t));
    file.read(reinterpret_cast<char*>(&m1), sizeof(int16_t));
    file.read(reinterpret_cast<char*>(&k), sizeof(int16_t));

    std::vector<double> u(k);
    std::vector<double> v(k);

    file.read(reinterpret_cast<char*>(u.data()), k * sizeof(double));
    file.read(reinterpret_cast<char*>(v.data()), k * sizeof(double));

    m_data->set_n(n);
    m_data->set_m(m);
    m_data->set_m1(m1);
    m_data->set_points(u, v);
}

void FileProcessor::write_file(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    auto n = static_cast<int16_t>(m_data->n());
    auto m = static_cast<int16_t>(m_data->m());
    auto m1 = static_cast<int16_t>(m_data->m1());
    auto points = m_data->points();
    auto k = points.first.size();

    file.write(reinterpret_cast<char*>(&n), sizeof(int16_t));
    file.write(reinterpret_cast<char*>(&m), sizeof(int16_t));
    file.write(reinterpret_cast<char*>(&m1), sizeof(int16_t));
    file.write(reinterpret_cast<char*>(&k), sizeof(int16_t));

    file.write(reinterpret_cast<char*>(points.first.data()),
               k * sizeof(double));
    file.write(reinterpret_cast<char*>(points.second.data()),
               k * sizeof(double));
}

void FileProcessor::open() {
    QString file_path =
        QFileDialog::getOpenFileName(nullptr, ("Open Wireframe File"), "",
                                     ("Binary Files (*.bin);;All Files (*)"));

    if (!file_path.isEmpty()) {
        read_file(file_path.toStdString());
        m_filename = file_path;
    }
}

void FileProcessor::save() {
    if (m_filename.isEmpty()) {
        save_as();
        return;
    }
    write_file(m_filename.toStdString());
}

void FileProcessor::save_as() {
    QString file_path =
        QFileDialog::getSaveFileName(nullptr, ("Save Binary File As"), "",
                                     ("Binary Files (*.bin);;All Files (*)"));
    if (!file_path.isEmpty()) {
        write_file(file_path.toStdString());
        m_filename = file_path;
    }
}
