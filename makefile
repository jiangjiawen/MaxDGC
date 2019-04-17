getclx: main.o readnet.o rwrgetseedcluster.o clusteringprocess.o expansionandmerge.o
	g++ -o getclx main.o readnet.o rwrgetseedcluster.o clusteringprocess.o expansionandmerge.o
main.o: main.cpp readnet.h readnet.cpp
	g++ -c main.cpp
readnet.o: readnet.cpp readnet.h
	g++ -c readnet.cpp
rwrgetseedcluster.o: rwrgetseedcluster.cpp rwrgetseedcluster.h
	g++ -c rwrgetseedcluster.cpp
clusteringprocess.o: clusteringprocess.cpp clusteringprocess.h
	g++ -c clusteringprocess.cpp
expansionandmerge.o: expansionandmerge.cpp expansionandmerge.h
	g++ -c expansionandmerge.cpp

clean:
	rm main.o readnet.o rwrgetseedcluster.o clusteringprocess.o expansionandmerge.o
