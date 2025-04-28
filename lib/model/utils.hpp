#pragma once

#include <QMessageBox>
#include <QString>
#include <string>

template <typename T>
bool check_double_param(const std::string& message, T value, T min, T max) {
    if (value < min || value > max) {
        QMessageBox::critical(nullptr, "Error", QString::fromStdString(message),
                              QMessageBox::Ok  // Кнопка "OK"
        );
        return false;
    }
    return true;
}
