if g++ -g -std=c++14 -o test main.cpp auto_result.cpp
then
	echo Build Success
	./test
else
	echo Build Failed
fi


