#include "fileprocessor.hpp"
#include <qlogging.h>
#include <qmessagebox.h>
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include <vector>

#include <QObject>
#include "utils.hpp"

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

    file.read(reinterpret_cast<char*>(&m_data->m_zn), sizeof(double));
    file.read(reinterpret_cast<char*>(&m_data->m_rot_x), sizeof(double));
    file.read(reinterpret_cast<char*>(&m_data->m_rot_y), sizeof(double));

    auto res = check_double_param<double>("invalid zn", m_data->m_zn, 0.3, 3) &&
               check_double_param<double>("invalid rotation x", m_data->m_rot_x,
                                          -180, 180) &&
               check_double_param<double>("invalid rotation y", m_data->m_rot_y,
                                          -180, 180);
    if (!res) {
        return;
    }

    file.read(reinterpret_cast<char*>(&m_data->m_far), sizeof(QColor));
    file.read(reinterpret_cast<char*>(&m_data->m_near), sizeof(QColor));

    file.read(reinterpret_cast<char*>(&n), sizeof(int16_t));
    file.read(reinterpret_cast<char*>(&m), sizeof(int16_t));
    file.read(reinterpret_cast<char*>(&m1), sizeof(int16_t));
    file.read(reinterpret_cast<char*>(&k), sizeof(int16_t));
    res = check_double_param<double>("invalid n", n, 1, 200) &&
          check_double_param<double>("invalid m", m, 2, 200) &&
          check_double_param<double>("invalid m1", m1, 1, 200);
    if (!res) {
        return;
    }

    std::vector<double> u(k);
    std::vector<double> v(k);

    file.read(reinterpret_cast<char*>(u.data()), k * sizeof(double));
    file.read(reinterpret_cast<char*>(v.data()), k * sizeof(double));

    m_data->set_m(m);
    m_data->set_m1(m1);
    m_data->set_points(u, v);
    m_data->set_n(n);
}

void FileProcessor::write_file(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);

    auto spline = m_data->spline();

    double zn = m_data->m_zn;
    double rotation_x = m_data->m_rot_x;
    double rotation_y = m_data->m_rot_y;
    QColor far = m_data->m_far;
    QColor near = m_data->m_near;

    auto n = static_cast<int16_t>(m_data->n());
    auto m = static_cast<int16_t>(m_data->m());
    auto m1 = static_cast<int16_t>(m_data->m1());
    auto points = m_data->points();
    auto k = points.first.size();

    file.write(reinterpret_cast<char*>(&zn), sizeof(double));
    file.write(reinterpret_cast<char*>(&rotation_x), sizeof(double));
    file.write(reinterpret_cast<char*>(&rotation_y), sizeof(double));
    file.write(reinterpret_cast<char*>(&far), sizeof(QColor));
    file.write(reinterpret_cast<char*>(&near), sizeof(QColor));

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

