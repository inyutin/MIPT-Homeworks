### Запуск тестов:

cxxtestgen --error-printer -o runner.cpp ./test/EquationsTest.hpp  
g++ -o runner -I$CXXTEST runner.cpp  
./runner  

Требуется cxxtestgen
