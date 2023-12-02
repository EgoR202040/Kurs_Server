#pragma once
/** @file
 * @author Егоров Е.А.
 * @version 1.0
 * @brief Заголовочный файл для модуля подключения к базе данных
 */
#include <map>
#include <string>
/** @brief Класс для подключения к базе данных
 * @details Чтение базы данных происходит в методе connect_to_base.
 * Для получения базы данных в виде массива map используется метод get_data.
 */
class Connector_to_base
{
private:
    std::map<std::string,std::string> data_base;
public:
 /**
 * @brief Функция установки соединения с базой данных
 * @param [in] base_file Путь к файлу с базой данных. Значение по умолчанию - "/home/stud/base/base.txt"
 * @throw crit_err, если файл с базой данных не прочитался
 */
    int connect_to_base(std::string base_file = "/home/stud/base/base.txt");
 /**
 * @brief Функция получения базы данных в виде массива map
 * @return data_base
 */
    std::map<std::string,std::string> get_data()
    {
        return data_base;
    }

};
