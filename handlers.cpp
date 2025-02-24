#include "handlers.h"
#include "getdb.h"
#include <nlohmann/json.hpp>
#include <httplib.h>
#include <iostream>
#include <ctime>
#include "sql.h"
#include "sqlext.h"


using json = nlohmann::json;

void hello_handler(const httplib::Request&, httplib::Response& res) {
    res.set_content("Hello from another source file!", "text/plain");
}

void json_handler(const httplib::Request&, httplib::Response& res) {
    json response = {
        {"message", "Hello, JSON World!"},
        {"status", "success"},
        {"timestamp", std::time(nullptr)}  
    };

    std::string json_str = response.dump(4); 
    res.set_content(json_str, "application/json; charset=utf-8"); 
}

void post_json_handler(const httplib::Request& req, httplib::Response& res) {
    try {
        json received_json = json::parse(req.body); 

        std::cout << "Received JSON: " << received_json.dump(4) << std::endl;

        json response = {
            {"message", "JSON received ayam bakar enak"},
            {"received_data", received_json},
            {"status", "success"}
        };

        res.set_content(response.dump(4), "application/json; charset=utf-8");
    }
    catch (const std::exception& e) {
        res.status = 400;  // Bad Request
        json error = {
            {"error", "Invalid JSON"},
            {"details", e.what()}
        };
        res.set_content(error.dump(4), "application/json; charset=utf-8");
    }
}

void json_handler_db(const httplib::Request& req, httplib::Response& res) {
    std::string jsonResponse = fetchFromTibero() +" "+ fetchFromTibero2();

    res.set_content(jsonResponse, "application/json");
}

//remote dealer 