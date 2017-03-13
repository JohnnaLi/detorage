
CC=gcc
CPPFLAGS= -I./include
CFLAGS=-Wall 
LIBS= 

#找到当前目录下所有的.c文件
src = $(wildcard *.c)

#将当前目录下所有的.c  转换成.o给obj
obj = $(patsubst %.c, %.o, $(src))


main=main
fdfs_upload_test=fdfs_upload


target=$(main) $(fdfs_upload_test)


ALL:$(target)



#main程序
$(main):./main.o make_log.o 
	$(CC) $^ -o $@ $(LIBS)

#fdfs_upload_test程序
$(fdfs_upload_test):./fdfs_upload_test.o make_log.o 
	$(CC) $^ -o $@ $(LIBS)


#生成所有的.o文件
$(obj):%.o:%.c
	$(CC) -c $< -o $@ $(CPPFLAGS) $(CFLAGS) 


#clean指令

clean:
	-rm -rf $(obj) $(target) ./test/*.o

distclean:
	-rm -rf $(obj) $(target) ./test/*.o

#将clean目标 改成一个虚拟符号
.PHONY: clean ALL distclean
