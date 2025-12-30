@echo off
echo ===========================================
echo Сборка библиотек и программы
echo ===========================================

echo 1. Очистка старых файлов...
del *.o *.exe *.dll *.lib 2>nul

echo 2. Компиляция статической библиотеки Number...
g++ -c Number/number.cpp -o number.o
ar rcs Number.lib number.o

echo 3. Компиляция динамической библиотеки Vector...
g++ -c Vector/vector.cpp -o vector.o -I. -DBUILD_VECTOR_DLL
g++ -shared -o Vector.dll vector.o Number.lib -Wl,--out-implib,Vector.lib

echo 4. Компиляция основной программы...
g++ main.cpp -o main.exe -I. -L. -lNumber -lVector

echo 5. Запуск программы...
echo ===========================================
main.exe
echo ===========================================
pause