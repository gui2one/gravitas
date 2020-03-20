#include "iss_data.h"

using json::JSON;
size_t write_to_string(void *ptr, size_t size, size_t count, std::string *stream)
{
	((std::string*)stream)->append((char*)ptr, 0, size*count);
	 return size*count;
}	



namespace Orbiter
{

	ISSData* ISSData::instance = 0;

	ISSData::ISSData() : Entity3d(){}

	ISSData* ISSData::getInstance(){
		if(instance == 0){
			instance = new ISSData();
			
		}
		
		return instance;
	}	
	

	
	IssPositionData ISSData::send_request()
	{
		JSON json_data;
		IssPositionData data;
		
		CURL * curl = curl_easy_init();
		if(curl)
		{
			CURLcode res;
			std::string response;
			//~ curl_easy_setopt(curl, CURLOPT_URL, "http://api.open-notify.org/iss-now.json");
			curl_easy_setopt(curl, CURLOPT_URL, "https://api.wheretheiss.at/v1/satellites/25544");
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
			
			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);

			json_data = JSON::Load(response);
			

			data.timestamp = json_data["timestamp"].ToInt();
			data.latitude = json_data["latitude"].ToFloat();
			data.longitude = json_data["longitude"].ToFloat();
			data.altitude = json_data["altitude"].ToFloat();
			data.velocity = json_data["velocity"].ToFloat();
			data.visibility = json_data["visibility"].ToFloat();
			data.daynum = json_data["daynum"].ToFloat();



			//~ printf("collected DayNum : %f\n", data.daynum);

		}

		return data;
	}	
	



		
}
