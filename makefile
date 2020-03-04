recommendation:recommendation.o User.o Distances.o Tweet.o CryptoCurrencyRecommendation.o InputOutput.o ClusterManagement.o HashManagementLSH.o HashTable.o HashTableElement.o  Point.o Cosine.o Euclidean.o
	g++ recommendation.o CryptoCurrencyRecommendation.o Distances.o  User.o Tweet.o InputOutput.o ClusterManagement.o HashManagementLSH.o HashTable.o HashTableElement.o  Point.o Cosine.o Euclidean.o -o recommendation
		rm -f *.o

recommendation.o:recommendation.cpp
	g++ -Wall -g3 -c recommendation.cpp


CryptoCurrencyRecommendation.o:CryptoCurrencyRecommendation/CryptoCurrencyRecommendation.cpp CryptoCurrencyRecommendation/CryptoCurrencyRecommendation.h
	g++ -Wall -g3 -c CryptoCurrencyRecommendation/CryptoCurrencyRecommendation.cpp

User.o:CryptoCurrencyRecommendation/User/User.cpp CryptoCurrencyRecommendation/User/User.h
	g++ -Wall -g3 -c CryptoCurrencyRecommendation/User/User.cpp

Tweet.o:CryptoCurrencyRecommendation/Tweet/Tweet.cpp CryptoCurrencyRecommendation/Tweet/Tweet.h
	g++ -Wall -g3 -c CryptoCurrencyRecommendation/Tweet/Tweet.cpp

InputOutput.o:IO/InputOutput.h IO/InputOutput.cpp
	g++ -Wall -g3 -c IO/InputOutput.cpp

ClusterManagement.o:ClusterManagement/ClusterManagement.cpp ClusterManagement/ClusterManagement.h
	g++ -Wall -g3 -c ClusterManagement/ClusterManagement.cpp


HashManagementLSH.o:HashManagementLSH/HashManagementLSH.h HashManagementLSH/HashManagementLSH.cpp
	g++ -Wall -g3 -c HashManagementLSH/HashManagementLSH.cpp
HashTable.o:HashManagementLSH/HashTable/HashTable.cpp HashManagementLSH/HashTable/HashTable.h
	g++ -Wall -g3 -c HashManagementLSH/HashTable/HashTable.cpp
HashTableElement.o:HashManagementLSH/HashTable/HashTableElement.cpp HashManagementLSH/HashTable/HashTableElement.h
	g++ -Wall -g3 -c HashManagementLSH/HashTable/HashTableElement.cpp

Point.o:Point/Point.h Point/Point.cpp
	g++ -Wall -g3 -c  Point/Point.cpp

Distances.o:Point/Distances.h Point/Distances.cpp
	g++ -Wall -g3 -c  Point/Distances.cpp


Cosine.o:HashFunctions/Cosine/Cosine.h HashFunctions/HashFunction.h
	g++ -Wall -g3 -c  HashFunctions/Cosine/Cosine.cpp

Euclidean.o:HashFunctions/Euclidean/Euclidean.h HashFunctions/HashFunction.h
	g++ -Wall -g3 -c HashFunctions/Euclidean/Euclidean.cpp





clean:
	rm -f cluster
	rm -f *.o
