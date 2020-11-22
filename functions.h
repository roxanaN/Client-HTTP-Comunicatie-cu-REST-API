#ifndef _FUNCTIONS_
#define _FUNCTIONS_

#define COOKIELEN 500
#define IPLEN 50
#define MAXLEN 1000
#define URLLEN 100

void get_details(JSON_Object *response_object, const char **url,
                 const char **method, const char **type, const char **username,
                 const char **password, const char **token, const char **id);
void get_some_details(JSON_Object *response_object, const char **url,
                      const char **method);                 
void get_weather_details(JSON_Object *response_object, const char **url,
                         const char **method, const char **type,
                         const char **weather_url, const char **q, const char **APPID,
                         const char **weather_method);
void cut_url(const char **url);
char *get_cookies(char *response);
char *get_ip(const char *hostname);
char *get_name(const char *url);
char *get_weather(const char *response);
JSON_Object *get_response_object(char *response);

#endif
