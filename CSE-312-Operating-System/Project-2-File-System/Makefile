disk:
	gcc -o makeFileSystem makeFileSystem.c
	./makeFileSystem 1 fileSystem.data
ops:
	gcc -o fileSystemOper fileSystemOper.c

clean:
	rm makeFileSystem fileSystemOper

ilhanHoca:
	./fileSystemOper fileSystem.data mkdir /usr
	./fileSystemOper fileSystem.data mkdir /usr/ysa
	./fileSystemOper fileSystem.data mkdir /bin/ysa 
	./fileSystemOper fileSystem.data write /usr/ysa/file1 ./linuxFile.data
	./fileSystemOper fileSystem.data write /usr/file2 ./linuxFile.data
	./fileSystemOper fileSystem.data write /file3 ./linuxFile.data
	./fileSystemOper fileSystem.data dir /
	./fileSystemOper fileSystem.data del /usr/ysa/file1
	./fileSystemOper fileSystem.data dumpe2fs
	./fileSystemOper fileSystem.data read /usr/file2 ./linuxFile2.data
	cmp ./linuxFile.data ./linuxFile2.data
	./fileSystemOper fileSystem.data chmod /usr/file2 -rw
	./fileSystemOper fileSystem.data read /usr/file2 ./linuxFile2.data
	./fileSystemOper fileSystem.data chmod /usr/file2 +rw
	./fileSystemOper fileSystem.data addpw /ysa/file2 test1234
	./fileSystemOper fileSystem.data addpw /usr/file2 test1234
	./fileSystemOper fileSystem.data read /usr/file2 ./linuxFile2.data
	./fileSystemOper fileSystem.data read /usr/file2 ./linuxFile2.data test1234
