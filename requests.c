#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(const char *host, const char *url, const char *url_params,
                          const char *token, const char *cookies) {
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // Scriem numele metodei, calea, parametri din url (daca exista) si tipul protocolului
    if (url_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, url_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }
    compute_message(message, line);

    // Adaugam host-ul
    sprintf(line, "HOST: %s", host);
    compute_message(message, line);

    // PAS 3: Adaugam token si / sau cookies, respectand forma protocolului
    if (token) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }

    if (cookies) {
        sprintf(line, "Cookie: %spath=/; httponly", cookies);
        compute_message(message, line);
    }

    // End line
    // sprintf(line, "\r\n");
    compute_message(message, "");
    free(line);
    
    return message;
}

char *compute_post_request(const char *host, const char *url, const char *form_data,
                           const char *type, const char *token, const char *cookies) {

    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // Scriem numele metodei, calea si tipul protocolului
    sprintf(line, "\nPOST %s HTTP/1.1", url);
    compute_message(message, line);

    // Adaugam host-ul
    sprintf(line, "HOST: %s", host);
    compute_message(message, line);

    // Adaugam headere, token si/ sau cookies
    sprintf(line, "Content-Type: %s", type);
    compute_message(message, line);

    sprintf(line, "Content-Length: %d", (int)strlen(form_data));
    compute_message(message, line);

    if (token) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }

    if (cookies) {
        sprintf(line, "Cookie: %spath=/; httponly", cookies);
        compute_message(message, line);
    }
    
    compute_message(message, "");
    // compute_message(message, "");

    // Adaugam datele suplimentare
    compute_message(message, form_data);

    // End line
    compute_message(message, "");
    free(line);

    return message;
}
