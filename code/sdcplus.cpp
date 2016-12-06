#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>

#include <iostream>
using namespace std;

#define BAUDRATE B9600

size_t sendcommand(int fd, const char * buf, const size_t len, char * recv_buf, const size_t recv_len)
{
	const size_t write_result = write(fd, buf, len);
	if(write_result < 0)
	{
		fprintf(stderr, "%s: write(2) failed %d %s\n",  __PRETTY_FUNCTION__, errno, strerror(errno));
		return 0;
	}
	const size_t read_result = read(fd, recv_buf, recv_len);
	return read_result;
}

int main(int argc, char ** argv)
{
	char * monitor_command = NULL;
	char * init_command = NULL;
	const char * delim = ",";
	struct termios m_oldtio,m_newtio;

	if(argc < 3)
	{
		fprintf(stderr, "Usage: %s device baudrate measure_string init_string\n", argv[0]);
		fprintf(stderr, "Usage: %s device baudrate measure_string\n", argv[0]);
		fprintf(stderr, "Usage: %s device baudrate\n", argv[0]);
		return -1;
	}	

	if(argc > 3)	monitor_command = argv[3];
	if(argc > 4)	init_command = argv[4];
	const size_t monitor_command_length 	= (monitor_command == NULL ? 0 : strlen(monitor_command));
	const size_t init_command_length 	= (init_command == NULL ? 0 : strlen(init_command));

	const size_t line_buffer_length = 256;
	char line_buffer[line_buffer_length];
	int serial_port = open(argv[1], O_RDWR | O_NOCTTY);
	if(serial_port == -1)
	{
		fprintf(stderr, "%s: error in open(2): %d %s\n", __PRETTY_FUNCTION__, errno, strerror(errno));
		return -1;
	}
	
	tcgetattr(serial_port, &m_oldtio);
        bzero(&m_newtio, sizeof(m_newtio));
        m_newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
        m_newtio.c_iflag = IGNPAR;
        m_newtio.c_oflag = 0;
        m_newtio.c_lflag = 0;
        m_newtio.c_cc[VTIME] = 0;
        m_newtio.c_cc[VMIN] = 1;

	tcflush(serial_port, TCIFLUSH);
	const int tcset_result = tcsetattr(serial_port, TCSANOW, &m_newtio);
	if(tcset_result < 0)
	{
		fprintf(stderr, "%s: error in tcsetattr(3): %d %s\n", __PRETTY_FUNCTION__, errno, strerror(errno));
		return -1;
	}

//	sendcommand(serial_port, init_command, init_command_length);
	while(true)
	{
		const char * command = "pc.solar.getparams"'
		const size_t command_length = strlen(command);

		const size_t response_length = 18+24;
		char * response[response_length];

		struct timespec time_before, time_after;
		clock_gettime(CLOCK_REALTIME, &time_before);

		const size_t command_result = sendcommand(serial_port, command, command_length, response, response_length);
			
		clock_gettime(CLOCK_REALTIME, &time_after);
//		printf("%d,%d,%d,%d,%s\n", time_before.tv_sec, time_before.tv_nsec, time_after.tv_sec, time_after.tv_nsec, line_buffer);
		cout << time_before.tv_sec << delim << time_before.tv_nsec << delim << time_after.tv_sec << delim << time_after.tv_nsec << delim << command_result << delim << response_length << endl << flush;
//		fflush(stdout);
		sleep(1);
	}
	
	return 0;
};
