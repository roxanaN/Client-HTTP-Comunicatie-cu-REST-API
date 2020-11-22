#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "parson.h"
#include "functions.h"

/* Functie care parseaza un obiect de tip JSON,
   pentru a extrage informatii despre url, metoda, tip, user, parola, token si id */
void get_details(JSON_Object *response_object, const char **url,
                 const char **method, const char **type, const char **username,
                 const char **password, const char **token, const char **id) {
    JSON_Object *data;
    JSON_Object *queryParams;

    *url = json_object_get_string(response_object, "url");
    *method = json_object_get_string(response_object, "method");
    *type = json_object_get_string(response_object, "type");

    data = json_object_get_object(response_object, "data");
    *username = json_object_get_string(data, "username");
    *password = json_object_get_string(data, "password");
    *token = json_object_get_string(data, "token");

    queryParams = json_object_get_object(data, "queryParams");
    *id = json_object_get_string(queryParams, "id");
}

/* Functie care parseaza un obiect de tip JSON,
   pentru a extrage informatii despre url si metoda */
void get_some_details(JSON_Object *response_object, const char **url,
                 const char **method) {
    *url = json_object_get_string(response_object, "url");
    *method = json_object_get_string(response_object, "method");                 
}

// Functie care extrage informatiile necesare pentru ultima etapa
void get_weather_details(JSON_Object *response_object, const char **url,
                    const char **method, const char **type,
                    const char **weather_url, const char **q, const char **APPID,
                    const char **weather_method) {
    JSON_Object *data;
    JSON_Object *queryParams;

    *url = json_object_get_string(response_object, "url");
    *method = json_object_get_string(response_object, "method");
    *type = json_object_get_string(response_object, "type");

    data = json_object_get_object(response_object, "data");
    *weather_url = json_object_get_string(data, "url");
    *weather_method = json_object_get_string(data, "method");

    queryParams = json_object_get_object(data, "queryParams");
    *q = json_object_get_string(queryParams, "q"); 
    *APPID = json_object_get_string(queryParams, "APPID");                  
}

/* Functie care primeste ca parametru raspunsul dat de catre server
   si returneaza obiectul de tip JSON */
JSON_Object *get_response_object(char *response) {
    // Pointez la inceputul obiectului de tip JSON
    char *json_ptr = NULL;
    json_ptr = strchr(response, '{') - 1;

    return json_value_get_object(json_parse_string(json_ptr));
}

// Functie care concateneaza cookie-urile
char *get_cookies(char *response) {
    char *all_cookies = calloc(MAXLEN, sizeof(char));
    char *stop = NULL;
    char *cookie_start = NULL;
    char *body_start;
    int over_the_cookie = strlen("Set-Cookie: ");

    char cookie[COOKIELEN];
    memset(all_cookies, 0, MAXLEN);

    // Pointez la prima prajiturica
    cookie_start = strstr(response, "Set-Cookie");

    // Cat timp exista prajiturici
    while (cookie_start) {
        // Tai prajiturica, pana la primul ";"
        stop = strchr(cookie_start, ';');
        body_start = cookie_start + over_the_cookie;

        // Pun prajiturica pe falfurie
        memset(cookie, 0, COOKIELEN);
        memcpy(cookie, body_start, (stop - body_start));
        cookie[strlen(cookie)] = '\0';

        // Pun farfuria pe masa cu prajiturici
        strcat(all_cookies, cookie);
        strcat(all_cookies, "; ");

        // Trec la urmatoarea prajiturica
        cookie_start = strstr(stop, "Set-Cookie");
    }

    return all_cookies;
}

void cut_url(const char **url) {
    *url = strstr(*url, "/data");
}

// Functie care primeste un nume si intoarce adresa IP asociata
char *get_ip(const char *hostname) {
    char *ip = calloc(IPLEN, sizeof(char));
	struct addrinfo hints, *result;

	memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
	getaddrinfo(hostname, NULL, &hints, &result);
	inet_ntop(result->ai_family, &((struct sockaddr_in *) result->ai_addr)->sin_addr, ip, IPLEN);

	freeaddrinfo(result);

    return ip;
}

// Functie care extrage numele din url
char *get_name(const char *url) {
    char *stop = strchr(url, '/');
    char *hostname = calloc(URLLEN, sizeof(char));

    memcpy(hostname, url, (stop - url));
    hostname[strlen(hostname)] = '\0';

    return hostname;
}

// Functie care extrage raspunsul dat de catre server, referitor la vreme
char *get_weather(const char *response) {
    char *stop = strchr(response, '{');
    char *weather = calloc(MAXLEN, sizeof(char));

    memcpy(weather, stop, strlen(stop));
    weather[strlen(weather)] = '\0';

    return weather;
}
