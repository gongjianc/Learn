#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void)
{
	char block[1024];
	int in,out;
	int nread;

	in = open("data",O_RDONLY);
	out = open("data.out2",O_WRONLY | O_CREAT,S_IRUSR|S_IWUSR);

	while((nread = read(in,block,sizeof(block))) > 0)
		write(out,block,nread);

	return 0;

}
