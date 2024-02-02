#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "model.cpp"



using std::string;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using nlohmann::json;

/*fonksiyon prtytiplerini burda cikartiyorz*/

size_t WriteCallback(void *, size_t, size_t, string *);
string getCompletion(const string&, const string& model ="gpt-3.5-turbo");  //interaktf set kuruldu..

//targetUrl="https://ecbs.meramedas.com.tr/bios/dpwebmap/dsg/contractor/cust_med/"

int main() {

    openai::start();

    cout << "prompt u gir: ";
    string prompt;
    getline(cin, prompt);
    string response =getCompletion(prompt);
    
    cout << response << endl;

      return 0;

}
  

/// girdiler bole olsın..

/// @brief 
/// @param contents 
/// @param size 
/// @param nmemb 
/// @param response 
/// @return 

/// serv için giris appt deger vs.. 

size_t WriteCallback(void * contents, size_t size, size_t nmemb, string * response) {

    size_t totalSize=size * nmemb;
    response -> append((char *) contents, totalSize);

    return totalSize;
}



// if Post==POST endppoint model tasaırmı icin apiden istek vsvs
//openai dan alinan benim apikeyw montelendi

string getCompletion(const string& prompt, const string& model = "gpt-3.5-turbo") {

    string apiKey = "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";
    string baseUrl= "https://api.openai.com/v1/chat/completions";
    string response;
    CURL * curl   = curl_easy_init();

    if(curl)
    {
        json requestData;
        requestData["model"]                    = model;
        requestData["messages"] [0] ["role"]    = "user";
        requestData["messages"] [0] ["content"] = prompt;
        requestData["temperature"]              = 0;

        string requestDataStr =requestData.dump().c_str();

        struct curl_slist* headers=      NULL;

        headers=    curl_slist_append(headers, "Content-Type: application/json");
        headers=    curl_slist_append(headers, ("Authotrization: Bearer" + apiKey).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, baseUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestDataStr.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, requestDataStr.length());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            cerr << "Curl request hatasi: " << curl_easy_strerror(res) << endl;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    ////return response;
    json jresponse      =  json::parse(response);
    return jresponse ["choises"] [0] ["message"] ["content"].get <string>();

}
