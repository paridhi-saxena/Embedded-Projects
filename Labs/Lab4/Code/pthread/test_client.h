#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

/* Since this program depends on libcurl, you may need to 
 * install libcurl4-gnutls-dev.
 *
 * On Linux Mint, this can be done by running:
 * sudo apt-get install libcurl4-gnutls-dev
 */

char *getDateTime() {
    char *array = (char*)malloc(sizeof(char)*20);
    memset (array, 0, 20);
    time_t rawtime;
    rawtime = time(NULL);
    struct tm  *timeinfo = localtime (&rawtime);
    strftime(array, 20, "%d.%m.%Y-%H:%M:%S", timeinfo);
    array[20] = '\0';
    return array;
}

void HTTP_POST(const char* url, const char* data, int size) {
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, url);
		printf("url id %s\n", url);
                curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE,(long)size);

                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
		printf("before easy perform\n");
		res = curl_easy_perform(curl);
		printf("after easy\n");
		if(res != CURLE_OK)
      			fprintf(stderr, "curl_easy_perform() failed: %s\n",
              			curl_easy_strerror(res));
		curl_easy_cleanup(curl);
	}
}

int call_POST(char* datafile, int adc, char* sensorstatus){
	const char* hostname="192.168.0.19";
	const int   port=8000;
	const int   id=9;
	const char* password="deepblue";
	const char* name="Team+Awesome";
	const int   adcval=adc;
	const char* status=sensorstatus;
	const char* timestamp=getDateTime();
	const char* filename=datafile;
	printf("file is %s\n", datafile);

	char buf[1024];
	snprintf(buf, 1024, "http://%s:%d/update?id=%d&password=%s&name=%s&data=%d&status=%s&timestamp=%s&filename=%s",
			hostname,
			port,
			id,
			password,
			name,
			adcval,
			status,
            		timestamp,
			filename);
	FILE *fp;
	
	struct stat num;
	stat(filename, &num);
	int size = num.st_size;
	printf("Image size: %dB\n", size);	

	char *buffer = (char*)malloc(size);

	fp = fopen(filename,"rb");
	int n = fread(buffer, 1, size, fp);

	HTTP_POST(buf, buffer, size);
	fclose(fp);
	//free(buffer);
	return 0;
}
