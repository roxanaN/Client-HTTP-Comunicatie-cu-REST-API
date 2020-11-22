#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "parson.h"
#include "functions.h"
#include "helpers.h"
#include "requests.h"

int main(int argc, char *argv[]) {
    JSON_Object *response_object = NULL;
    char *message;
    char *response;
    char *ip = NULL;
    char *name = NULL;
    char *weather = NULL;
    char *cookies = NULL;
    const char *url = NULL;
    const char *method = NULL;
    const char *type = NULL;
    const char *username = NULL;
    const char *password = NULL;
    const char *token = NULL;
    const char *id = NULL;
    const char *weather_url = NULL;
    const char *q = NULL;
    const char *APPID = NULL;
    const char *weather_method = NULL;
    int sockfd;

    // Etapa 1
    // Request
    sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request(IP_SERVER, "/task1/start", NULL, NULL, NULL);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s \n", response);

    close_connection(sockfd);

    /* Etapa 2
    *  Se parseaza raspunsul primit anterior.
    *  Se face request, folosind valorile obtinute in urma parsarii
    */
    response_object = get_response_object(response);
    get_details(response_object, &url, &method, &type, &username, &password, &token, &id);
    cookies = get_cookies(response);

    // Credentialele pentru logare, respectand structura prezentata in enunt
    char *login = calloc(LOGINLEN, sizeof(char));
    sprintf(login, "username=%s&password=%s", username, password);

    // Request
    sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    if (!strcmp(method, "POST")) {
        message = compute_post_request(IP_SERVER, url, login, type, token, cookies);
    } else {
        message = compute_get_request(IP_SERVER, url, login, token, cookies);
    }
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s \n", response);

    close_connection(sockfd);
    
    // Etapa 3
    response_object = get_response_object(response);
    get_details(response_object, &url, &method, &type, &username, &password, &token, &id);
    cookies = get_cookies(response);

    // Raspunsurile ghicitorilor si id-ul
    char *url_params = calloc(PARAMLEN, sizeof(char));
    sprintf(url_params, "raspuns1=omul&raspuns2=numele&id=%s", id);

    // Request
    sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    if (!strcmp(method, "GET")) {
        message = compute_get_request(IP_SERVER, url, url_params, token, cookies);
    } else {
        message = compute_post_request(IP_SERVER, url, url_params, type, token, cookies);
    }
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s \n", response);

    close_connection(sockfd);

    // Etapa 4
    // Extragem din raspunsul primit url-ul, metoda si cookies-urile
    response_object = get_response_object(response);
    get_some_details(response_object, &url, &method);
    cookies = get_cookies(response);

    // Request
    sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    if (!strcmp(method, "GET")) {
        message = compute_get_request(IP_SERVER, url, NULL, token, cookies);
    } else {
        message = compute_post_request(IP_SERVER, url, NULL, type, token, cookies);
    }
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s \n", response);

    close_connection(sockfd);

    // Etapa 5
    response_object = get_response_object(response);
    get_weather_details(response_object, &url, &method, &type, &weather_url, &q, &APPID, &weather_method);
    cookies = get_cookies(response);

    // Extragem numele serverului din url
    name = get_name(weather_url);
    // Obtinem ip-ul asociat acestui nume
    ip = get_ip(name);

    // Extragem url-ul din data.url
    cut_url(&weather_url);
    memset(url_params, 0, PARAMLEN);
    sprintf(url_params, "q=%s&APPID=%s", q, APPID);

    // Request
    sockfd = open_connection(ip, 80, AF_INET, SOCK_STREAM, 0);
    if (!strcmp(weather_method, "GET")) {
        message = compute_get_request(ip, weather_url, url_params, token, NULL);
    } else {
        message = compute_post_request(IP_SERVER, weather_url, NULL, type, token, cookies);
    }
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s \n", response);

    close_connection(sockfd);

    // Trimitem informatii despre vreme
    weather = get_weather(response);
    sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    if (!strcmp(method, "POST")) {
        message = compute_post_request(IP_SERVER, url, weather, type, token, cookies);
    } else {
        message = compute_get_request(IP_SERVER, url, NULL, token, cookies);
    }
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s \n", response);

    close_connection(sockfd);

    // Eliberam memoria alocata 
    free(message);
    free(weather);
    free(ip);
    free(name);
    free(login);
    free(url_params);
    free(cookies);

    return 0;
}