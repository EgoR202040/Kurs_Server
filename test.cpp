#include "Includer.h"
#include <limits>
#include <chrono>
#include <thread>
#include <UnitTest++/UnitTest++.h>
#define inf numeric_limits<double>::infinity()
#define _UNITTEST_ 1
//
//Тест подключения к базе данных (Класс Connector_to_base)
//
SUITE(Connector_test){
	TEST(newline_in_path){
		Connector_to_base Con;
		std::string path = "test_files/base\n";
		CHECK_THROW(Con.connect_to_base(path),crit_err);
	}
	TEST(incorrect_path){
		Connector_to_base Con;
		std::string path = "#5df%s";
		CHECK_THROW(Con.connect_to_base(path),crit_err);
	}
	TEST(empty_path){
		Connector_to_base Con;
		std::string path = "";
		CHECK_THROW(Con.connect_to_base(path),crit_err);
	}
	TEST(wrong_path){
		Connector_to_base Con;
		std::string path = "/tesotfsa/base.txt";
		CHECK_THROW(Con.connect_to_base(path),crit_err);
	}
	TEST(correct_path){
		Connector_to_base Con;
		std::string path = "test_files/base.txt";
		CHECK_EQUAL(0,Con.connect_to_base(path));
	}
	TEST(check_reading){
		Connector_to_base Con;
		std::string check_pass = "pass";
		std::string check_login = "login";
		std::string path = "test_files/base.txt";
		Con.connect_to_base(path);
		if((Con.get_data()[check_login]!= check_pass) or (Con.get_data().find(check_pass)==Con.get_data().end())){
			CHECK(false);
		}else{
			CHECK(true);
		}
	}
}
//
//Тест Калькулятора (Класс Calculator)
//
SUITE(Calculator_test){
	TEST(positiv_overflow){
		std::vector<double> v = {1.7e+307,1000000};
		Calculator c(v);
		CHECK_EQUAL(inf,c.send_res());
	}
	TEST(negativ_overflow){
		std::vector<double> v = {-2.2250738585072014e307,100000000};
		Calculator c(v);
		CHECK_EQUAL(-inf,c.send_res());
	}
	TEST(negative_number_1){
		std::vector<double> v ={1000,-1000};
		Calculator c(v);
		CHECK_EQUAL(-1000000,c.send_res());
	}
	TEST(negative_number_2){
		std::vector<double> v ={-1000,-1000};
		Calculator c(v);
		CHECK_EQUAL(1000000,c.send_res());
	}
	
}
//
//Тест создания/открытия лог файла (Класс Logger)
//
SUITE(Logger_test){
	TEST(wrong_path){
		Logger l;
		std::string path = "Не/путь/к/файлу.txt";
		CHECK_THROW(l.set_path(path),crit_err);
	}
	TEST(empty_path){
		Logger l;
		std::string path = "";
		CHECK_THROW(l.set_path(path),std::invalid_argument);
	}
	TEST(correct_path){
		Logger l;
		std::string path = "test_files/test_log.txt";
		CHECK_EQUAL(0,l.set_path(path));
	}
	TEST(newline_in_path_log){
		Logger l;
		std::string path = "test_files/log\n";
		CHECK_THROW(l.set_path(path),std::invalid_argument);
	}
}
//
//Тест md5 хеширования и функции генерации SALT (Класс Client_Communicate)
//
SUITE(Client_Communicate){
	TEST(salt_generator){
		Client_Communicate com;
		std::string salt_one = com.generate_salt();
		std::this_thread::sleep_for(std::chrono::seconds(1)); //Исскуственная задержка
		std::string salt_two = com.generate_salt();
		if(salt_one == salt_two){
			CHECK(false);
		}else{CHECK(true);}
	}
	TEST(salt_len){
		Client_Communicate com;
		std::string salt = com.generate_salt();
		CHECK_EQUAL(salt.length(),16);
	}
	TEST(md5_gen){
		std::string hash_check = "5E97E942837598CA32678F7010575554";
		Client_Communicate com;
		std::string SALT = "3e74235568ba8f1e";
		std::string hash = com.md5(SALT);
		CHECK_EQUAL(hash,hash_check);
	}
}
//Запуск тестов
int main(int argc, char **argv)
{
    return UnitTest::RunAllTests();
}
