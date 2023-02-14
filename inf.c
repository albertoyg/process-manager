#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	if (argc != 3) {
		fprintf(stderr, "Usage: inf tag interval\n");
	} else {
		const char* tag = argv[1];
		int interval = atoi(argv[2]);
		while(1) {
			int i = 0;
            while(i < 10000)
            {
                int j = 0;
                while(j < 1000)
                    j++;
                i++;
            }
			printf("%s\n", tag);
			sleep(interval);
		}
	}
	exit(0);
}
