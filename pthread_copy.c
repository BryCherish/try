#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/mman.h>
#include<pthread.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>

int pthread_num = 5;
int *pthread_done;
typedef struct file
{
	char *src;
	char *des;
	int threadNumber;
	int allsize;
}File;
void *copy(void *arg)
{
	File* file = (File*)arg;
	int each_size = (file->allsize % pthread_num == 0) ? (file->allsize / pthread_num) : (file->allsize / pthread_num +1);
	//计算偏移量
	int offset = file->threadNumber * each_size;
	memcpy(file->des+offset,file->src+offset,each_size);
}

int Create_thread( char*src, char *des)
{
	int sfd,dfd,size;
	if( (sfd = open(src,O_RDWR)) ==-1)
	{
		perror("open failed");
		exit(0);
	}
	if( (dfd = open(des,O_RDWR|O_CREAT,0664)) ==-1)
	{
		perror("des create failed");
		exit(0);
	}
	if( (size = lseek(sfd,0,SEEK_END))==-1)
	{
		perror("lseek failed");
		exit(0);
	}
	//拓展空文件
	ftruncate(dfd,size);
	//mmap
	char *mmap_src=mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,sfd,0);
	char *mmap_des=mmap(NULL,size,PROT_WRITE|PROT_READ,MAP_SHARED,dfd,0);
	//关闭文件描述符
	close(sfd);
	close(dfd);
	//线程创建
	pthread_t tid[pthread_num];
	File *file = (File*)malloc(sizeof(File)*pthread_num);
	for(int i=0;i<pthread_num;i++)
	{
		file[i].src=mmap_src;
		file[i].des=mmap_des;
		file[i].threadNumber=i;
		file[i].allsize=size;
		pthread_create(&tid[i],NULL,copy,(void*)&file[i]);
	}
	//回收线程
	for(int i=0;i<pthread_num;i++)
	{
		pthread_join(tid[i],NULL);
	}
	munmap(mmap_src,size);
	munmap(mmap_des,size);

}

/*void ProBar()
  {
  int rate=0;
  char str[102];
  memset(str,0,sizeof(char)*102);
  const char* ptr="|/-\\";
  while(rate<=100)
  {
  str[rate]='=';
  printf("[%-100s][%d\%][%c]\r",str,rate,ptr[rate%4]);
  usleep(100000);
  fflush(stdout);
  rate++;
  }
  printf("\n");
  }*/
int main(int argc,char **argv)
{
	if(argc<3)
	{
		printf("参数过少..\n");
		exit(0);
	}
	if( (access(argv[1],F_OK)!=0))
	{
		printf("file no exits..\n");
		exit(0);
	}
	if(argv[3]!=0)
	{
		pthread_num=atoi(argv[3]);
		if(pthread_num<0 || pthread_num>100)
		{
			printf("线程数目错误..\n");
			exit(0);
		}
	}
	Create_thread(argv[1],argv[2]);

	return 0;
}
