#pragma once

#include <stdio.h>
#include <ctime>
#include <string>
#include <thread>
#include <curl/curl.h>
#include "pch.h"
#include "core.h"
#include "SimpleJSON/json.hpp"

namespace Orbiter
{

// url : http://api.open-notify.org/iss-now.json

struct IssPositionData
{
	
	double latitude;
	double longitude;
	double altitude;
	double velocity;
	double visibility;
	double footprint;
	
	int timestamp;
	double daynum;
	
	double solar_lat;
	double solar_lon;	
};



class ISSData : Entity3d
{
private:
	
	/* private constructor to prevent instancing */
	ISSData();
	static ISSData* instance;
	


public : 
	
	static ISSData* getInstance();
	IssPositionData send_request();


	inline const std::vector<IssPositionData>& getData()
	{
		return m_data;
	}	
	unsigned int m_max_positions = 500;
	std::vector<Orbiter::IssPositionData> m_data;	
	

};

	
}


