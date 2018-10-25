/*
Weather Client

You are going to develop a program that gets the statistics about weather in the current city
using information from a certain server. The goal is to calculate statistics using the data from weather server.

To communicate with the weather server you have to implement interface IWeatherServer,
which provides the raw string from the real server for the requested day and time.

The real server (i.e. "weather.com") gets the requests in this format:
"<date>;<time>", for example:
"31.08.2018;03:00"

The server answers on requests with string like this:
"20;181;5.1"
This string contains the weather for the requested time and means next:
"<air_temperature_in_celsius>;<wind_direction_in_degrees>:<wind_speed>".
Wind direction value may be in range from 0 to 359 inclusively, temperature may be negative.

The task:
1. Implement fake server, because interacting with real network is inacceptable within the unit tests.
To do this, you need to use real server responses. Fortunately, you've collected some results for the several dates from the weather server.
Each line means "<request>" : "<response>":

"31.08.2018;03:00" : "20;181;5.1"
"31.08.2018;09:00" : "23;204;4.9"
"31.08.2018;15:00" : "33;193;4.3"
"31.08.2018;21:00" : "26;179;4.5"

"01.09.2018;03:00" : "19;176;4.2"
"01.09.2018;09:00" : "22;131;4.1"
"01.09.2018;15:00" : "31;109;4.0"
"01.09.2018;21:00" : "24;127;4.1"

"02.09.2018;03:00" : "21;158;3.8"
"02.09.2018;09:00" : "25;201;3.5"
"02.09.2018;15:00" : "34;258;3.7"
"02.09.2018;21:00" : "27;299;4.0"

IMPORTANT:
* Server returns empty string if request is invalid.
* Real server stores weather only for times 03:00, 09:00, 15:00 and 21:00 for every date. Do not use other hours in a day.

2. Implement IWeatherClient using fake server.
*/

/*
  Assumptions:
1. To get average/minimal/maximum marks WeatherClient asks server for weather for times 03:00, 09:00, 15:00 and 21:00 and then calculates necessary value based on these data.
2. The weather is always returned from server in the further format 20;181;5.1 or as empty string in case of invalid request. The server returns data in valid format.

  Architecture:
1. WeatherClient is initialized with the implementation of IWeatherServer with functionality to get raw string with weather.
2. Weather client parses the obtained raw string into Weather structure and uses it then to calculate average/minimal/maximum marks.
3. There is a separate utils method ConvertStringToWeather to parse raw string to Weather object and list if strings to list of Weather objects.
4. WeatherClient also has separate method GetWeatherMarksForDay for getting weather for all time periods during the day; returns list of strings

   Steps:
 1. Check ConvertStringToWeather with empty string
 2. Check ConvertStringToWeather with valid weather string
 3. Check ConvertStringToWeather with valid list of strings
 4. Implement IWeatherServer and IWeatherClient with stubs
 5. Check GetAverageTemperature for 31.08
 6. Acceptance for GetAverageTemperature: check for 01.09, 02.09
 7. Check GetMinimumTemperature for 31.08
 8. Acceptance for GetMinimumTemperature: check for 01.09, 02.09
 9. Check GetMaximumTemperature for 31.08
 10. Acceptance for GetMaximumTemperature: check for 01.09, 02.09
 11. Check GetAverageWindDirection for 31.08
 12. Acceptance for GetAverageWindDirection: check for 01.09, 02.09
 13. Check GetMaximumWindSpeed for 31.08
 14. Acceptance for GetMaximumWindSpeed: check for 01.09, 02.09
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>

struct Weather
{
    short temperature;
    unsigned short windDirection;
    double windSpeed;

    bool operator==(const Weather& right) const
    {
        return temperature == right.temperature &&
               windDirection == right.windDirection &&
               std::abs(windSpeed - right.windSpeed) < 0.01;
    }
};

class IWeatherServer
{
public:
    virtual ~IWeatherServer() { }
    // Returns raw response with weather for the given day and time in request
    virtual std::string GetWeather(const std::string& request) = 0;
};

// Implement this interface
class IWeatherClient
{
public:
    virtual ~IWeatherClient() { }
    virtual double GetAverageTemperature(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetMinimumTemperature(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetMaximumTemperature(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetAverageWindDirection(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetMaximumWindSpeed(IWeatherServer& server, const std::string& date) = 0;
};


Weather ConvertStringToWeather(const std::string& rawData)
{
    if(rawData.empty())
    {
        throw std::runtime_error("Invalid format");
    }

    Weather weather;

    std::istringstream stream(rawData);
    stream >> weather.temperature;

    stream.ignore();
    stream >> weather.windDirection;
    stream.ignore();
    stream >> weather.windSpeed;

    return weather;
}

TEST(ConvertStringToWeather, EmptyString)
{
    EXPECT_THROW(ConvertStringToWeather(""), std::runtime_error);
}

TEST(ConvertStringToWeather, ValidString)
{
    Weather etalon {20, 181, 5.1};
    EXPECT_EQ(etalon, ConvertStringToWeather("20;181;5.1"));
}
