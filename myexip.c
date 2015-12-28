/*
 * myexip
 * Alvaro Miranda
 *
 * C cli to query myexternalip.com
 *
 * usage:
 * ./myexip [-6] [-h]
 *
 */

#include <stdio.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <curl/curl.h>

struct ip { char ip[50]; };
struct ip myip;

size_t  write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    char buf[size*nmemb+1];
    char * pbuf = &buf[0];
    memset(buf, '\0', size*nmemb+1);
    size_t i = 0;
    for(;  i < nmemb ; i++){
        strncpy(pbuf,ptr,size);
        pbuf += size;
        ptr += size;
    }
    strncpy(myip.ip,buf, sizeof(myip.ip));
    return size * nmemb;
}

int main(int argc, char **argv)
{
    char *url = "http://ipv4.myexternalip.com/raw"; // default to ipv4
    long http_code = 0;

    int c; // getopt counter

    opterr = 0;
    while ((c = getopt (argc, argv, "h6v:")) != -1){
        switch (c)
        {
            case 'h':
                printf("use: %s [-6]\n",argv[0]);
                exit(0);
            case '6':
                url = "http://ipv6.myexternalip.com/raw";
                break;
            case '?':
                printf("unknown arg %c\n", optopt);
                exit(1);
        } // end switch
    } // end while

    CURL *curl_handle;
    curl_handle = curl_easy_init();

    curl_easy_setopt(curl_handle,   CURLOPT_URL, url);
    curl_easy_setopt(curl_handle,   CURLOPT_NOPROGRESS  ,1);
    curl_easy_setopt(curl_handle,   CURLOPT_WRITEFUNCTION,&write_data);
    curl_easy_perform(curl_handle);
    curl_easy_getinfo (curl_handle, CURLINFO_RESPONSE_CODE, &http_code);

    if (CURLE_OK == 0 && http_code == 200 ){
        printf("%s\n",myip.ip);
    } else {
        exit(1);
    }

    curl_easy_cleanup(curl_handle);
    return 0;
}