if g++ -g -std=c++14 -o test main.cpp result.cpp
then
	echo Build Success
	./test
else
	echo Build Failed
fi


