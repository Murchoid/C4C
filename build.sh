
make run
if [ $? -ne 0 ];then
	exit
fi
cd tests
./build.sh
cd -
