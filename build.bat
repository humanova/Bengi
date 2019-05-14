
::Compiling everything

cd basm
echo "Compiling basm..." 
make clean
make basm
cd ..

cd Bengi
echo "Compiling bengivm..." 
make clean
make bengi
cd ..

cd BengiUnitTest
echo "Compiling run_tests..."
make clean
make run_tests
cd ..

::Compiling and running tests
cd tests
echo "Compilin tests w/python script..."
python compile.py

echo "Running tests using VM shared object w/python script..."
python ben.py --test
cd ..