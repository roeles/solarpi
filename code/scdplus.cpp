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
	sleep(1);
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
	//const size_t monitor_command_length 	= (monitor_command == NULL ? 0 : strlen(monitor_command));
	//const size_t init_command_length 	= (init_command == NULL ? 0 : strlen(init_command));

	const size_t line_buffer_length = 256;
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
/*
	cout << "time_before.tv_sec" << delim << "time_before.tv_nsec" << delim << "time_after.tv_sec" << delim << "time_after.tv_nsec" << delim;
	cout << "stage1_voltage" << delim << "stage2_voltage" << delim << "stage3_voltage" << delim;
	cout << "stage1_length" << delim << "stage2_length" << delim;
	cout << "discharge_threshold_low" << delim << "discharge_threshold_high" << delim;
	cout << "year" << delim << "month" << delim << "dom" << delim << "hour" << delim << "minute" << delim << "sec" << delim;
	cout << "ah100" << delim << "ah" << delim;
	cout << "kwh100" << delim << "kwh" << endl << flush;
*/
//	while(true)
	{
		const char * command = "pc.solar.getparams";
		const size_t command_length = strlen(command);

		const char * expected_response = "solar.pc.sendparams.";
		const size_t expected_response_length = strlen(expected_response);

		const size_t response_length = 1024; //18+27;
		char response[response_length];

		struct timespec time_before, time_after;
		clock_gettime(CLOCK_REALTIME, &time_before);

		const size_t command_result = sendcommand(serial_port, command, command_length, response, response_length);
	
		const char * response_values = &response[expected_response_length];
		const unsigned int stage1_voltage 	= response_values[0] + 79;
		const unsigned int stage2_voltage 	= response_values[1] + 79;
		const unsigned int stage3_voltage 	= response_values[2] + 79;
		const unsigned int stage1_length	= response_values[3] - 1;
		const unsigned int stage2_length 	= response_values[4] - 1;
		const unsigned int dis_lo_thres	= response_values[5] + 79;
		const unsigned int dis_hi_thres	= response_values[6] + 79;
		
		const unsigned int year		= ((unsigned int)response_values[14]) + 1999;
		const unsigned int month	= ((unsigned int)response_values[15]) + 1;
		const unsigned int dom		= ((unsigned int)response_values[16]) + 1;
		const unsigned int hour		= ((unsigned int)response_values[17]) + 1;
		const unsigned int min		= ((unsigned int)response_values[18]) + 1;
		const unsigned int sec		= ((unsigned int)response_values[19]) + 1;

		const unsigned ah100		= ((unsigned int)response_values[20])-1;
		const unsigned ah		= ((unsigned int)response_values[21])-1;
		
		const unsigned kwh100		= ((unsigned int)response_values[22])-1;
		const unsigned kwh		= ((unsigned int)response_values[23])-1;

		const char * command_getdata = "pc.solar.getdata";
		const size_t command_getdata_length = strlen(command_getdata);
	
		clock_gettime(CLOCK_REALTIME, &time_after);
//		printf("%d,%d,%d,%d,%s\n", time_before.tv_sec, time_before.tv_nsec, time_after.tv_sec, time_after.tv_nsec, line_buffer);
		cout << time_before.tv_sec << delim << time_before.tv_nsec << delim << time_after.tv_sec << delim << time_after.tv_nsec << delim;
		cout << stage1_voltage << delim << stage2_voltage << delim << stage3_voltage << delim;
		cout << stage1_length << delim << stage2_length << delim;
		cout << dis_lo_thres << delim << dis_hi_thres << delim;
		cout << year << delim << month << delim << dom << delim << hour << delim << min << delim << sec << delim;
		cout << ah100 << delim << ah << delim;
		cout << kwh100 << delim << kwh;

		const char * command_getdata_expected_response = "solar.pc.senddata";
		const size_t command_getdata_expected_response_length = strlen(command_getdata_expected_response);

		const size_t command_getdata_result = sendcommand(serial_port, command_getdata, command_getdata_length, response, response_length);
		
		for(size_t i=command_getdata_expected_response_length; i<command_getdata_result; i++)
			cout << ((unsigned int)response[i]) << ",";




		cout << endl << flush;
//		fflush(stdout);
	}
	
	return 0;
};
