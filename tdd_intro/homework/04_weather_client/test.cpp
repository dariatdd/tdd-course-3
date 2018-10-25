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

using WeatherList = std::vector<Weather>;
using StringList = std::vector<std::string>;

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

Weather ConvertStringToWeather(const std::string& rawData);
WeatherList ConvertStringToWeather(const StringList& rawDataList);

class FakeWeatherServer : public IWeatherServer
{
    virtual std::string GetWeather(const std::string& request) override
    {
        static const std::map<std::string, std::string> weatherResults = {{"31.08.2018;03:00","20;181;5.1"},
                                                                          {"31.08.2018;09:00", "23;204;4.9"},
                                                                          {"31.08.2018;15:00", "33;193;4.3"},
                                                                          {"31.08.2018;21:00", "26;179;4.5"},

                                                                          {"01.09.2018;03:00", "19;176;4.2"},
                                                                          {"01.09.2018;09:00", "22;131;4.1"},
                                                                          {"01.09.2018;15:00", "31;109;4.0"},
                                                                          {"01.09.2018;21:00", "24;127;4.1"},

                                                                          {"02.09.2018;03:00", "21;158;3.8"},
                                                                          {"02.09.2018;09:00", "25;201;3.5"},
                                                                          {"02.09.2018;15:00", "34;258;3.7"},
                                                                          {"02.09.2018;21:00", "27;299;4.0"},
                                                                         };
        auto valueIt = weatherResults.find(request);
        if(valueIt != weatherResults.end())
        {
            return valueIt->second;
        }
        else
        {
            return "";
        }
    }
};

class WeatherClient : public IWeatherClient
{
public:
    virtual double GetAverageTemperature(IWeatherServer& server, const std::string& date) override
    {
        WeatherList weatherMarks = GetWeatherMarksForDay(server, date);
        double temperatureSum = 0.0;
        for(const auto& mark: weatherMarks)
        {
            temperatureSum += mark.temperature;
        }
        return temperatureSum / weatherMarks.size();
    }

    virtual double GetMinimumTemperature(IWeatherServer& server, const std::string& date) override
    {
        WeatherList weatherMarks = GetWeatherMarksForDay(server, date);
        auto minWeather = std::min_element(weatherMarks.begin(), weatherMarks.end(), [] (const Weather& left, const Weather& right)
        {
           return left.temperature < right.temperature;
        });

        return minWeather->temperature;
    }

    virtual double GetMaximumTemperature(IWeatherServer& server, const std::string& date) override
    {
        WeatherList weatherMarks = GetWeatherMarksForDay(server, date);
        auto maxWeather = std::max_element(weatherMarks.begin(), weatherMarks.end(), [] (const Weather& left, const Weather& right)
        {
          return left.temperature < right.temperature;
        });

        return maxWeather->temperature;
    }

    virtual double GetAverageWindDirection(IWeatherServer& server, const std::string& date) override
    {
        return 0;
    }

    virtual double GetMaximumWindSpeed(IWeatherServer& server, const std::string& date) override
    {
       return 0;
    }

    WeatherList GetWeatherMarksForDay(IWeatherServer& server, const std::string& date)
    {
        StringList weatherResults = {};
        const StringList times = {"03:00", "09:00", "15:00", "21:00"};
        for(const auto& time: times)
        {
            weatherResults.push_back(server.GetWeather(date + ";" + time));
        }

        return ConvertStringToWeather(weatherResults);
    }
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

WeatherList ConvertStringToWeather(const StringList& rawDataList)
{
    std::vector<Weather> result;
    for(const auto& data: rawDataList)
    {
        result.push_back(ConvertStringToWeather(data));
    }
    return result;
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

TEST(ConvertStringListToWeatherList, EmptyList)
{
    EXPECT_EQ(WeatherList(), ConvertStringToWeather(std::vector<std::string>()));
}

TEST(ConvertStringListToWeatherList, ValidList)
{
    StringList inputData = {"20;181;5.1", "23;204;4.9"};
    WeatherList etalon = {Weather {20, 181, 5.1},
                          Weather {23, 204, 4.9}};
    EXPECT_EQ(etalon,ConvertStringToWeather(inputData));
}


TEST(GetWeatherListForDate, Date3108)
{
    FakeWeatherServer server;
    WeatherClient client;
    WeatherList etalon = {Weather {20, 181, 5.1},
                          Weather {23, 204, 4.9},
                          Weather {33, 193, 4.3},
                          Weather {26, 179, 4.5}};
    EXPECT_EQ(etalon, client.GetWeatherMarksForDay(server, "31.08.2018"));
}

TEST(GetWeatherListForDate, Date0109)
{
    FakeWeatherServer server;
    WeatherClient client;
    WeatherList etalon = {Weather {19, 176, 4.2},
                          Weather {22, 131, 4.1},
                          Weather {31, 109, 4.0},
                          Weather {24, 127, 4.1}};
    EXPECT_EQ(etalon, client.GetWeatherMarksForDay(server, "01.09.2018"));
}

TEST(GetWeatherListForDate, IncorrectServerRequest)
{
   FakeWeatherServer server;
   WeatherClient client;
   WeatherList etalon = {Weather {19, 176, 4.2},
                         Weather {22, 131, 4.9},
                         Weather {31, 109, 4.0},
                         Weather {24, 127, 4.1}};
   EXPECT_THROW(client.GetWeatherMarksForDay(server, "01,09.2018"), std::runtime_error);
}

TEST(GetAverageTemperature, For3108)
{
   FakeWeatherServer server;
   WeatherClient client;
   EXPECT_EQ(25.5, client.GetAverageTemperature(server, "31.08.2018"));
}

TEST(GetAverageTemperature, Acceptance)
{
   FakeWeatherServer server;
   WeatherClient client;
   EXPECT_EQ(24, client.GetAverageTemperature(server, "01.09.2018"));
   EXPECT_EQ(26.75, client.GetAverageTemperature(server, "02.09.2018"));
}

TEST(GetMinimumTemperature, For3108)
{
   FakeWeatherServer server;
   WeatherClient client;
   EXPECT_EQ(20, client.GetMinimumTemperature(server, "31.08.2018"));
}

TEST(GetMinimumTemperature, Acceptance)
{
   FakeWeatherServer server;
   WeatherClient client;
   EXPECT_EQ(19, client.GetMinimumTemperature(server, "01.09.2018"));
   EXPECT_EQ(21, client.GetMinimumTemperature(server, "02.09.2018"));
}

TEST(GetMiaximumTemperature, For3108)
{
   FakeWeatherServer server;
   WeatherClient client;
   EXPECT_EQ(33, client.GetMaximumTemperature(server, "31.08.2018"));
}

