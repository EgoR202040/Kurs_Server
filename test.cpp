#include "Includer.h"
#include <limits>
#include <chrono>
#include <thread>
#include <UnitTest++/UnitTest++.h>
#define inf numeric_limits<double>::infinity()
//
//Тест подключения к базе данных (Класс Connector_to_base)
//
SUITE(Connector_test){
	TEST(digits_in_path){
		Connector_to_base Con;
		std::string path = "/home/stud/Kurs_Server/test_temp_files/base0.txt";
		CHECK_EQUAL(0,Con.connect_to_base(path));
	}
	TEST(newline_in_path){
		Connector_to_base Con;
		std::string path = "/home/stud/Kurs_Server/test_temp_files/base\n";
		CHECK_THROW(Con.connect_to_base(path),std::invalid_argument);
	}
	TEST(incorrect_path){
		Connector_to_base Con;
		std::string path = "#5df%s";
		CHECK_THROW(Con.connect_to_base(path),std::invalid_argument);
	}
	TEST(empty_path){
		Connector_to_base Con;
		std::string path = "";
		CHECK_THROW(Con.connect_to_base(path),std::invalid_argument);
	}
	TEST(wrong_path){
		Connector_to_base Con;
		std::string path = "/tesotfsa/base.txt";
		CHECK_THROW(Con.connect_to_base(path),std::invalid_argument);
	}
	TEST(correct_path){
		Connector_to_base Con;
		std::string path = "Server/base.txt";
		CHECK_EQUAL(0,Con.connect_to_base(path));
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
	TEST(incorrect_data){
		std::vector<double> v = {"abcd","abd"};
		Calculator c(v);
		CHECK_EQUAL(0,c.send_res());
	}
	TEST(negativ_overflow){
		std::vector<double> v = {2.2250738585072014e-308,0.00000000000000000000000000000001};
		Calculator c(v);
		CHECK_EQUAL(0,c.send_res());
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
	TEST(digits_in_path_log){
		Logger l;
		std::string path = "/home/stud/Kurs_Server/test_temp_files/l0g.txt";
		CHECK(true);
	}
	TEST(wrong_path){
		Logger l;
		std::string path = "/tesotfsa/test/path/to/log";
		CHECK_THROW(l.set_path(path),std::invalid_argument);
	}
	TEST(empty_path){
		Logger l;
		std::string path = "";
		CHECK_THROW(l.set_path(path),std::invalid_argument);
	}
	TEST(correct_path){
		Logger l;
		std::string path = "test_log.txt";
		CHECK_EQUAL(0,l.set_path(path));
	}
	TEST(newline_in_path_log){
		Logger l;
		std::string path = "/home/stud/Kurs_Server/test_temp_files/log\n";
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
	TEST(md5_gen){
		Client_Communicate com;
		std::string SALT = com.generate_salt();
		std::string hash_one = com.md5(SALT);
		std::string hash_two = com.md5(SALT);
		if(hash_one == hash_two){
			CHECK(true);
		}else{CHECK(false);}
	}
}
//Запуск тестов
int main(int argc, char **argv)
{
    return UnitTest::RunAllTests();
}
