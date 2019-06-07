
# Compiling everything

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

cd BengiLIB/BengiSO
echo "Compiling shared object file..." 
make clean
make bengi
cd ..

cd BengiUnitTest
echo "Compiling run_tests..."
make clean
make run_tests
cd ..

# Compiling and running tests
cd tests
echo "Compilin tests w/python script..."
python3 compile.py

echo "Running tests using VM shared object w/python script..."
python3 ben.py --test
cd ..