#ifndef HANDLERS_H
#define HANDLERS_H

#include <httplib.h>


void hello_handler(const httplib::Request&, httplib::Response& res);

void json_handler(const httplib::Request&, httplib::Response&);

void post_json_handler(const httplib::Request&, httplib::Response&);

void json_handler_db(const httplib::Request&, httplib::Response&);

#endif // HANDLERS_H
