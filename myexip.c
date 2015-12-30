/*
 * myexip
 * Alvaro Miranda
 * 20151229
 *
 * C cli to query myexternalip.com
 *
 * usage:
 * ./myexip [-6] [-h]
 *
 * inspired on
 * http://stackoverflow.com/questions/897027/i-want-to-store-a-result-for-character-string-with-libcurl
 * http://marc.info/?l=curl-library&m=100204184522546&w=2
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>

struct str {
    char*  bytes;
    size_t len;
};

static int str_append(struct str* s, const char* b, size_t len_b) {

    const size_t len_total = s->len + len_b;
    char* bytes = realloc(s->bytes, len_total);
    if (bytes != NULL) {
        s->bytes = bytes;
        strncpy(s->bytes + s->len, b, len_b);
        s->len = len_total;
        return 1;
    }
    return 0;
}

static size_t store_http_body(void* ptr, size_t size, size_t nmemb, void* user) {

    const size_t len = size * nmemb;
    str_append((struct str*)user, (char*)ptr, len);
    return len;
}

int main(int argc, char **argv) {

    static const char USER_AGENT[] = "myexip/tiny c cli to myexip";
    static const char V4_URL[]     = "http://ipv4.myexternalip.com/raw";
    static const char V6_URL[]     = "http://ipv6.myexternalip.com/raw";

    const char* url = V4_URL; // default to ipv4
    long        http_code = 0;
    int         c; // getopt counter

    opterr = 0;
    while ((c = getopt (argc, argv, "h6")) != -1){
        switch (c){
            case 'h':
                printf("use: %s [-6]\n", argv[0]); // basic usage
                return 0;
            case '6':
                url = V6_URL; // override to ipv6
                break;
            case '?':
                printf("unknown arg %c\n", optopt);
                return 1;
        } // end switch
    } // end while

    CURL*       curl_handle;
    CURLcode    res;
    char        err_buf[CURL_ERROR_SIZE] = { 0 };
    struct str  http_body = { NULL, 0 };

    curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, USER_AGENT);
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, store_http_body);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&http_body);
    curl_easy_setopt(curl_handle, CURLOPT_ERRORBUFFER, err_buf);

    res = curl_easy_perform(curl_handle);
    curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &http_code);

    if (res == CURLE_OK && http_code == 200 ) {
        write(1, http_body.bytes, http_body.len);
        return 0;
    }

    if (strlen(err_buf) > 0) {
        printf("libcurl: %s\n", err_buf);
    } else {
        printf("libcurl: %ld %s\n", http_code, url);
    }

    return 1;
}
