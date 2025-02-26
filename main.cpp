#include <httplib.h>
#include <iostream>
#include "handlers.h"
#include <curl/curl.h>
#include <string>
#include <nlohmann/json.hpp> // Include JSON library

using json = nlohmann::json; // Use JSON alias


size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output)
{
	size_t realsize = size * nmemb;
	output->append((char*)contents, realsize);
	return realsize;
}




void post_json_handler_curl(const httplib::Request& req, httplib::Response& res) {
    // Check for authentication header
    auto auth_header = req.get_header_value("Authorization");
    if (auth_header != "your mom") {
        res.status = 401;
        res.set_content("{\"error\": \"Unauthorized\"}", "application/json");
        return;
    }

    try {
        // Parse JSON request body
        json request_json = json::parse(req.body);

        // Example: Extract name from JSON
        std::string name = request_json.value("name", "Unknown");

        // Create a JSON response
        json response_json;
        response_json["message"] = "Hello, " + name + "!";
        
        res.set_content(response_json.dump(), "application/json");
    } catch (json::parse_error&) {
        res.status = 400;
        res.set_content("{\"error\": \"Invalid JSON\"}", "application/json");
    }
}

int main() {
    httplib::Server svr;
    CURL* curl;
    CURLcode res;
    std::string response_string;
   
    std::cout << "Start" << std::endl;
 /*
    std::cout << "before curl" << std::endl;
    curl = curl_easy_init;
    
    if (curl)
    {
		curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:9090/json");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

        res = curl_easy_perform(curl);
        
        if (res != CURLE_OK) {
            std::cerr << "cURL Error: " << curl_easy_strerror(res) << std::endl;
        }
        else {
            std::cout << "Response: " << response_string << std::endl;
        }
        
        curl_easy_cleanup(curl);
	}
	else {
		std::cerr << "Failed to initialize cURL" << std::endl;
	}

    curl = curl_easy_init;

    if (curl) 
    {
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:9090/json");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) 
        {

            std::cerr << "CURL Error: " << curl_easy_strerror(res) << std::endl;
        }
        else 
        {
            std::cerr << "Response: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }
    */

    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("Hello, API World!", "text/plain");
        });

    svr.Get("/hello ", hello_handler);

    //svr.Get("/json", json_handler);

    svr.Post("/json", post_json_handler);

    svr.Get("/json", json_handler_db);

    std::cout << "Server is running on http://localhost:9090\n";
    svr.listen("0.0.0.0", 9090);

    return 0;
}
